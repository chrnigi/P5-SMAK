#include "ChessFSM.h"
#include "ChessNetworking.h"
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

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
  
  gameState.id = doc["id"];
}

void updateGame(){
  WiFiClient client;
  HTTPClient http;
  http.begin(client, endpoint("/games/" + String(gameState.id))); 

  http.addHeader("Content-Type", "application/json");

  String payload;
  JsonDocument doc;
  doc["id"] = gameState.id;
  doc["gamestate"] = gameState.state_string;

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
  doc["id"] = gameState.id;
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