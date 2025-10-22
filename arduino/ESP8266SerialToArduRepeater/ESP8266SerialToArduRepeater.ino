// Setup instructions: https://forum.arduino.cc/t/arduino-mega-2560-serial-communication-to-esp8266-nodemcu/1057321/11

// ESP8266 SoftwareSerial
// https://circuits4you.com/2016/12/14/software-serial-esp8266/

#include <SoftwareSerial.h>

// pins Rx GPIO14 (D5) and Tx GPIO 12 (D6)
SoftwareSerial swSer(14, 12);  

void setup() {
  Serial.begin(9600);   //Initialize hardware serial with baudrate of 9600
  swSer.begin(9600);    //Initialize software serial with baudrate of 9600
  Serial.println("\nESP8266 Software serial test started");
}

void loop() {
  while (swSer.available() > 0) {  //wait for data at software serial
    Serial.write(swSer.read()); //Send data recived from software serial to hardware serial    
  }
  while (Serial.available() > 0) { //wait for data at hardware serial
    swSer.write(Serial.read());     //send data recived from hardware serial to software serial
  }
}