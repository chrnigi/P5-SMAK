#define PINA1 13 // D7
#define PINB1 12 // D6
#define PINC1 14 // D5
#define PINA2 2  // D4 -- if 5V on start then no boot
#define PINB2 0  // D3 -- if 5V on start then no boot
#define PINC2 4  // D2
#define PINA3 5  // D1
#define PINB3 16 // D0
#define PINC3 A0 // A0 -- has no PULLUP resistor, read using analogRead() instead
// GPIO 3, RX -- no boot if 5V OR GND
// GPIO 15, D8 -- pulled to ground, no boot if pulled HIGH 

// Very important table of which pins suck and which are usefull: https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/


bool A1value = true;
bool B1value = true;
bool C1value = true;
bool A2value = true;
bool B2value = true;
bool C2value = true;
bool A3value = true;
bool B3value = true;
bool C3value = true;

bool formerA1value = true;
bool formerB1value = true;
bool formerC1value = true;
bool formerA2value = true;
bool formerB2value = true;
bool formerC2value = true;
bool formerA3value = true;
bool formerB3value = true;
bool formerC3value = true;

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
		turn_counter = 10;
		return;
	}
	
	if (++turn_counter > 8)
	{
		serial_print_board();
		serial_print_draw();
		return;
	}
	
	is_player_x = !is_player_x;
	serial_print_board();
}

void printInputState(){
  Serial.print("A1 ");
  Serial.print("B1 ");
  Serial.print("C1 ");
  Serial.print("A2 ");
  Serial.print("B2 ");
  Serial.print("C2 ");
  Serial.print("A3 ");
  Serial.print("B3 ");
  Serial.print("C3 ");
  Serial.println();
  Serial.print(A1value);
  Serial.print("  ");
  Serial.print(B1value);
  Serial.print("  ");
  Serial.print(C1value);
  Serial.print("  ");
  Serial.print(A2value);
  Serial.print("  ");
  Serial.print(B2value);
  Serial.print("  ");
  Serial.print(C2value);
  Serial.print("  ");
  Serial.print(A3value);
  Serial.print("  ");
  Serial.print(B3value);
  Serial.print("  ");
  Serial.print(C3value);
  Serial.println();
}

void setup() {

  Serial.begin(9600);

  // PULLUP mode to avoid random value when floating
  pinMode(PINA1,INPUT_PULLUP);
  pinMode(PINB1,INPUT_PULLUP);
  pinMode(PINC1,INPUT_PULLUP);
  pinMode(PINA2,INPUT_PULLUP);
  pinMode(PINB2,INPUT_PULLUP);
  pinMode(PINC2,INPUT_PULLUP);
  pinMode(PINA3,INPUT_PULLUP);
  pinMode(PINB3,INPUT_PULLUP);
  pinMode(PINC3,INPUT); // Analog pins do have a PULLUP resistor

  Serial.println("\n\nSetup complete");
}

void loop() {

  A1value = digitalRead(PINA1);
  B1value = digitalRead(PINB1);
  C1value = digitalRead(PINC1);
  A2value = digitalRead(PINA2);
  B2value = digitalRead(PINB2);
  C2value = digitalRead(PINC2);
  A3value = digitalRead(PINA3);
  B3value = digitalRead(PINB3);
  int bleb = analogRead(PINC3);
  C3value = (bleb < 9);
  Serial.print("Analog C3: ");
  Serial.print(bleb);
  Serial.println();


  if (A1value != formerA1value){
    turn(0);
    formerA1value = A1value;
  }
  
  if (B1value != formerB1value){
    turn(1);
    formerB1value = B1value;
  }

  if (C1value != formerC1value){
    turn(2);
    formerC1value = C1value;
  }

  if (A2value != formerA2value){
    turn(3);
    formerA2value = A2value;
  }

  if (B2value != formerB2value){
    turn(4);
    formerB2value = B2value;
  }

  if (C2value != formerC2value){
    turn(5);
    formerC2value = C2value;
  }

   if (A3value != formerA3value){
    turn(6);
    formerA3value = A3value;
  } 

  if (B3value != formerB3value){
    turn(7);
    formerB3value = B3value;
  }

  if (C3value != formerC3value){
    turn(8);
    formerC3value = C3value;
  }

  delay(500);
}
