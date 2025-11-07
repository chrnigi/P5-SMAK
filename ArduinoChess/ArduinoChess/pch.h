//
// pch.h
//

#pragma once

#include "gtest/gtest.h"
#include <vector>
#include <string>
#include <sstream>

constexpr char p_EMPTY_SQUARE = ' ';
constexpr char p_BLACK_PAWN = 'p', p_WHITE_PAWN = 'P';
constexpr char p_BLACK_ROOK = 'r', p_WHITE_ROOK = 'R';
constexpr char p_BLACK_KNIGHT = 'n', p_WHITE_KNIGHT = 'N';
constexpr char p_BLACK_BISHOP = 'b', p_WHITE_BISHOP = 'B';
constexpr char p_BLACK_QUEEN = 'q', p_WHITE_QUEEN = 'Q';
constexpr char p_BLACK_KING = 'k', p_WHITE_KING = 'K';

constexpr const int WHITE_KING_STARTINGSQUARE = 63 - 3;
constexpr const int BLACK_KING_STARTINGSQUARE = 0 + 5;

constexpr const int WHITE_ROOK_KINGSIDE_STARTINGSQUARE = 63 - 8;
constexpr const int WHITE_ROOK_QUEENSIDE_STARTINGSQUARE = 63 - 0;
constexpr const int BLACK_ROOK_KINGSIDE_STARTINGSQUARE = 0;
constexpr const int BLACK_ROOK_QUEENSIDE_STARTINGSQUARE = 0 + 8;

constexpr const int WHITE_KING_KINGSIDE_CASTLESQUARE = 63 - 1;
constexpr const int WHITE_KING_QUEENSIDE_CASTLESQUARE = 63 - 5;
constexpr const int BLACK_KING_KINGSIDE_CASTLESQUARE = 0 + 2;
constexpr const int BLACK_KING_QUEENSIDE_CASTLESQUARE = 0 + 6;

constexpr const int WHITE_ROOK_KINGSIDE_CASTLESQUARE = 63 - 2;
constexpr const int WHITE_ROOK_QUEENSIDE_CASTLESQUARE = 63 - 4;
constexpr const int BLACK_ROOK_KINGSIDE_CASTLESQUARE = 0 + 3;
constexpr const int BLACK_ROOK_QUEENSIDE_CASTLESQUARE = 0 + 5;

enum class piece_types : char {
	empty = p_EMPTY_SQUARE,

	w_pawn = p_WHITE_PAWN,
	w_rook = p_WHITE_ROOK,
	w_knight = p_WHITE_KNIGHT,
	w_bishop = p_WHITE_BISHOP,
	w_queen = p_WHITE_QUEEN,
	w_king = p_WHITE_KING,

	b_pawn = p_BLACK_PAWN,
	b_rook = p_BLACK_ROOK,
	b_knight = p_BLACK_KNIGHT,
	b_bishop = p_BLACK_BISHOP,
	b_queen = p_BLACK_QUEEN,
	b_king = p_BLACK_KING,
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
	white_castling_kingside_KINGUP_ROOKUP,
	white_castling_queenside_KINGUP_ROOKUP,
	white_castling_kingside_kingdown_ROOKUP,
	white_castling_queenside_kingdown_ROOKUP,
	white_castling_kingside_KINGUP_rookdown,
	white_castling_queenside_KINGUP_rookdown,

	black,
	black_begin_move,
	black_begin_capture,
	black_begin_enemy_capture,

	black_castling,
	black_castling_kingside_kingdown,
	black_castling_queenside_kingdown,
	black_castling_kingside_KINGUP_ROOKUP,
	black_castling_queenside_KINGUP_ROOKUP,
	black_castling_kingside_kingdown_ROOKUP,
	black_castling_queenside_kingdown_ROOKUP,
	black_castling_kingside_KINGUP_rookdown,
	black_castling_queenside_KINGUP_rookdown,

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
	if (board[pin_number] == p_WHITE_KING)
		chess_state.white_kingside = chess_state.white_queenside = false;
	else if (board[pin_number] == p_WHITE_ROOK)
	{
		chess_state.white_kingside = chess_state.white_kingside && fsm.x == WHITE_ROOK_KINGSIDE_STARTINGSQUARE;
		chess_state.white_queenside = chess_state.white_queenside && fsm.x == WHITE_ROOK_QUEENSIDE_STARTINGSQUARE;
	}
}

static void BlackCheckCastling(const int pin_number)
{
	if (board[pin_number] == p_BLACK_KING)
	{
		chess_state.black_kingside = chess_state.black_queenside = false;
	}
	else if (board[pin_number] == p_BLACK_ROOK)
	{
		chess_state.black_kingside = chess_state.black_kingside && fsm.x == BLACK_ROOK_KINGSIDE_STARTINGSQUARE;
		chess_state.black_queenside = chess_state.black_queenside && fsm.x == BLACK_ROOK_QUEENSIDE_STARTINGSQUARE;
	}
}

static void WhiteCheckEnPassant(const int pin_number)
{
	if (board[pin_number] == p_WHITE_PAWN)
	{
		chess_state.en_passant = pin_number - fsm.x == -16;
		chess_state.en_passant_square = pin_number + 8;
	}
}

static void BlackCheckEnPassant(const int pin_number)
{
	if (board[pin_number] == p_BLACK_PAWN)
	{
		chess_state.en_passant = pin_number - fsm.x == 16;
		chess_state.en_passant_square = pin_number - 8;
	}
}

static void WhiteHandlePONR(const int pin_number) {
	if (board[pin_number] == p_WHITE_PAWN) chess_state.ply_since_ponr = 0;
	else chess_state.ply_since_ponr++;
}

static void BlackHandlePONR(const int pin_number) {
	if (board[pin_number] == p_BLACK_PAWN) chess_state.ply_since_ponr = 0;
	else chess_state.ply_since_ponr++;
}

static void WhiteHandlePromotion(const int pin_number) {
	if (board[pin_number] == p_WHITE_PAWN && pin_number < 8) board[pin_number] = p_WHITE_QUEEN;
}

static void BlackHandlePromotion(const int pin_number) {
	if (board[pin_number] == p_BLACK_PAWN && pin_number > (63 - 8)) board[pin_number] = p_BLACK_QUEEN;
}

/// <summary>
/// Test if we can begin castling
/// </summary>
/// <param name="s">The current state, expects WHITE or BLACK</param>
/// <param name="pin_number">The current piece to move, expects a king of the states color</param>
/// <returns></returns>
static bool CanCastle(const states s, const int pin_number){
	if (s == white) {
		return (board[pin_number] == p_WHITE_KING &&
			(chess_state.white_kingside || chess_state.white_queenside));
	}
	else if (s == black) {
		return (board[pin_number] == p_BLACK_KING &&
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
			board[fsm.x] = p_EMPTY_SQUARE;
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
		bool is_en_passant = (is_down && chess_state.en_passant && chess_state.en_passant_square == pin_number);
		if (is_y_down) {
			// SEND CAPTURE
			state = black;
			board[fsm.y] = board[fsm.x];
			board[fsm.x] = p_EMPTY_SQUARE;
			if (is_en_passant) board[fsm.y + 8] = p_EMPTY_SQUARE;
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
			board[fsm.x] = p_EMPTY_SQUARE;
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
		bool is_en_passant = (is_down && chess_state.en_passant && chess_state.en_passant_square == pin_number);
		if (is_y_down) {
			// SEND CAPTURE
			state = white;
			board[fsm.y] = board[fsm.x];
			board[fsm.x] = p_EMPTY_SQUARE;
			if (is_en_passant) board[fsm.y - 8] = p_EMPTY_SQUARE;
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
	
	case white_castling: // entry for castling, king moved
	{
		if (is_down) {
			if (pin_number == WHITE_KING_KINGSIDE_CASTLESQUARE && chess_state.white_kingside)
				state = white_castling_kingside_kingdown;
			else if (pin_number == WHITE_KING_QUEENSIDE_CASTLESQUARE && chess_state.white_queenside)
				state = white_castling_queenside_kingdown;
			else {
				state = white_begin_move;
				pin_change(pin_number, is_up);
			}
		}
		else if (is_white_piece) {
			if (board[pin_number] == p_WHITE_ROOK) {
				if (pin_number == WHITE_ROOK_KINGSIDE_STARTINGSQUARE && chess_state.white_kingside)
					state = white_castling_kingside_KINGUP_ROOKUP;
				else if (pin_number == WHITE_ROOK_QUEENSIDE_STARTINGSQUARE && chess_state.white_queenside)
					state = white_castling_queenside_KINGUP_ROOKUP;
				else
					state = error;
			}
			else
			{
				state = error;
			}
		}
		else if (is_black_piece)
		{
			state = white_begin_capture;
			fsm.y = pin_number;
		}
		break;
	}
	case white_castling_kingside_kingdown:
	{
		if (is_down && board[pin_number] == p_WHITE_ROOK && pin_number == WHITE_ROOK_KINGSIDE_STARTINGSQUARE)
			state = white_castling_kingside_kingdown_ROOKUP;
		else state = error;
		break;
	}
	case white_castling_queenside_kingdown:
	{
		if (is_down && board[pin_number] == p_WHITE_ROOK && pin_number == WHITE_ROOK_QUEENSIDE_STARTINGSQUARE)
			state = white_castling_queenside_kingdown_ROOKUP;
		else state = error;
		break;
	}

	case white_castling_kingside_kingdown_ROOKUP:
	{
		if (is_down && pin_number == WHITE_ROOK_KINGSIDE_CASTLESQUARE)
		{
			// end castling
			chess_state.white_kingside = chess_state.white_queenside = false;
			chess_state.ply++;
			//chess_state.ply_since_ponr = 0;
			board[WHITE_ROOK_KINGSIDE_STARTINGSQUARE] = p_EMPTY_SQUARE;
			board[WHITE_ROOK_KINGSIDE_CASTLESQUARE] = p_WHITE_ROOK;
			board[WHITE_KING_STARTINGSQUARE] = p_EMPTY_SQUARE;
			board[WHITE_KING_KINGSIDE_CASTLESQUARE] = p_WHITE_KING;
			state = black;
		}
		else state = error;
		break;
	}
	case white_castling_queenside_kingdown_ROOKUP:
	{
		if (is_down && pin_number == WHITE_ROOK_QUEENSIDE_CASTLESQUARE)
		{
			// end castling
			chess_state.white_kingside = chess_state.white_queenside = false;
			chess_state.ply++;
			//chess_state.ply_since_ponr = 0;
			board[WHITE_ROOK_QUEENSIDE_STARTINGSQUARE] = p_EMPTY_SQUARE;
			board[WHITE_ROOK_QUEENSIDE_CASTLESQUARE] = p_WHITE_ROOK;
			board[WHITE_KING_STARTINGSQUARE] = p_EMPTY_SQUARE;
			board[WHITE_KING_QUEENSIDE_CASTLESQUARE] = p_WHITE_KING;
			state = black;
		}
		else state == error;
		break;
	}

	case white_castling_kingside_KINGUP_ROOKUP: 
	{
		if (is_down && pin_number == WHITE_KING_KINGSIDE_CASTLESQUARE)
			state = white_castling_kingside_kingdown_ROOKUP;
		else if (is_down && pin_number == WHITE_ROOK_KINGSIDE_CASTLESQUARE)
			state = white_castling_kingside_KINGUP_rookdown;
		else
			state = error;
		break;
	}
	case white_castling_queenside_KINGUP_ROOKUP:
	{
		if (is_down && pin_number == WHITE_KING_QUEENSIDE_CASTLESQUARE)
			state = white_castling_queenside_kingdown_ROOKUP;
		else if (is_down && pin_number == WHITE_ROOK_QUEENSIDE_CASTLESQUARE)
			state = white_castling_queenside_KINGUP_rookdown;
		else
			state = error;
		break;
	}

	case white_castling_kingside_KINGUP_rookdown:
	{
		if (is_down && pin_number == WHITE_KING_KINGSIDE_CASTLESQUARE) {
			// end castling
			chess_state.white_kingside = chess_state.white_queenside = false;
			chess_state.ply++;
			//chess_state.ply_since_ponr = 0;
			board[WHITE_ROOK_KINGSIDE_STARTINGSQUARE] = p_EMPTY_SQUARE;
			board[WHITE_ROOK_KINGSIDE_CASTLESQUARE] = p_WHITE_ROOK;
			board[WHITE_KING_STARTINGSQUARE] = p_EMPTY_SQUARE;
			board[WHITE_KING_KINGSIDE_CASTLESQUARE] = p_WHITE_KING;
			state = black;
		}
		else state = error;
		break;
	}
	case white_castling_queenside_KINGUP_rookdown:
	{
		if (is_down && pin_number == WHITE_ROOK_QUEENSIDE_CASTLESQUARE)
		{
			// end castling
			chess_state.white_kingside = chess_state.white_queenside = false;
			chess_state.ply++;
			//chess_state.ply_since_ponr = 0;
			board[WHITE_ROOK_QUEENSIDE_STARTINGSQUARE] = p_EMPTY_SQUARE;
			board[WHITE_ROOK_QUEENSIDE_CASTLESQUARE] = p_WHITE_ROOK;
			board[WHITE_KING_STARTINGSQUARE] = p_EMPTY_SQUARE;
			board[WHITE_KING_QUEENSIDE_CASTLESQUARE] = p_WHITE_KING;
			state = black;
		}
		else state == error;
		break;
	}

	case black_castling: // entry for castling, king moved
	{
		if (is_down) {
			if (pin_number == BLACK_KING_KINGSIDE_CASTLESQUARE && chess_state.black_kingside)
				state = black_castling_kingside_kingdown;
			else if (pin_number == BLACK_KING_QUEENSIDE_CASTLESQUARE && chess_state.black_queenside)
				state = black_castling_queenside_kingdown;
			else {
				state = black_begin_move;
				pin_change(pin_number, is_up);
			}
		}
		else if (is_black_piece) {
			if (board[pin_number] == p_BLACK_ROOK) {
				if (pin_number == BLACK_ROOK_KINGSIDE_STARTINGSQUARE && chess_state.black_kingside)
					state = black_castling_kingside_KINGUP_ROOKUP;
				else if (pin_number == BLACK_ROOK_QUEENSIDE_STARTINGSQUARE && chess_state.black_queenside)
					state = black_castling_queenside_KINGUP_ROOKUP;
				else
					state = error;
			}
			else
			{
				state = error;
			}
		}
		else if (is_white_piece)
		{
			state = black_begin_capture;
			fsm.y = pin_number;
		}
		break;
	}
	case black_castling_kingside_kingdown:
	{
		if (is_down && board[pin_number] == p_BLACK_ROOK && pin_number == BLACK_ROOK_KINGSIDE_STARTINGSQUARE)
			state = black_castling_kingside_kingdown_ROOKUP;
		else state = error;
		break;
	}
	case black_castling_queenside_kingdown:
	{
		if (is_down && board[pin_number] == p_BLACK_ROOK && pin_number == BLACK_ROOK_QUEENSIDE_STARTINGSQUARE)
			state = black_castling_queenside_kingdown_ROOKUP;
		else state = error;
		break;
	}

	case black_castling_kingside_kingdown_ROOKUP:
	{
		if (is_down && pin_number == BLACK_ROOK_KINGSIDE_CASTLESQUARE)
		{
			// end castling
			chess_state.black_kingside = chess_state.black_queenside = false;
			chess_state.ply++;
			//chess_state.ply_since_ponr = 0;
			board[BLACK_ROOK_KINGSIDE_STARTINGSQUARE] = p_EMPTY_SQUARE;
			board[BLACK_ROOK_KINGSIDE_CASTLESQUARE] = p_BLACK_ROOK;
			board[BLACK_KING_STARTINGSQUARE] = p_EMPTY_SQUARE;
			board[BLACK_KING_KINGSIDE_CASTLESQUARE] = p_BLACK_KING;
			state = black;
		}
		else state = error;
		break;
	}
	case black_castling_queenside_kingdown_ROOKUP:
	{
		if (is_down && pin_number == BLACK_ROOK_QUEENSIDE_CASTLESQUARE)
		{
			// end castling
			chess_state.black_kingside = chess_state.black_queenside = false;
			chess_state.ply++;
			//chess_state.ply_since_ponr = 0;
			board[BLACK_ROOK_QUEENSIDE_STARTINGSQUARE] = p_EMPTY_SQUARE;
			board[BLACK_ROOK_QUEENSIDE_CASTLESQUARE] = p_BLACK_ROOK;
			board[BLACK_KING_STARTINGSQUARE] = p_EMPTY_SQUARE;
			board[BLACK_KING_QUEENSIDE_CASTLESQUARE] = p_BLACK_KING;
			state = black;
		}
		else state == error;
		break;
	}

	case black_castling_kingside_KINGUP_ROOKUP:
	{
		if (is_down && pin_number == BLACK_KING_KINGSIDE_CASTLESQUARE)
			state = black_castling_kingside_kingdown_ROOKUP;
		else if (is_down && pin_number == BLACK_ROOK_KINGSIDE_CASTLESQUARE)
			state = black_castling_kingside_KINGUP_rookdown;
		else
			state = error;
		break;
	}
	case black_castling_queenside_KINGUP_ROOKUP:
	{
		if (is_down && pin_number == BLACK_KING_QUEENSIDE_CASTLESQUARE)
			state = black_castling_queenside_kingdown_ROOKUP;
		else if (is_down && pin_number == BLACK_ROOK_QUEENSIDE_CASTLESQUARE)
			state = black_castling_queenside_KINGUP_rookdown;
		else
			state = error;
		break;
	}

	case black_castling_kingside_KINGUP_rookdown:
	{
		if (is_down && pin_number == BLACK_KING_KINGSIDE_CASTLESQUARE) {
			// end castling
			chess_state.black_kingside = chess_state.black_queenside = false;
			chess_state.ply++;
			//chess_state.ply_since_ponr = 0;
			board[BLACK_ROOK_KINGSIDE_STARTINGSQUARE] = p_EMPTY_SQUARE;
			board[BLACK_ROOK_KINGSIDE_CASTLESQUARE] = p_BLACK_ROOK;
			board[BLACK_KING_STARTINGSQUARE] = p_EMPTY_SQUARE;
			board[BLACK_KING_KINGSIDE_CASTLESQUARE] = p_BLACK_KING;
			state = black;
		}
		else state = error;
		break;
	}
	case black_castling_queenside_KINGUP_rookdown:
	{
		if (is_down && pin_number == BLACK_ROOK_QUEENSIDE_CASTLESQUARE)
		{
			// end castling
			chess_state.black_kingside = chess_state.black_queenside = false;
			chess_state.ply++;
			//chess_state.ply_since_ponr = 0;
			board[BLACK_ROOK_QUEENSIDE_STARTINGSQUARE] = p_EMPTY_SQUARE;
			board[BLACK_ROOK_QUEENSIDE_CASTLESQUARE] = p_BLACK_ROOK;
			board[BLACK_KING_STARTINGSQUARE] = p_EMPTY_SQUARE;
			board[BLACK_KING_QUEENSIDE_CASTLESQUARE] = p_BLACK_KING;
			state = black;
		}
		else state == error;
		break;
	}
	
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
