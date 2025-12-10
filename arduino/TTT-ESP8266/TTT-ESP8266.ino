// Setup instructions serial: https://forum.arduino.cc/t/arduino-mega-2560-serial-communication-to-esp8266-nodemcu/1057321/11

// ESP8266 SoftwareSerial
// https://circuits4you.com/2016/12/14/software-serial-esp8266/

#include <SoftwareSerial.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <sstream>

// pins Rx GPIO14 (D5) and Tx GPIO 12 (D6)
SoftwareSerial swSer(14, 12);  
String serial;


// SSID the ESP8266 connects to for internet access
const char* ssid = "MSO ASUS Zenfone 10"; // WiFi network name
const char* password = "spejdernet"; // WiFi network password
bool connectedToWifi = false;

String server = "p5webserv.head9x.dk";
String port = "8081";
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

struct gamestate_struct{
  int turn_counter;
  int x_board;
  int y_board;
  enum result_enum{X_WON, O_WON, DRAW, IN_PROGRESS, } result = IN_PROGRESS;
  int64_t GameID = 0;
} gamestate;

String resultAsString(int input){
  switch (input){
    case gamestate_struct::result_enum::X_WON:
      return "xwin";
    case gamestate_struct::result_enum::O_WON:
      return "owin";
    case gamestate_struct::result_enum::DRAW:
      return "draw";
    default:
      return "inprogress";    
  }
}

bool readGamestate(String message){
  std::stringstream messageStream(message.c_str());
  std::string segment;

   if (std::getline(messageStream, segment, ' ') && segment == "Gamestate:") {
      std::getline(messageStream, segment, ' ');
      gamestate.turn_counter = stoi(segment);

      std::getline(messageStream, segment, ' ');
      gamestate.x_board= stoi(segment);

      std::getline(messageStream, segment, ' ');
      gamestate.y_board = stoi(segment);

      std::getline(messageStream, segment, ' ');
      gamestate.result = (gamestate_struct::result_enum)stoi(segment);

      return true;
   }
   return false;
}

bool isGameNewGame(){
  return (gamestate.turn_counter == 1);
}

bool isGameInProgress(){
  return (gamestate.result == gamestate_struct::IN_PROGRESS);
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
  
  gamestate.GameID = doc["id"];
}

void updateGame(){
  WiFiClient client;
  HTTPClient http;
  http.begin(client, endpoint("/games/" + String(gamestate.GameID)));

  http.addHeader("Content-Type", "application/json");

  String payload = "{\"result\":\"" + resultAsString(gamestate.result) + "\", \"id\" : " + gamestate.GameID +" }";

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

void sendTurn(){
  WiFiClient client;
  HTTPClient http;
  http.begin(client, endpoint("/boardstates"));
  //Serial.print("Send turn: ");
  //Serial.println(endpoint("/boardstates"));

  http.addHeader("Content-Type", "application/json");
  
  String payload;
  JsonDocument doc;
  doc["id"] = gamestate.GameID;
  doc["x_placement"] = gamestate.x_board;
  doc["o_placement"] = gamestate.y_board;
  doc["moveno"] = gamestate.turn_counter;

  serializeJson(doc, payload);

  Serial.println("sendTurn: " + endpoint() + "  " + payload);

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


void setup() {
  Serial.begin(9600);   //Initialize hardware serial with baudrate of 9600
  swSer.begin(9600);    //Initialize software serial with baudrate of 9600
  //delay(1000);
  Serial.println("\n################################################################################\n");
  wifiSetup();
  //delay(500);
  Serial.println("\n################################################################################\n");

}

void loop() {
  while (swSer.available() > 0) {  //wait for data at software serial
    serial = swSer.readStringUntil('\n');
    Serial.println(serial);

    
    if (readGamestate(serial)){ //returns true if serial is a gamestate
      if(WiFi.status()== WL_CONNECTED){

        if (isGameNewGame()) createNewGame();
        sendTurn();
        if (!isGameInProgress()) updateGame();

      } else {
        Serial.println("Failed to send gamestate, wifi disconnected");}
    }
    
    //Serial.write(swSer.read()); //Send data recived from software serial to hardware serial    
  }
  while (Serial.available() > 0) { //wait for data at hardware serial
    swSer.write(Serial.read());     //send data received from hardware serial to software serial
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