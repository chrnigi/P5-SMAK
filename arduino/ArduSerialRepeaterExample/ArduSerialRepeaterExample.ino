// Setup instructions: https://forum.arduino.cc/t/arduino-mega-2560-serial-communication-to-esp8266-nodemcu/1057321/11

// Arduino Mega serial1 test

// mega pin 18 is Tx
//      pin 19 is Rx
// for loopback test connect pin 18 to pin 19

// for RS232 shield connect pin 18 to Tx and pin 19 to Rx
// for loopback test connect 9 pin D connector pins 2 and 3

unsigned long time;

void setup() {
  Serial.begin(9600);   // initialise serial monitor port
  Serial1.begin(9600);  // initialise Serial1
  Serial.write("Arduino Mega Serial1 test -  for loopback test connect pin 18 to pin 19\n");
}

void loop() {
  if (Serial1.available())        // read from Serial1 output to Serial
    Serial.write(Serial1.read());
  if (Serial.available()) {       // read from Serial output to Serial1
    int inByte = Serial.read();
    //Serial.write(inByte);     // local echo if required
    Serial1.write(inByte);
  }
}