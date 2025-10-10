// Digital Hall sensors: ACTIVE = magnet detected
// We poll every 5 ms with millis().

const int SCAN_EVERY_MS = 5;    // scan every 5 ms
const int HITS_NEEDED   = 2;    // how many repeated reads to accept a change

const bool ACTIVE_LEVEL = LOW;   
const bool USE_PULLUPS  = true;

// 16 DIGITAL INPUT PINS
const int inputPins[16] = {
  A0, A1, A2, A3,
  A4, A5, A6, A7,
  A8, A9, A10, A11,
  A12, A13, A14, A15
};

// Labels for printing (4x4 example)
const char* names[16] = {
  "A1","B1","C1","D1",
  "A2","B2","C2","D2",
  "A3","B3","C3","D3",
  "A4","B4","C4","D4"
};

/// state[i]: 0 = INACTIVE, 1 = ACTIVE
int state[16];
int changeStreak[16];         // counts repeated confirmations of a proposed change
unsigned long scans = 0;      // how many scans we've done
unsigned long lastScan = 0;

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < 16; i++) {
    pinMode(inputPins[i], USE_PULLUPS ? INPUT_PULLUP : INPUT);
    state[i] = 0;            // start as INACTIVE
    changeStreak[i] = 0;
  }

  Serial.println("Digital Hall scan started");
  Serial.print("ACTIVE_LEVEL = ");
  Serial.println(ACTIVE_LEVEL == LOW ? "LOW" : "HIGH");
}

void loop() {
  // scan every SCAN_EVERY_MS using millis()
  unsigned long now = millis();
  if (now - lastScan < (unsigned long)SCAN_EVERY_MS) return;
  lastScan = now;
  scans++;

  // Read all 16 sensors
  for (int i = 0; i < 16; i++) {
    int raw = digitalRead(inputPins[i]);
    int proposed = (raw == ACTIVE_LEVEL) ? 1 : 0;

    // Require the same proposal HITS_NEEDED times to accept (anti-flicker)
    if (proposed != state[i]) {
      changeStreak[i]++;
      if (changeStreak[i] >= HITS_NEEDED) {
        state[i] = proposed;
        changeStreak[i] = 0;

        Serial.print(names[i]);
        Serial.print(" -> ");
        Serial.println(state[i] ? "ACTIVE" : "INACTIVE");
      }
    } else {
      changeStreak[i] = 0;
    }
  }

  // Heartbeat about once per second (200 scans * 5 ms ≈ 1 s)
  if (scans % 200 == 0) {
    Serial.println("Working");
  }
}

  /* 
  const char* names[64] = { 
  A1..H1 "A1","B1","C1","D1","E1","F1","G1","H1",
  A2..H2 "A2","B2","C2","D2","E2","F2","G2","H2",
  A3..H3 "A3","B3","C3","D3","E3","F3","G3","H3",
  A4..H4 "A4","B4","C4","D4","E4","F4","G4","H4",
  A5..H5 "A5","B5","C5","D5","E5","F5","G5","H5",
  A6..H6 "A6","B6","C6","D6","E6","F6","G6","H6",
  A7..H7 "A7","B7","C7","D7","E7","F7","G7","H7", 
  A8..H8 "A8","B8","C8","D8","E8","F8","G8","H8" 
  }; 
  */



