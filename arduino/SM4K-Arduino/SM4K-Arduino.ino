// Digital Hall sensors: ACTIVE = magnet detected
// We poll every 10 ms with millis().

void(* resetFunc) (void) = 0;

const int SCAN_EVERY_MS = 10;    // scan every 10 ms
const int HITS_NEEDED   = 40;    // how many repeated reads to accept a change

const bool ACTIVE_LEVEL = LOW;   
const bool USE_PULLUPS  = true;

unsigned long time;

String commands;

// 64 DIGITAL INPUT PINS
const int inputPins[64] = {
  A0, A1, A2, A3, 21, 20 ,19 ,18,
  A4, A5, A6, A7, 17, 16, 2, 3,
  A8, A9, A10, A11, 4, 5, 6, 7,
  A12, A13, A14, A15, 8, 9, 10, 11,
  53, 51, 49, 47, 52, 50, 48, 46,
  45, 43, 41, 39, 44, 42, 40, 38,
  37, 35, 33, 31, 36, 34, 32, 30,
  29, 27, 25, 23, 28, 26, 24, 22
};

// Labels for printing (8x8 example)
const char* names[64] = {
"H1", "H2", "H3", "H4", "H5", "H6", "H7", "H8", 
"G1", "G2", "G3", "G4", "G5", "G6", "G7", "G8", 
"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", 
"E1", "E2", "E3", "E4", "E5", "E6", "E7", "E8", 
"D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", 
"C1", "C2", "C3", "C4", "C5", "C6", "C7", "C8", 
"B1", "B2", "B3", "B4", "B5", "B6", "B7", "B8", 
"A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8"
};

/// state[i]: 0 = INACTIVE, 1 = ACTIVE
bool state[64];
int changeStreak[64];         // counts repeated confirmations of a proposed change for each coordinate
unsigned long scans = 0;      // how many scans we've done
unsigned long lastScan = 0;

void command(String command){
  if (command == "reset\n"){
    Serial.println("Resetting!");
    Serial3.println("Arduino is Resetting");
    delay(500);
    resetFunc();
  }
  else if (command == "resetChess\n"){
    Serial.println("Resetting chess game!");
    Serial3.print("resetChess");
  }
}

//Print output from the ESP8266
void readESPserial(){
  if (Serial3.available()){
    Serial.print("Received string from ESP: ");
    Serial.print(Serial3.readString());
  }
}

void setup() {
  Serial.begin(9600);
  Serial3.begin(9600); 
  Serial.println("Setup!\n########################");

  for (int i = 0; i < 64; i++) {
    pinMode(inputPins[i], USE_PULLUPS ? INPUT_PULLUP : INPUT);
    state[i] = false;            // start as INACTIVE
    changeStreak[i] = 0;
  }

  Serial3.println("Arduino has finished setup.");
}

void loop() {
 // scan every SCAN_EVERY_MS using millis()
  unsigned long now = millis();

  while (Serial.available() > 0) {
  commands = Serial.readString();
  command(commands);
  }

  readESPserial();

  if (now - lastScan < (unsigned long)SCAN_EVERY_MS) return;

  lastScan = now;
  scans++;

  // Read all sensors
  for (int i = 0; i < 64; i++) {
    int raw = digitalRead(inputPins[i]);
    bool proposed = (raw == ACTIVE_LEVEL) ? true : false;

    // Require the same proposal HITS_NEEDED times to accept change
    if (proposed != state[i]) {
      changeStreak[i]++;
      if (changeStreak[i] >= HITS_NEEDED) {
        state[i] = proposed;
        changeStreak[i] = 0;

        Serial.print("PinChanges: ");
        Serial.print(i);
        Serial.print(" ");
        Serial.println(state[i]);

        Serial3.print("PinChanges: ");
        Serial3.print(i);
        Serial3.print(" ");
        Serial3.println(state[i]);
      }
    } else {
      changeStreak[i] = 0;
    }
  }
}
