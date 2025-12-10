// Setup instructions serial: https://forum.arduino.cc/t/arduino-mega-2560-serial-communication-to-esp8266-nodemcu/1057321/11

// ESP8266 SoftwareSerial
// https://circuits4you.com/2016/12/14/software-serial-esp8266/

#include <SoftwareSerial.h>
#include "ChessFSM.h"
#include "ChessNetworking.h"

String serial;

struct gameState_struct{
  int pinChanged;
  bool is_up;
  int64_t id = 0;
  String state_string = "NOT_STARTED";
} gameState;

bool readPinChange(String message){
  std::stringstream messageStream(message.c_str());
  std::string segment;
  
   if (std::getline(messageStream, segment, ' ') && segment == "PinChanges:") {
      std::getline(messageStream, segment, ' ');
      gameState.pinChanged = stoi(segment);

      std::getline(messageStream, segment, ' ');
      gameState.is_up= stoi(segment);

      return true;
   }
   return false;
}

//If move is registered as an error, update the state_string to ERROR and post to endpoint.
static void setErrorAndSend()
{
  state = states::error;
	gameState.state_string = "ERROR";
  updateGame();
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
      Serial.print(gameState.pinChanged);
      Serial.print(" Is up: ");
      Serial.println(!gameState.is_up);

      // Control wifi status
      if(WiFi.status()== !WL_CONNECTED){
        Serial.println("Failed to send gamestate, wifi disconnected");
        }
      pin_change(gameState.pinChanged,!gameState.is_up);
      Serial.print("Gamestate: ");
      Serial.println(returnState());
      std::string printBoard=print_board();
      Serial.println(printBoard.c_str());
    }

    //If "resetChess" is received from the Arduino, reset the chess gamestate and board, and create a new game on server
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
