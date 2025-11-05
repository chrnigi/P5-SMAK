#include "pch.h"
#include <iostream>
#include <string>
#include <Windows.h>

void pretty_print_board();

void main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();

	using std::cout;
	using std::string;

	//init_empty_board();
	//board[1] = BLACK_KING;
	std::cout << print_board() << "\n";

	pretty_print_board();

	pin_change(64 - 9, true);
	pin_change(64 - 17, false);
	pretty_print_board();
	pin_change(8, true);
	pin_change(24, false);
	pin_change(63, true);
	pin_change(63 - 8, false);
	pin_change(9, true);
	pin_change(25, false);
	//pin_change(63 - 15, true);
	//pin_change(63 - 15 - 16, false);
	pretty_print_board();

	std::cout << print_fen_string() << "\n";
}

void pretty_print_board() 
{
	using std::cout;
	cout << "\n***********************************\n";
	cout << "Pretty Print board: \n";
	cout << "\033[4m" <<"   a   b   c   d   e   f   g   h  \n" << "\033[24m";
	//cout << "  _______________________________\n";
	cout << "8| ";
	for (int i = 0; i < 64; i++) {
		cout << board[i] << " | ";
		if ((i & 7) == 7 && i > 50) cout << "\033[4m";
		if ((i & 7) == 7 && i < 60) cout << "\n" << 7 - (i / 8) << "| ";
		
	}
	cout << "\b" << "\033[24m" << " \n";
}

