// Links:
// Wiring and serial example code: https://forum.arduino.cc/t/arduino-mega-2560-serial-communication-to-esp8266-nodemcu/1057321/11
// Software Reset: https://www.instructables.com/two-ways-to-reset-arduino-in-software/
// MQTT for use later on the ESP: https://www.emqx.com/en/blog/esp32-connects-to-the-free-public-mqtt-broker#getting-started-with-mqtt-on-esp32

// Digital Hall sensors: ACTIVE = magnet detected
// We poll every 10 ms with millis().

void(* resetFunc) (void) = 0;

const bool heartbeat = false;

const int SCAN_EVERY_MS = 10;    // scan every 10 ms
const int HITS_NEEDED   = 40;    // how many repeated reads to accept a change

const bool ACTIVE_LEVEL = LOW;   
const bool USE_PULLUPS  = true;

unsigned long time;

// 16 DIGITAL INPUT PINS
const int inputPins[16] = {
  A0, A1, A2, A3,
  A4, A5, A6, A7,
  A8, A9, A10, A11,
  A12, A13, A14, A15
};

// Labels for printing (4x4 example)
const char* names[16] = {
  "A4","B4","C4","D4",
  "A3","B3","C3","D3",
  "A2","B2","C2","D2",
  "A1","B1","C1","D1"
};

// Maps where on the physical 4x4 board, TTT is played and what turn index is used for a given field
const int mapPinToBoard[] = {
  0, 1 ,2, -1,
  3, 4 ,5, -1,
  6, 7 ,8, -1,
  -1,-1,-1,-1
};

/// state[i]: 0 = INACTIVE, 1 = ACTIVE
bool state[16];
int changeStreak[16];         // counts repeated confirmations of a proposed change
unsigned long scans = 0;      // how many scans we've done
unsigned long lastScan = 0;

bool gameHasStarted = 0;
long gameID = 0;

enum{NO_WINNER_YET, X_WON, Y_WON, DRAW} result = 0;

bool is_player_x = true;
int x_board = 0b000000000,
	y_board = 0b000000000;
int turn_counter = 0;

// Bit vectors for checking if someone has won
int victory_vectors [] = 
{
	0b1110000000, 0b000111000, 0b000000111, // rows
	0b100100100, 0b010010010, 0b001001001, // columns
	0b100010001, 0b001010100 // diagonals
};

// Test if someone has won the game
bool check_win(int board)
{
	for(int i = 0; i < 8; i++){ // 8 is the length of victory_vectors
		if ((board & victory_vectors[i]) == victory_vectors[i]) 
			return true;
	}
	return false;
}

// Check if a piece can be placed here
bool check_placement(int placement)
{
  //Serial.printf("Check placement: %d, %d", placement, (x_board | y_board));
	return ((x_board | y_board) // combine both boards to get a single board with all pieces
		& placement) == 0; // binary ANDs the board with the position, which should result in either a null vector or the placement vector 
}

void serial_print_victory()
{	
	Serial.println("***********");
	Serial.print("Game is over, player ");
  if(is_player_x){
    Serial.println("X won.");
  } else {
    Serial.println("O won.");
  }
	Serial.println("***********");
}
void serial_print_draw()
{
	Serial.println("***********");
	Serial.println("Game is over, game ended in a draw.");
	Serial.println("***********");
}
void serial_print_board()
{
	int combined_board = x_board | y_board;
	Serial.println("-------");
	for (int i = 0; i < 9; i++)
	{
		if (i%3 == 0) Serial.print("| ");
    if (((1 << i) & combined_board) == 0) Serial.print(" ");
		else Serial.print((((1 << i) & x_board) > 0) ? "X": "O");
		if (i%3 == 2) Serial.println(" |");
	}
	Serial.println("-------");
}

void turn(int sensor_nr){

  //printInputState();
  //Serial.printf("Player turn: %s\n", is_player_x ? "X" : "O");
  //Serial.printf("x_board : %d\n", x_board);
  //Serial.printf("y_board : %d\n", o_board);

	if (turn_counter > 9) return; // early end if the game is over
	sensor_nr = 1 << sensor_nr;
	
	int * current_board = is_player_x ? &x_board : &y_board;
	
	if (!check_placement( sensor_nr))
	{
		serial_print_board();
		Serial.println("Failed to place piece, this space is already occupied");
		return;
	}
	
	*current_board |= sensor_nr; // place the piece
	if (check_win(*current_board))
	{
		serial_print_board();
		serial_print_victory();
    if(is_player_x) result = X_WON; else result = Y_WON;
		turn_counter = 10;
		return;
	}
	
	if (++turn_counter > 8)
	{
		serial_print_board();
		serial_print_draw();
    result = DRAW;
		return;
	}
	
	is_player_x = !is_player_x;
	serial_print_board();
  sendGameState();
}

void command(String command){
  if (command == "reset\n"){
    Serial.println("Resetting!");
    Serial1.println("Arduino is Resetting");
    delay(500);
    resetFunc();
  }
}

String serial;
void readESPserial(){
  if (Serial1.available()){
    serial = Serial1.readString();
    Serial.print("Received string from ESP: ");
    Serial.print(serial);
    command(serial);
  }
}

void readSoftwareSerial(){
  if (Serial.available()){
    serial = Serial.readString();
    Serial.print("Received string from Software Serial: ");
    Serial.print(serial);
    command(serial);
  }
}

void sendGameState(){

if (!gameHasStarted){ // generate game ID at the first turn, using the time since reset as a random seed
  randomSeed(millis());
  gameID = random(2147483647); // this is NOT a UID however is should be good enough
  gameHasStarted = true;
}

  Serial1.print("Gamestate: ");
  Serial1.print(gameID);
  Serial1.print(" ");
  Serial1.print(is_player_x);
  Serial1.print(" ");
  Serial1.print(x_board);
  Serial1.print(" ");
  Serial1.print(y_board);
  Serial1.print(" ");
  Serial1.println(result);
  Serial1.println("------");
}

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600); 
  Serial.println("Setup!\n########################");

  for (int i = 0; i < 16; i++) {
    pinMode(inputPins[i], USE_PULLUPS ? INPUT_PULLUP : INPUT);
    state[i] = false;            // start as INACTIVE
    changeStreak[i] = 0;
  }


  Serial.println("Digital Hall scan started");
  Serial.print("ACTIVE_LEVEL = ");
  Serial.println(ACTIVE_LEVEL == LOW ? "LOW" : "HIGH");
  Serial1.println("Arduino has finished setup.");
}


void loop() {
  // scan every SCAN_EVERY_MS using millis()
  unsigned long now = millis();
  
  if (now - lastScan < (unsigned long)SCAN_EVERY_MS) return;

  // Read serial
  readSoftwareSerial();
  readESPserial();

  lastScan = now;
  scans++;

  // Read all sensors
  for (int i = 0; i < 16; i++) {
    int raw = digitalRead(inputPins[i]);
    bool proposed = (raw == ACTIVE_LEVEL) ? true : false;

    // Require the same proposal HITS_NEEDED times to accept change
    if (proposed != state[i]) {
      changeStreak[i]++;
      if (changeStreak[i] >= HITS_NEEDED) {
        state[i] = proposed;
        changeStreak[i] = 0;

        Serial.print(names[i]);
        Serial.print(" -> ");
        Serial.println(state[i] ? "ACTIVE" : "INACTIVE");

        if (state[i] == true){ // if a state changed to ACTIVE
          if (mapPinToBoard[i] != -1){ // Check if the field is a valid TTT field
            //Serial.println("Valid field");
            turn(mapPinToBoard[i]); // Run the turn corresponding to the field
            }
        }
      }
    } else {
      changeStreak[i] = 0;
    }
  }

  

  // Heartbeat about once per second (200 scans * 10 ms)
  if (scans % 200 == 0 && heartbeat) {
    Serial.println("Working");
  }
}
