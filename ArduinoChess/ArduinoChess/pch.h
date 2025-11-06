//
// pch.h
//

#pragma once

#include "gtest/gtest.h"
#include <vector>
#include <string>
#include <sstream>

constexpr char EMPTY_SQUARE = ' ';
constexpr char BLACK_PAWN = 'p', WHITE_PAWN = 'P';
constexpr char BLACK_ROOK = 'r', WHITE_ROOK = 'R';
constexpr char BLACK_KNIGHT = 'n', WHITE_KNIGHT = 'N';
constexpr char BLACK_BISHOP = 'b', WHITE_BISHOP = 'B';
constexpr char BLACK_QUEEN = 'q', WHITE_QUEEN = 'Q';
constexpr char BLACK_KING = 'k', WHITE_KING = 'K';

constexpr const int WHITE_ROOK_KING_SQUARE = 63 - 8;
constexpr const int WHITE_ROOK_QUEEN_SQUARE = 63 - 0;
constexpr const int BLACK_ROOK_KING_SQUARE = 0;
constexpr const int BLACK_ROOK_QUEEN_SQUARE = 0 + 8;

constexpr const int WHITE_KING_KINGSIDE_SQUARE = 63 - 1;
constexpr const int WHITE_KING_QUEENSIDE_SQUARE = 63 - 5;
constexpr const int BLACK_KING_KINGSIDE_SQUARE = 0 + 2;
constexpr const int BLACK_KING_QUEENSIDE_SQUARE = 0 + 6;

enum class piece_types : char {
	empty = ' ',

	w_pawn = 'P',
	w_rook = 'R',
	w_knight = 'N',
	w_bishop = 'B',
	w_queen = 'Q',
	w_king = 'K',

	b_pawn = 'p',
	b_rook = 'r',
	b_knight = 'n',
	b_bishop = 'b',
	b_queen = 'q',
	b_king = 'k',
};

static std::vector<piece_types> piece_chars = {
	piece_types::empty,

	piece_types::w_pawn,
	piece_types::w_rook,
	piece_types::w_rook,
	piece_types::w_bishop,
	piece_types::w_queen,
	piece_types::w_king,

	piece_types::b_pawn,
	piece_types::b_rook,
	piece_types::b_knight,
	piece_types::b_bishop,
	piece_types::b_queen,
	piece_types::b_king,
};

static std::vector<std::string> piece_strings = {
	"empty",

	"white pawn",
	"white rook",
	"white rook",
	"white bishop",
	"white queen",
	"white king",

	"black pawn",
	"black rook",
	"black knight",
	"black bishop",
	"black queen",
	"black king",
};

/// <summary>
/// Chess board, list of 64 squares with a char for each. Captital letters for white pieces.
/// </summary>
/// ' ' -> empty space
/// 'r' -> black rook,		'R' -> white rook
/// 'n' -> black knight	'N' -> white knight
/// 'b' -> black bishop	'B' -> white bishop
/// 'q' -> black queen	'Q' -> white queen
/// 'k' -> black king		'K' -> white king
/// 'p' -> black pawn		'P' -> white pawn
static char board[64] = 
{
	'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r',
	'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P',
	'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R',
};

static const char starting_board_initializer[64] = {
	'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r',
	'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P',
	'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R',
};
static void init_starting_board() {
	int i = 0;
	for (char c : starting_board_initializer)
		board[i++] = c;
}

static const char empty_board_initializer[64] = {
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
};
static void init_empty_board() {
	int i = 0;
	for (char c : empty_board_initializer)
		board[i++] = c;
}

enum states {
	white,
	white_begin_move,
	white_begin_capture,
	white_begin_enemy_capture,

	white_castling,
	white_castling_kingside_kingdown,
	white_castling_queenside_kingdown,

	white_castling_kingside_kingdown_ROOKUP,
	white_castling_queenside_kingdown_ROOKUP,

	white_castling_kingside_KINGUP_ROOKUP,
	white_castling_queenside_KINGUP_ROOKUP,

	white_castling_kingside_KINGUP_rookdown,
	white_castling_queenside_KINGUP_rookdown,

	black,
	black_begin_move,
	black_begin_capture,
	black_begin_enemy_capture,

	error,
} static state;

struct chess_variables {
	bool black_kingside = true,
		black_queenside = true,
		white_kingside = true,
		white_queenside = true;

	bool en_passant = false;
	int en_passant_square = 0;

	int ply_since_ponr = 0;

	int ply = 0;

	int turn() const { return (ply + 2) / 2; }
	void reset() {
		black_kingside = black_queenside = white_kingside = white_queenside = true;
		en_passant = false;
		en_passant_square = 0;
		ply_since_ponr = 0;
		ply = 0;
	}

} static chess_state;

struct fsm_variables {
	int x = -1;
	int y = -1;
	void reset() {
		x = y = -1;
	}
} static fsm;

static void WhiteCheckCastling(const int pin_number)
{
	if (board[pin_number] == WHITE_KING)
		chess_state.white_kingside = chess_state.white_queenside = false;
	else if (board[pin_number] == WHITE_ROOK)
	{
		chess_state.white_kingside = chess_state.white_kingside && fsm.x == WHITE_ROOK_KING_SQUARE;
		chess_state.white_queenside = chess_state.white_queenside && fsm.x == WHITE_ROOK_QUEEN_SQUARE;
	}
}

static void BlackCheckCastling(const int pin_number)
{
	if (board[pin_number] == BLACK_KING)
	{
		chess_state.black_kingside = chess_state.black_queenside = false;
	}
	else if (board[pin_number] == BLACK_ROOK)
	{
		chess_state.black_kingside = chess_state.black_kingside && fsm.x == BLACK_ROOK_KING_SQUARE;
		chess_state.black_queenside = chess_state.black_queenside && fsm.x == BLACK_ROOK_QUEEN_SQUARE;
	}
}

static void WhiteCheckEnPassant(const int pin_number)
{
	if (board[pin_number] == WHITE_PAWN)
	{
		chess_state.en_passant = pin_number - fsm.x == -16;
		chess_state.en_passant_square = pin_number + 8;
	}
}

static void BlackCheckEnPassant(const int pin_number)
{
	if (board[pin_number] == BLACK_PAWN)
	{
		chess_state.en_passant = pin_number - fsm.x == 16;
		chess_state.en_passant_square = pin_number - 8;
	}
}

static void WhiteHandlePONR(const int pin_number) {
	if (board[pin_number] == WHITE_PAWN) chess_state.ply_since_ponr = 0;
	else chess_state.ply_since_ponr++;
}

static void BlackHandlePONR(const int pin_number) {
	if (board[pin_number] == BLACK_PAWN) chess_state.ply_since_ponr = 0;
	else chess_state.ply_since_ponr++;
}

static void WhiteHandlePromotion(const int pin_number) {
	if (board[pin_number] == WHITE_PAWN && pin_number < 8) board[pin_number] = WHITE_QUEEN;
}

static void BlackHandlePromotion(const int pin_number) {
	if (board[pin_number] == BLACK_PAWN && pin_number > (63 - 8)) board[pin_number] = BLACK_QUEEN;
}

/// <summary>
/// Test if we can begin castling
/// </summary>
/// <param name="s">The current state, expects WHITE or BLACK</param>
/// <param name="pin_number">The current piece to move, expects a king of the states color</param>
/// <returns></returns>
static bool CanCastle(const states s, const int pin_number){
	if (s == white) {
		return (board[pin_number] == WHITE_KING &&
			(chess_state.white_kingside || chess_state.white_queenside));
	}
	else if (s == black) {
		return (board[pin_number] == BLACK_KING &&
			(chess_state.black_kingside || chess_state.black_queenside));
	}
	else return false;
}

static void pin_change(const int pin_number, const bool is_up = false)
{
	if (pin_number < 0 || pin_number > 63) return; // guard against out of bound pins

	const bool is_down = !is_up;
	char piece = board[pin_number];
	bool is_empty_square = (piece == ' ');
	bool is_black_piece = !(!is_empty_square && piece < 'a');
	bool is_white_piece = !(!is_empty_square && piece > 'a');
	
	switch (state)
	{
	case white:
	{
		if (is_down) state = error;
		else if (CanCastle(state, pin_number)) {
			state = white_castling;
		}
		else if (is_white_piece) {
			state = white_begin_move;
			fsm.x = pin_number;
		}
		else {
			state = white_begin_enemy_capture;
			fsm.y = pin_number;
		}
		break;
	}

	case white_begin_move:
	{
		auto is_x_down = is_down && fsm.x == pin_number;
		if (is_x_down) 
		{ state = white; }
		else if (is_down) 
		{
			// SEND MOVE
			state = black;
			board[pin_number] = board[fsm.x];
			board[fsm.x] = EMPTY_SQUARE;
			chess_state.ply++;

			WhiteCheckEnPassant(pin_number);
			WhiteHandlePONR(pin_number);
			WhiteCheckCastling(pin_number);
			WhiteHandlePromotion(pin_number);
		}
		else if (is_black_piece)
		{
			state = white_begin_capture;
			fsm.y = pin_number;
		}
		else { state = error; }
		break;
	}

	case white_begin_capture: 
	{
		bool is_y_down = (is_down && (fsm.y == pin_number));
		if (is_y_down) {
			// SEND CAPTURE
			state = black;
			board[fsm.y] = board[fsm.x];
			board[fsm.x] = EMPTY_SQUARE;
			chess_state.ply_since_ponr = 0;
			chess_state.ply++;

			chess_state.en_passant = false;

			WhiteCheckCastling(pin_number);
			WhiteHandlePromotion(pin_number);
		}
		else state = error;

		break;
	}

	case white_begin_enemy_capture:
	{
		if (is_up && is_white_piece) {
			state = white_begin_capture;
			fsm.x = pin_number;
		}
		else if (is_up) // not white => black or blank 
		{
			state = error;
		}
		else if (pin_number == fsm.y)
		{
			state = white;
		}
		else if (is_down)
		{
			state = error;
		}
		
		break;
	}

	case black:
	{
		if (is_down) state = error;
		else if (is_black_piece) {
			state = black_begin_move;
			fsm.x = pin_number;
		}
		else {
			state = black_begin_enemy_capture;
			fsm.y = pin_number;
		}
		break;
	}

	case black_begin_move:
	{
		auto is_x_down = is_down && fsm.x == pin_number;
		if (is_x_down) { state = black; }
		else if (is_down) {
			// SEND MOVE
			state = white;
			board[pin_number] = board[fsm.x];
			board[fsm.x] = EMPTY_SQUARE;
			chess_state.ply++;

			BlackCheckEnPassant(pin_number);
			BlackHandlePONR(pin_number);
			BlackCheckCastling(pin_number);
			BlackHandlePromotion(pin_number);
		}
		else if (is_white_piece) {
			state = black_begin_capture;
			fsm.y = pin_number;
		}
		else { state = error; }
		break;
	}

	case black_begin_capture:
	{
		bool is_y_down = (is_down && (fsm.y == pin_number));
		if (is_y_down) {
			// SEND CAPTURE
			state = white;
			board[fsm.y] = board[fsm.x];
			board[fsm.x] = EMPTY_SQUARE;
			chess_state.ply_since_ponr = 0;
			chess_state.ply++;

			chess_state.en_passant = false;

			BlackCheckCastling(pin_number);
			BlackHandlePromotion(pin_number);
		}
		else state = error;

		break;
	}

	case black_begin_enemy_capture:
	{
		if (is_up && is_black_piece) {
			state = black_begin_capture;
			fsm.x = pin_number;
		}
		else if (is_up) // not white => black or blank 
		{
			state = error;
		}
		else if (pin_number == fsm.y)
		{
			state = black;
		}
		else if (is_down)
		{
			state = error;
		}

		break;
	}
	
	case white_castling:
	{
		if (is_down) {
			if (pin_number == WHITE_)
		}
		else if (is_white_piece) {

		}
		else if (is_black_piece)
			state = error;
		break;
	}
	case white_castling_kingside_kingdown:
	case white_castling_queenside_kingdown:

	case white_castling_kingside_kingdown_ROOKUP:
	case white_castling_queenside_kingdown_ROOKUP:

	case white_castling_kingside_KINGUP_ROOKUP:
	case white_castling_queenside_KINGUP_ROOKUP:

	case white_castling_kingside_KINGUP_rookdown:
	case white_castling_queenside_KINGUP_rookdown:
	
	default: // error
		break;
	}
}

static std::string pin_to_square(const int pin_number) {
	int rank = (63 - pin_number) >> 3; // remove file
	std::string s = std::string({
		(char)('a' + (pin_number & 7)),
			(char)('0' + rank + 1)
		});
	return s;
}

static std::string print_board()
{
	using std::stringstream;
	stringstream ss; 
	ss << "\nPrint board: \n";
	for (int i = 0; i < 64; i++) {
		ss << board[i] << " ";
		if ((i & 7) == 7) ss << "\n";
	}
	return ss.str();
}

static std::string print_fen_string() {
	using std::stringstream;
	stringstream ss;

	// board
	int blank_counter = 0;
	for (int i = 0; i < 64; i++) {
		if (board[i] == ' ') {
			blank_counter++;
		}
		else if (blank_counter == 0) {
			ss << board[i];
		}
		else {
			ss << blank_counter << board[i];
			blank_counter = 0;
		}

		if (i == 63) {
			if (blank_counter > 0) ss << blank_counter;
		}
		else if ((i & 7) == 7)
		{
			if (blank_counter == 0)
				ss << '/';
			else
				ss << blank_counter << '/';
			blank_counter = 0;
		}
	}

	// who to play
	ss << ((state == white) ? " w " : " b ");

	// castling
	if (chess_state.white_kingside ||
		chess_state.white_queenside ||
		chess_state.black_kingside ||
		chess_state.black_queenside)
	{
		ss << ((chess_state.white_kingside) ? "K" : "")
			<< ((chess_state.white_queenside) ? "Q" : "")
			<< ((chess_state.black_kingside) ? "k" : "")
			<< ((chess_state.black_queenside) ? "q" : "")
			<< " ";
	}
	else
	{
		ss << "- ";
	}

	// en passant
	if (chess_state.en_passant)
		ss << pin_to_square(chess_state.en_passant_square) << " "; // print the square
	else
		ss << "- ";

	// ply's since last capture
	ss << chess_state.ply_since_ponr;

	// move counter
	ss << " " << chess_state.turn();

	return ss.str();
}

static void reset() {
	state = white;
	init_starting_board();
	chess_state.reset();
	fsm.reset();
}

static void clean_state() {
	state = white;
	init_empty_board();
	chess_state.reset();
	fsm.reset();
}
