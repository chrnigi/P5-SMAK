// Setup instructions serial: https://forum.arduino.cc/t/arduino-mega-2560-serial-communication-to-esp8266-nodemcu/1057321/11

// ESP8266 SoftwareSerial
// https://circuits4you.com/2016/12/14/software-serial-esp8266/

#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <sstream>
#include "ChessFSM.h"

String serial;

// SSID the ESP8266 connects to for internet access
const char* ssid = "MSO ASUS Zenfone 10"; // WiFi network name
const char* password = "spejdernet"; // WiFi network password
bool connectedToWifi = false;

String server = "130.225.39.244";
String port = "8080";
String path = "/games";

String endpoint(){
  return ("http://"+ server + ":" + port + path);
}
String endpoint(String newPath){
  path = newPath;
  return endpoint();
}
String endpoint(String newPort, String newPath){
  port = newPort;
  path = newPath;
  return endpoint();
}
String endpoint(String newServer, String newPort, String newPath){
  server = newServer;
  port = newPort;
  path = newPath;
  return endpoint();
}

struct gameChange_struct{
  int pinChanged;
  bool state;
  int64_t GameID = 0;
  String gamestate = "NOT_STARTED";
} gameChange;

bool readPinChange(String message){
  std::stringstream messageStream(message.c_str());
  std::string segment;
  
   if (std::getline(messageStream, segment, ' ') && segment == "PinChanges:") {
      std::getline(messageStream, segment, ' ');
      gameChange.pinChanged = stoi(segment);

      std::getline(messageStream, segment, ' ');
      gameChange.state= stoi(segment);

      return true;
   }
   return false;
}

void createNewGame(){   
  WiFiClient client;
  HTTPClient http;
  //http.begin(client,"http://130.225.39.244:8080/games");
  http.begin(client, endpoint("/games"));

  //Serial.println(String( endpoint("/games")));

  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST("{\"gamestate\":\"NOT_STARTED\"}");

  String payload = "{}"; 

  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
    Serial.println("createNewGame: " + endpoint() + "   " + payload);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
    http.end();
    return;
  }
  http.end();

  JsonDocument doc;
  deserializeJson(doc, payload);
  
  gameChange.GameID = doc["id"];
}

void updateGame(){
  WiFiClient client;
  HTTPClient http;
  http.begin(client, endpoint("/games/" + String(gameChange.GameID))); 

  http.addHeader("Content-Type", "application/json");

  String payload;
  JsonDocument doc;
  doc["id"] = gameChange.GameID;
  doc["gamestate"] = gameChange.gamestate;

  serializeJson(doc, payload);

  Serial.println("updateGame: " + endpoint() + "   " + payload);

  int httpResponseCode = http.PATCH(payload);

  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}

//Start wifi setup
void wifiSetup(){
  // Documentation: https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/generic-class.html
  WiFi.begin(ssid, password); 
  WiFi.mode(WIFI_STA);

  // Connecting to Wifi Network
  Serial.println();
  Serial.print("Connecting to WiFi");

  unsigned long timeout = millis();
  while (WiFi.status() != WL_CONNECTED){
    if (millis() - timeout > 10000) {
      Serial.println(">>> WiFi Connection timed out");
      return;
    }
    delay(500);
    Serial.print(".");
  }
  connectedToWifi = true;
  Serial.println("success!");
  Serial.print("Local IP Address is: ");
  Serial.println(WiFi.localIP());
}

//clear serial buffer
void clearSerialInput() {
  while (Serial.available() > 0) {  //wait for data at software serial
    Serial.readStringUntil('\n');
  }
}
void setup() {
  Serial.begin(9600);   //Initialize hardware serial with baudrate of 9600
  Serial.println("\n################################################################################\n");
  wifiSetup();
  //delay(500);
  Serial.println("\n################################################################################\n");
  init_starting_board();

  std::string printBoard=print_board();
  Serial.println(printBoard.c_str());
  clearSerialInput();
  createNewGame();
}

void loop() {
  while (Serial.available() > 0) {  //wait for data at software serial
    serial = Serial.readStringUntil('\n');
    Serial.println(serial);

    if (readPinChange(serial)){ //returns true if serial is a pin change
      Serial.print(gameChange.pinChanged);
      Serial.print(" Is up: ");
      Serial.println(!gameChange.state);

      // Control wifi status
      if(WiFi.status()== !WL_CONNECTED){
        Serial.println("Failed to send gamestate, wifi disconnected");
        }
      pin_change(gameChange.pinChanged,!gameChange.state);
      Serial.print("Gamestate: ");
      Serial.println(returnState());
      std::string printBoard=print_board();
      Serial.println(printBoard.c_str());
    }

    //If "resetChess" is received from the Arduino, reset the chess state and board, and create a new game on server
    if (serial=="resetChess") {
      Serial.println("ESP: Resetting chess game!");
      clean_state();
      init_starting_board();
      createNewGame();

      Serial.println("ESP: Chess game has been reset!");
    }
  }

  // Prints WiFi status if it has changed
  if (connectedToWifi != (WiFi.status()== WL_CONNECTED)){
    connectedToWifi = !connectedToWifi;
    if (connectedToWifi){
      Serial.println("WiFi connected");
    } else {
      Serial.println("WiFi disconnected");
    }
  }
}

void sendMove(const int ply, const int from, const int to, 
	const char piece, const char captured, 
	const bool promotion, const bool en_passant, const bool kingside_castle, const bool queenside_castle)
{
  WiFiClient client;
  HTTPClient http;
  http.begin(client, endpoint("/moves"));

  http.addHeader("Content-Type", "application/json");
  
  //Create payload
  String payload;
  JsonDocument doc;
  doc["id"] = gameChange.GameID;
  doc["ply_number"] = ply;
  if (en_passant) {
    doc["move_type"] = "enpassant";
  } else if (kingside_castle || queenside_castle) {
    doc["move_type"] = "castling";
  } else if (promotion) {
    doc["move_type"] = "promotion";
  } else {
    doc["move_type"] = "normal";
  }
  doc["piece_moved"] = String(piece);
  if (captured == ' ') {
    doc["piece_captured"] = "Z";
  } else {
    doc["piece_captured"] = String(captured);
  } 
  doc["from_square"] = from;
  doc["to_square"] = to;

  serializeJson(doc, payload);

  Serial.println("sendMove: " + endpoint() + "  " + payload);

  int httpResponseCode = http.POST(payload);

  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}

//If move is registered as an error, update the gamestate to ERROR and post to endpoint.
static void setErrorAndSend()
{
  state = states::error;
	gameChange.gamestate = "ERROR";
  updateGame();
}
