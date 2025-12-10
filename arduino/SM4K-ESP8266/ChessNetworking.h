#pragma once

#include <string>
#include <ESP8266WiFi.h>

// SSID the ESP8266 connects to for internet access
const char* ssid = "MSO ASUS Zenfone 10"; // WiFi network name
const char* password = "spejdernet"; // WiFi network password
bool connectedToWifi = false;

String server = "130.225.39.244";
String port = "8080";
String path = "/games";

String endpoint();
String endpoint(String newPath);
String endpoint(String newPort, String newPath);
String endpoint(String newServer, String newPort, String newPath);

void wifiSetup();

void createNewGame();

void updateGame();

void sendMove(const int ply, const int from, const int to, 
	const char piece, const char captured, 
	const bool promotion, const bool en_passant, const bool kingside_castle, const bool queenside_castle);


