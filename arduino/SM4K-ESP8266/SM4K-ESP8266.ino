// Setup instructions serial: https://forum.arduino.cc/t/arduino-mega-2560-serial-communication-to-esp8266-nodemcu/1057321/11

// ESP8266 SoftwareSerial
// https://circuits4you.com/2016/12/14/software-serial-esp8266/

#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <sstream>
#include "pch.h"


// pins Rx GPIO14 (D5) and Tx GPIO 12 (D6)
//SoftwareSerial Serial(14, 12);  
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

int64_t GameID = 0;

struct gameChange_struct{
  int pinChanged;
  bool state;
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

  int httpResponseCode = http.POST("{\"result\":\"inprogress\"}");

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
  
  GameID = doc["id"];
}


void updateGame(){
  WiFiClient client;
  HTTPClient http;
  http.begin(client, endpoint("/games/" + String(GameID))); 

  http.addHeader("Content-Type", "application/json");

  String payload;
  JsonDocument doc;
  doc["id"] = GameID;
  doc["result"] = "inprogress";


  serializeJson(doc, payload);

  Serial.println("updateGame: " + endpoint() + "   " + payload);

  int httpResponseCode = http.PATCH(payload);

  http.begin(client, endpoint("/games/" + String(GameID))); 
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
  for (int i = 0; i < 50; i++) {
    while (Serial.available() > 0)
      {
      char k = Serial.read();
      delay(1);
      }
    delay(1);
  }
}
void setup() {
  Serial.begin(9600);   //Initialize hardware serial with baudrate of 9600
  //Serial.begin(9600);    //Initialize software serial with baudrate of 9600
  //delay(1000);
  Serial.println("\n################################################################################\n");
  wifiSetup();
  //delay(500);
  Serial.println("\n################################################################################\n");
  //init_starting_board();

  //initialize test board with pawns in each corner
  clean_state();
  board[63] = p_WHITE_PAWN; 
  board[56] = p_WHITE_PAWN;
  board[0] =  p_BLACK_PAWN;
  board[7] = p_BLACK_PAWN;
  std::string printBoard=print_board();
  Serial.println(printBoard.c_str());
  clearSerialInput();
  //createNewGame();
}

void loop() {
  while (Serial.available() > 0) {  //wait for data at software serial
    serial = Serial.readStringUntil('\n');

    if (readPinChange(serial)){ //returns true if serial is a pin change
      pin_change(gameChange.pinChanged,!gameChange.state);
      Serial.print(gameChange.pinChanged);
      Serial.print(" Is up: ");
      Serial.println(!gameChange.state);
      std::string printBoard=print_board();
      Serial.println(printBoard.c_str());

      // Control wifi status
      if(WiFi.status()== WL_CONNECTED){
        //if (isGameNewGame()) createNewGame();
        //sendMove();
        if (!isGameInProgress()) updateGame();
      } else {
        Serial.println("Failed to send gamestate, wifi disconnected");
      }
      
    }
    if (serial=="resetChess") {
      Serial.println("ESP: Resetting chess game!");
      reset();
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
	const bool promotion = false, const bool en_passant = false, const bool kingside_castle = false, const bool queenside_castle = false)
{
  WiFiClient client;
  HTTPClient http;
  http.begin(client, endpoint("/boardstates"));

  http.addHeader("Content-Type", "application/json");
  
  String payload;
  JsonDocument doc;
  doc["id"] = GameID;
  doc["ply_number"] = ply;
  if (en_passant) {
    doc["move_type"] = "enpassant";
  } else if (kingside_castle | queenside_castle) {
    doc["move_type"] = "castling";
  } else if (promotion) {
    doc["move_type"] = "promotion";
  } else {
    doc["move_type"] = "normal";
  }
  doc["piece_moved"] = toupper(piece);
  doc["piece_captured"] = captured;
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

  static void setErrorAndSend()
{
	state = states::error;

}