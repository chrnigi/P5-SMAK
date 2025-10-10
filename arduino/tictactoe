bool is_player_x = true;
int x_board = 0b000000000,
	y_board = 0b000000000;
int turn_counter = 0;

// Bit vectors for checking if someone has won
int victory_vectors [] = 
{
	0b1110000000, 0b000111000, 0b000000111, // rows
	0b100100100, 0b010010010, 0b001001001, // columns
	0b100010001, 0b001010100, // diagonals
}

// Test if someone has won the game
bool check_win(int board)
{
	for(int i = 0; i < 8; i++){ // 8 is the length of victory_vectors
		if (board & victory_vectors[i] == victory_vectors[i]) 
			return true;
	}
	return false;
}

// Check if a piece can be placed here
bool check_placement(int placement)
{
	return (x_board | y_board) // combine both boards to get a single board with all pieces
		& placement == 0; // binary ANDs the board with the position, which should result in either a null vector or the placement vector 
}

void serial_print_victory()
{	
	Serial.println("***********");
	Serial.println("Game is over, player " + is_player_x ? "X" : "O" + " won.");
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
	Serial.println("-----");
	for (int i = 0; i < 9; i++)
	{
		if (i&3 == 0) Serial.print("|");
		Serial.print(1 << i & combined_board ? 
			(i << i & x_board "X": "O") 
			: " ");
		if (i&3 == 2) Serial.println("|");
	}
	Serial.println("-----");
}

void turn(int sensor_nr){
	if (turn_counter > 9) return; // early end if the game is over
	sensor_nr = 1 << sensor_nr;
	
	int * current_board = is_player_x ? &x_board : &y_board;
	
	if (!check_placement(int sensor_nr))
	{
		serial_print_board();
		Serial.println("Failed to place piece, this space is already occupied");
		return;
	}
	
	*current_board |= sensor_nr; // place the piece
	if (check_win(*board))
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
