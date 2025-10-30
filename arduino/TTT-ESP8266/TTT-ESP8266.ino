// Setup instructions serial: https://forum.arduino.cc/t/arduino-mega-2560-serial-communication-to-esp8266-nodemcu/1057321/11

// ESP8266 SoftwareSerial
// https://circuits4you.com/2016/12/14/software-serial-esp8266/

#include <SoftwareSerial.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h> // used to make the HTTP get request
#include <sstream>

// pins Rx GPIO14 (D5) and Tx GPIO 12 (D6)
SoftwareSerial swSer(14, 12);  
String serial;


// SSID the ESP8266 connects to for internet access
const char* ssid = "MSO ASUS Zenfone 10"; // WiFi network name
const char* password = "spejdernet"; // WiFi network password

const char* server = "api.restful-api.dev";
const int port = 8080;

struct gamestate_struct{
  int turn_counter;
  int x_board;
  int y_board;
  enum{X_WON, Y_WON, DRAW, IN_PROGRESS, } result = 3;
  int_64_t GameID = 0;
} gamestate;

bool readGamestate(String message){
  std::stringstream messageStream(message);
  std::string segment;

   if (std::getline(messageStream, segment, ' ') && segment == "Gamestate:") {
      std::getline(messageStream, segment, ' ')
      gamestate.turn_counter = stoi(segment);
      std::getline(messageStream, segment, ' ')
      gamestate.x_board= stoi(segment);
      std::getline(messageStream, segment, ' ')
      gamestate.y_board = stoi(segment);
      std::getline(messageStream, segment, ' ')
      gamestate.result = stoi(segment);
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
  String json = POST("/games","{\"result\":\"notstarted\"}");
  // Extract Game ID from json
  // gamestate.GameID =
}

void updateGame(){

}

void sendTurn(){


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

  Serial.println("success!");
  Serial.print("Local IP Address is: ");
  Serial.println(WiFi.localIP());
}

void exampleAPIrequest(){ // code from https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/examples/WiFiClient/WiFiClient.ino and https://www.techatomfusion.com/blogs/beginners-guide-calling-api-esp8266
  // Make HTTP request
  Serial.print("Connecting to ");
  Serial.println(server);
  WiFiClient client;
  if (!client.connect(server, port)) {
    Serial.println("Connection failed");
    return;
  }

  // Send HTTP request
  client.print("GET /objects HTTP/1.1\r\n");
  client.print("Host: ");
  client.print(server);
  client.print("\r\n\r\n");
  delay(10);

  // wait for data to be available
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      //delay(60000);
      return;
    }
  }

  // Read response
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  // Close the connection
  Serial.println("closing connection");
  client.stop();
  Serial.println("Example API call finished");
}

String POST(String path, String payload){ 

  String str = "";
  //server = ;
  //path = "/";
  //payload = "{\"name\": \"myTestObject\", \"data\": { \"Text\": \"Hello World!\" }}\r\n"; 

  Serial.println("-------");
  Serial.println(payload);
  Serial.println("-------");
  
  
  // Make HTTP POST request
  Serial.print("Connecting to ");
  Serial.println(server);
  WiFiClient client;
  if (!client.connect(server, port)) {
    Serial.println("Connection failed");
    return;
  }

  // Send HTTP POST request
  client.print("POST ");
  client.print(path);
  client.print(" HTTP/1.1\r\n");
  client.print("Host: ");
  client.print(server);
  client.print("\r\n");
  client.print("Content-Type:application/json\r\n");
  client.print("Content-Length:");
  client.print(payload.length());
  //client.print("Accept:text/plain\r\n");
  client.print("\r\n");
  client.print("\r\n");

  client.print(payload);
  client.print("\r\n");


  delay(10);

  // wait for data to be available
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      //delay(60000);
      return "";
    }
  }

  // Read response
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
    str.append(line);
  }

  // Close the connection
  Serial.println("closing connection");
  client.stop();
  Serial.println("Example API call finished");

  return str;
}

void setup() {
  Serial.begin(9600);   //Initialize hardware serial with baudrate of 9600
  swSer.begin(9600);    //Initialize software serial with baudrate of 9600
  delay(1000);

  wifiSetup();
  delay(500);
  Serial.println("\n################################################################################\n");
  //exampleAPIrequest();
  POST("");
  Serial.println("\n################################################################################");

}


void loop() {
  while (swSer.available() > 0) {  //wait for data at software serial
    serial = swSer.readStringUntil('\n');
    Serial.println(serial);
    if (readGameState(serial)){ //returns true if serial is a gamestate
      if (isGameNewGame()) createnewGame();
      sendTurn();
      if (!isGameInProgress()) updateGame();
    }

    //Serial.write(swSer.read()); //Send data recived from software serial to hardware serial    
  }
  while (Serial.available() > 0) { //wait for data at hardware serial
    swSer.write(Serial.read());     //send data received from hardware serial to software serial
  }



}