//
// pch.h
// The file needs to be named pch.h for gtest' unit testing
// The gtest include and redefinition of std::string = String, should be removed for the MCUs
// The entry points for working with this module on the MCUs is be contained to:
//	sendMove(...)
//	setErrorAndSend()
//

#pragma once

#include <gtest/gtest.h> 
#include <vector>
#include <string>
#include <sstream>

#define String std::string

#pragma region piece_types
constexpr char p_EMPTY_SQUARE = ' ';
constexpr char p_BLACK_PAWN = 'p', p_WHITE_PAWN = 'P';
constexpr char p_BLACK_ROOK = 'r', p_WHITE_ROOK = 'R';
constexpr char p_BLACK_KNIGHT = 'n', p_WHITE_KNIGHT = 'N';
constexpr char p_BLACK_BISHOP = 'b', p_WHITE_BISHOP = 'B';
constexpr char p_BLACK_QUEEN = 'q', p_WHITE_QUEEN = 'Q';
constexpr char p_BLACK_KING = 'k', p_WHITE_KING = 'K';
#pragma endregion

#pragma region Named squares
constexpr const int WHITE_KING_STARTINGSQUARE = 63 - 3;
constexpr const int BLACK_KING_STARTINGSQUARE = 0 + 4;

constexpr const int WHITE_ROOK_KINGSIDE_STARTINGSQUARE = 63 - 0;
constexpr const int WHITE_ROOK_QUEENSIDE_STARTINGSQUARE = 63 - 7;
constexpr const int BLACK_ROOK_KINGSIDE_STARTINGSQUARE = 0 + 7;
constexpr const int BLACK_ROOK_QUEENSIDE_STARTINGSQUARE = 0;

constexpr const int WHITE_KING_KINGSIDE_CASTLESQUARE = 63 - 1;
constexpr const int WHITE_KING_QUEENSIDE_CASTLESQUARE = 63 - 5;
constexpr const int BLACK_KING_KINGSIDE_CASTLESQUARE = 0 + 6;
constexpr const int BLACK_KING_QUEENSIDE_CASTLESQUARE = 0 + 2;

constexpr const int WHITE_ROOK_KINGSIDE_CASTLESQUARE = 63 - 2;
constexpr const int WHITE_ROOK_QUEENSIDE_CASTLESQUARE = 63 - 4;
constexpr const int BLACK_ROOK_KINGSIDE_CASTLESQUARE = 0 + 5;
constexpr const int BLACK_ROOK_QUEENSIDE_CASTLESQUARE = 0 + 3;
#pragma endregion

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
	white_move,
	white_capture,
	white_enemy_capture,

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
	black_move,
	black_capture,
	black_enemy_capture,

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
	bool black_kingside  = true; // Can black castle kingside  i.e king hasn't moved or kingside rook hasn't moved
	bool black_queenside = true; // Can black castle queenside i.e king hasn't moved or queenside rook hasn't moved
	bool white_kingside  = true; // Can white castle kingside  i.e king hasn't moved or kingside rook hasn't moved
	bool white_queenside = true; // Can white castle queenside i.e king hasn't moved or queenside rook hasn't moved

	bool en_passant = false; // Can the next move be an passant capture
	uint8_t en_passant_square = 0; // If en_passant then this represent the square where that can happen

	uint8_t ply_since_ponr = 0; // plys since a capture or pawn move, i.e. the point of no return. Used for 50 move rule

	uint16_t ply = 0; // plys since the game started, i.e. the number of moves made in the game

	uint16_t turn() const { return (ply + 2) / 2; }
	void reset() {
		black_kingside = black_queenside = white_kingside = white_queenside = true;
		en_passant = false;
		en_passant_square = 0;
		ply_since_ponr = 0;
		ply = 0;
	}

} static chess_state;

struct fsm_variables {
	uint8_t x = 0; // Allied piece' FROM square
	uint8_t y = 0; // Enemy piece' FROM square
	void reset() {
		x = y = 0;
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

static bool WhiteHandlePromotion(const int pin_number) {
	if (board[pin_number] == p_WHITE_PAWN && pin_number < 8) {
		board[pin_number] = p_WHITE_QUEEN;
		return true;
	}
	return false;
}

static bool BlackHandlePromotion(const int pin_number) {
	if (board[pin_number] == p_BLACK_PAWN && pin_number > (63 - 8)) {
		board[pin_number] = p_BLACK_QUEEN;
		return true;
	}
	return false;
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

/// <summary>
/// Should be called at the end of termination state before the next turn. Sends data to the cloud
/// </summary>
/// <param name="ply">The current ply</param>
/// <param name="from">The square the piece was moved from</param>
/// <param name="to">The square the piece was moved to</param>
/// <param name="piece">The piece that was moved in char form (see piece_types)</param>
/// <param name="captured">The piece that was captured in char form (see piece_types), ' ' if none</param>
/// <param name="promotion">True if a piece was promoted</param>
/// <param name="en_passant">True if this was en passant move</param>
/// <param name="kingside_castle">True if this was kingside castling</param>
/// <param name="queenside_castle">True if this was queenside castling</param>
static void sendMove(const int ply, const int from, const int to, 
	const char piece, const char captured, 
	const bool promotion = false, const bool en_passant = false, const bool kingside_castle = false, const bool queenside_castle = false) 
{
	// deal with stuff

}

/// <summary>
/// Passes a queenside castling move to sendMove
/// </summary>
/// <param name="ply">The current ply</param>
/// <param name="is_white">Whether this was white's move</param>
static void sendCastleQueenside(const int ply, const int from, const int to, const bool is_white) {
	if (is_white)
		sendMove(ply, WHITE_KING_STARTINGSQUARE, WHITE_KING_QUEENSIDE_CASTLESQUARE, p_WHITE_KING, p_WHITE_ROOK, false, false, false, true);
	else
		sendMove(ply, BLACK_KING_STARTINGSQUARE, BLACK_KING_QUEENSIDE_CASTLESQUARE, p_BLACK_KING, p_BLACK_ROOK, false, false, false, true);
}

/// <summary>
/// Passes a kingside castling move to sendMove
/// </summary>
/// <param name="ply">The current ply</param>
/// <param name="is_white">Whether this was white's move</param>
static void sendCastleKingside(const int ply, const bool is_white) {
	if (is_white)
		sendMove(ply, WHITE_KING_STARTINGSQUARE, WHITE_KING_KINGSIDE_CASTLESQUARE, p_WHITE_KING, p_WHITE_ROOK, false, false, true, false);
	else
		sendMove(ply, BLACK_KING_STARTINGSQUARE, BLACK_KING_KINGSIDE_CASTLESQUARE, p_BLACK_KING, p_BLACK_ROOK, false, false, true, false);
}

/// <summary>
/// Sets the fsm state to error, so the game can't proceed.
/// Then sends an update to the cloud.
/// </summary>
static void setErrorAndSend()
{
	state = states::error;
	// deal with stuff
}

/// <summary>
/// Converts a pin-string to a pin-integer
/// </summary>
/// <param name="str">String in the format of <file char><rank char> such as 'b4'</param>
/// <returns>uint8_t integer between 0 and 63</returns>
static uint8_t pin(String str) {
	char file_char = str.at(0);
	uint8_t file = (file_char - 'a');

	char rank_char = str.at(1);
	uint8_t rank = rank_char - '0';
	uint8_t r = (8 - rank) << 3;

	return (r | file);
}

static void pin_change(const uint8_t pin_number, const bool is_up = false)
{
	if (pin_number > 63) return; // guard against out of bound pins

	const bool is_down = !is_up;
	char piece = board[pin_number];
	bool is_empty_square = (piece == ' ');
	bool is_black_piece = !is_empty_square && !(piece < 'a');
	bool is_white_piece = !is_empty_square && !(piece > 'a');
	
	switch (state)
	{
	case white:
	{
		if (is_down || is_empty_square) setErrorAndSend();
		else if (CanCastle(state, pin_number)) {
			state = white_castling;
		}
		else if (is_white_piece) {
			state = white_move;
			fsm.x = pin_number;
		}
		else {
			state = white_enemy_capture;
			fsm.y = pin_number;
		}
		break;
	}

	case white_move:
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
			bool promotion = WhiteHandlePromotion(pin_number);

			sendMove(chess_state.ply, fsm.x, pin_number, board[pin_number], p_EMPTY_SQUARE, promotion);
		}
		else if (is_black_piece)
		{
			state = white_capture;
			fsm.y = pin_number;
		}
		else setErrorAndSend();

		break;
	}

	case white_capture: 
	{
		bool is_y_down = (is_down && (fsm.y == pin_number));
		bool is_en_passant = (is_down && chess_state.en_passant && chess_state.en_passant_square == pin_number);
		if (is_y_down || is_en_passant) {
			// SEND CAPTURE
			state = black;
			const char captured = board[fsm.y];
			board[pin_number] = board[fsm.x];
			board[fsm.x] = p_EMPTY_SQUARE;
			if (is_en_passant) board[fsm.y] = p_EMPTY_SQUARE;
			chess_state.ply_since_ponr = 0;
			chess_state.ply++;

			chess_state.en_passant = false;

			WhiteCheckCastling(pin_number);
			const bool promotion = WhiteHandlePromotion(pin_number);

			sendMove(chess_state.ply, fsm.x, pin_number, board[pin_number], captured, promotion, is_en_passant);
		}
		else setErrorAndSend();

		break;
	}

	case white_enemy_capture:
	{
		if (is_up && is_white_piece) {
			state = white_capture;
			fsm.x = pin_number;
		}
		else if (is_up) // not white => black or blank 
		{
			setErrorAndSend();
		}
		else if (pin_number == fsm.y)
		{
			state = white;
		}
		else if (is_down)
		{
			setErrorAndSend();
		}
		
		break;
	}

	case black:
	{
		if (is_down || is_empty_square) setErrorAndSend();
		else if (CanCastle(state, pin_number)) {
			state = black_castling;
		}
		else if (is_black_piece) {
			state = black_move;
			fsm.x = pin_number;
		}
		else {
			state = black_enemy_capture;
			fsm.y = pin_number;
		}
		break;
	}

	case black_move:
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
			bool promotion = BlackHandlePromotion(pin_number);

			sendMove(chess_state.ply, fsm.x, pin_number, board[pin_number], p_EMPTY_SQUARE, promotion);
		}
		else if (is_white_piece) {
			state = black_capture;
			fsm.y = pin_number;
		}
		else setErrorAndSend();
		break;
	}

	case black_capture:
	{
		bool is_y_down = (is_down && (fsm.y == pin_number));
		bool is_en_passant = (is_down && chess_state.en_passant && chess_state.en_passant_square == pin_number);
		if (is_y_down || is_en_passant) {
			// SEND CAPTURE
			state = white;
			const char captured = board[fsm.y];
			board[pin_number] = board[fsm.x];
			board[fsm.x] = p_EMPTY_SQUARE;
			if (is_en_passant) board[fsm.y] = p_EMPTY_SQUARE;
			chess_state.ply_since_ponr = 0;
			chess_state.ply++;

			chess_state.en_passant = false;

			BlackCheckCastling(pin_number);
			const bool promotion = BlackHandlePromotion(pin_number);

			sendMove(chess_state.ply, fsm.x, pin_number, board[pin_number], captured, promotion, is_en_passant);
		}
		else setErrorAndSend();

		break;
	}

	case black_enemy_capture:
	{
		if (is_up && is_black_piece) {
			state = black_capture;
			fsm.x = pin_number;
		}
		else if (is_up) // not white => black or blank 
		{
			setErrorAndSend();
		}
		else if (pin_number == fsm.y)
		{
			state = black;
		}
		else if (is_down)
		{
			setErrorAndSend();
		}

		break;
	}
	
	case white_castling: // entry for castling, king moved
	{
		if (is_down) {
			if (pin_number == WHITE_KING_STARTINGSQUARE)
				state = white; // undo move
			else if (pin_number == WHITE_KING_KINGSIDE_CASTLESQUARE && chess_state.white_kingside)
				state = white_castling_kingside_kingdown;
			else if (pin_number == WHITE_KING_QUEENSIDE_CASTLESQUARE && chess_state.white_queenside)
				state = white_castling_queenside_kingdown;
			else {
				state = white_move;
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
					setErrorAndSend();
			}
			else
			{
				setErrorAndSend();
			}
		}
		else if (is_black_piece)
		{
			state = white_capture;
			fsm.y = pin_number;
			fsm.x = WHITE_KING_STARTINGSQUARE;
		}
		break;
	}
	case white_castling_kingside_kingdown:
	{
		if (is_up && board[pin_number] == p_WHITE_ROOK && pin_number == WHITE_ROOK_KINGSIDE_STARTINGSQUARE)
			state = white_castling_kingside_kingdown_ROOKUP;
		else setErrorAndSend();
		break;
	}
	case white_castling_queenside_kingdown:
	{
		if (is_up && board[pin_number] == p_WHITE_ROOK && pin_number == WHITE_ROOK_QUEENSIDE_STARTINGSQUARE)
			state = white_castling_queenside_kingdown_ROOKUP;
		else setErrorAndSend();
		break;
	}

	case white_castling_kingside_kingdown_ROOKUP:
	{
		if (is_down && pin_number == WHITE_ROOK_KINGSIDE_CASTLESQUARE)
		{
			// end castling
			chess_state.white_kingside = chess_state.white_queenside = false;
			chess_state.ply++;
			chess_state.en_passant = false;
			//chess_state.ply_since_ponr = 0;
			board[WHITE_ROOK_KINGSIDE_STARTINGSQUARE] = p_EMPTY_SQUARE;
			board[WHITE_ROOK_KINGSIDE_CASTLESQUARE] = p_WHITE_ROOK;
			board[WHITE_KING_STARTINGSQUARE] = p_EMPTY_SQUARE;
			board[WHITE_KING_KINGSIDE_CASTLESQUARE] = p_WHITE_KING;
			state = black;

			sendCastleKingside(chess_state.ply, true);
		}
		else setErrorAndSend();
		break;
	}
	case white_castling_queenside_kingdown_ROOKUP:
	{
		if (is_down && pin_number == WHITE_ROOK_QUEENSIDE_CASTLESQUARE)
		{
			// end castling
			chess_state.white_kingside = chess_state.white_queenside = false;
			chess_state.ply++;
			chess_state.en_passant = false;
			//chess_state.ply_since_ponr = 0;
			board[WHITE_ROOK_QUEENSIDE_STARTINGSQUARE] = p_EMPTY_SQUARE;
			board[WHITE_ROOK_QUEENSIDE_CASTLESQUARE] = p_WHITE_ROOK;
			board[WHITE_KING_STARTINGSQUARE] = p_EMPTY_SQUARE;
			board[WHITE_KING_QUEENSIDE_CASTLESQUARE] = p_WHITE_KING;
			state = black;

			sendCastleQueenside(chess_state.ply, WHITE_KING_STARTINGSQUARE, WHITE_KING_QUEENSIDE_CASTLESQUARE, true);
		}
		else setErrorAndSend();
		break;
	}

	case white_castling_kingside_KINGUP_ROOKUP: 
	{
		if (is_down && pin_number == WHITE_KING_KINGSIDE_CASTLESQUARE)
			state = white_castling_kingside_kingdown_ROOKUP;
		else if (is_down && pin_number == WHITE_ROOK_KINGSIDE_CASTLESQUARE)
			state = white_castling_kingside_KINGUP_rookdown;
		else
			setErrorAndSend();
		break;
	}
	case white_castling_queenside_KINGUP_ROOKUP:
	{
		if (is_down && pin_number == WHITE_KING_QUEENSIDE_CASTLESQUARE)
			state = white_castling_queenside_kingdown_ROOKUP;
		else if (is_down && pin_number == WHITE_ROOK_QUEENSIDE_CASTLESQUARE)
			state = white_castling_queenside_KINGUP_rookdown;
		else
			setErrorAndSend();
		break;
	}

	case white_castling_kingside_KINGUP_rookdown:
	{
		if (is_down && pin_number == WHITE_KING_KINGSIDE_CASTLESQUARE) {
			// end castling
			chess_state.white_kingside = chess_state.white_queenside = false;
			chess_state.ply++;
			chess_state.en_passant = false;
			//chess_state.ply_since_ponr = 0;
			board[WHITE_ROOK_KINGSIDE_STARTINGSQUARE] = p_EMPTY_SQUARE;
			board[WHITE_ROOK_KINGSIDE_CASTLESQUARE] = p_WHITE_ROOK;
			board[WHITE_KING_STARTINGSQUARE] = p_EMPTY_SQUARE;
			board[WHITE_KING_KINGSIDE_CASTLESQUARE] = p_WHITE_KING;
			state = black;

			sendCastleKingside(chess_state.ply, true);
		}
		else setErrorAndSend();
		break;
	}
	case white_castling_queenside_KINGUP_rookdown:
	{
		if (is_down && pin_number == WHITE_KING_QUEENSIDE_CASTLESQUARE)
		{
			// end castling
			chess_state.white_kingside = chess_state.white_queenside = false;
			chess_state.ply++;
			chess_state.en_passant = false;
			//chess_state.ply_since_ponr = 0;
			board[WHITE_ROOK_QUEENSIDE_STARTINGSQUARE] = p_EMPTY_SQUARE;
			board[WHITE_ROOK_QUEENSIDE_CASTLESQUARE] = p_WHITE_ROOK;
			board[WHITE_KING_STARTINGSQUARE] = p_EMPTY_SQUARE;
			board[WHITE_KING_QUEENSIDE_CASTLESQUARE] = p_WHITE_KING;
			state = black;

			sendCastleQueenside(chess_state.ply, WHITE_KING_STARTINGSQUARE, WHITE_KING_QUEENSIDE_CASTLESQUARE, true);
		}
		else setErrorAndSend();
		break;
	}

	case black_castling: // entry for castling, king moved
	{
		if (is_down) {
			if (pin_number == BLACK_KING_STARTINGSQUARE)
				state = black; // undo move
			else if (pin_number == BLACK_KING_KINGSIDE_CASTLESQUARE && chess_state.black_kingside)
				state = black_castling_kingside_kingdown;
			else if (pin_number == BLACK_KING_QUEENSIDE_CASTLESQUARE && chess_state.black_queenside)
				state = black_castling_queenside_kingdown;
			else {
				state = black_move;
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
					setErrorAndSend();
			}
			else
			{
				setErrorAndSend();
			}
		}
		else if (is_white_piece)
		{
			state = black_capture;
			fsm.y = pin_number;
			fsm.x = BLACK_KING_STARTINGSQUARE;
		}
		break;
	}
	case black_castling_kingside_kingdown:
	{
		if (is_up && board[pin_number] == p_BLACK_ROOK && pin_number == BLACK_ROOK_KINGSIDE_STARTINGSQUARE)
			state = black_castling_kingside_kingdown_ROOKUP;
		else setErrorAndSend();
		break;
	}
	case black_castling_queenside_kingdown:
	{
		if (is_up && board[pin_number] == p_BLACK_ROOK && pin_number == BLACK_ROOK_QUEENSIDE_STARTINGSQUARE)
			state = black_castling_queenside_kingdown_ROOKUP;
		else setErrorAndSend();
		break;
	}

	case black_castling_kingside_kingdown_ROOKUP:
	{
		if (is_down && pin_number == BLACK_ROOK_KINGSIDE_CASTLESQUARE)
		{
			// end castling
			chess_state.black_kingside = chess_state.black_queenside = false;
			chess_state.ply++;
			chess_state.en_passant = false;
			//chess_state.ply_since_ponr = 0;
			board[BLACK_ROOK_KINGSIDE_STARTINGSQUARE] = p_EMPTY_SQUARE;
			board[BLACK_ROOK_KINGSIDE_CASTLESQUARE] = p_BLACK_ROOK;
			board[BLACK_KING_STARTINGSQUARE] = p_EMPTY_SQUARE;
			board[BLACK_KING_KINGSIDE_CASTLESQUARE] = p_BLACK_KING;
			state = white;

			sendCastleKingside(chess_state.ply, false);
		}
		else setErrorAndSend();
		break;
	}
	case black_castling_queenside_kingdown_ROOKUP:
	{
		if (is_down && pin_number == BLACK_ROOK_QUEENSIDE_CASTLESQUARE)
		{
			// end castling
			chess_state.black_kingside = chess_state.black_queenside = false;
			chess_state.ply++;
			chess_state.en_passant = false;
			//chess_state.ply_since_ponr = 0;
			board[BLACK_ROOK_QUEENSIDE_STARTINGSQUARE] = p_EMPTY_SQUARE;
			board[BLACK_ROOK_QUEENSIDE_CASTLESQUARE] = p_BLACK_ROOK;
			board[BLACK_KING_STARTINGSQUARE] = p_EMPTY_SQUARE;
			board[BLACK_KING_QUEENSIDE_CASTLESQUARE] = p_BLACK_KING;
			state = white;

			sendCastleQueenside(chess_state.ply, BLACK_KING_STARTINGSQUARE, BLACK_KING_QUEENSIDE_CASTLESQUARE, false);
		}
		else setErrorAndSend();
		break;
	}

	case black_castling_kingside_KINGUP_ROOKUP:
	{
		if (is_down && pin_number == BLACK_KING_KINGSIDE_CASTLESQUARE)
			state = black_castling_kingside_kingdown_ROOKUP;
		else if (is_down && pin_number == BLACK_ROOK_KINGSIDE_CASTLESQUARE)
			state = black_castling_kingside_KINGUP_rookdown;
		else
			setErrorAndSend();
		break;
	}
	case black_castling_queenside_KINGUP_ROOKUP:
	{
		if (is_down && pin_number == BLACK_KING_QUEENSIDE_CASTLESQUARE)
			state = black_castling_queenside_kingdown_ROOKUP;
		else if (is_down && pin_number == BLACK_ROOK_QUEENSIDE_CASTLESQUARE)
			state = black_castling_queenside_KINGUP_rookdown;
		else
			setErrorAndSend();
		break;
	}

	case black_castling_kingside_KINGUP_rookdown:
	{
		if (is_down && pin_number == BLACK_KING_KINGSIDE_CASTLESQUARE) {
			// end castling
			chess_state.black_kingside = chess_state.black_queenside = false;
			chess_state.ply++;
			chess_state.en_passant = false;
			//chess_state.ply_since_ponr = 0;
			board[BLACK_ROOK_KINGSIDE_STARTINGSQUARE] = p_EMPTY_SQUARE;
			board[BLACK_ROOK_KINGSIDE_CASTLESQUARE] = p_BLACK_ROOK;
			board[BLACK_KING_STARTINGSQUARE] = p_EMPTY_SQUARE;
			board[BLACK_KING_KINGSIDE_CASTLESQUARE] = p_BLACK_KING;
			state = white;

			sendCastleKingside(chess_state.ply, false);
		}
		else setErrorAndSend();
		break;
	}
	case black_castling_queenside_KINGUP_rookdown:
	{
		if (is_down && pin_number == BLACK_KING_QUEENSIDE_CASTLESQUARE)
		{
			// end castling
			chess_state.black_kingside = chess_state.black_queenside = false;
			chess_state.ply++;
			chess_state.en_passant = false;
			//chess_state.ply_since_ponr = 0;
			board[BLACK_ROOK_QUEENSIDE_STARTINGSQUARE] = p_EMPTY_SQUARE;
			board[BLACK_ROOK_QUEENSIDE_CASTLESQUARE] = p_BLACK_ROOK;
			board[BLACK_KING_STARTINGSQUARE] = p_EMPTY_SQUARE;
			board[BLACK_KING_QUEENSIDE_CASTLESQUARE] = p_BLACK_KING;
			state = white;

			sendCastleQueenside(chess_state.ply, BLACK_KING_STARTINGSQUARE, BLACK_KING_QUEENSIDE_CASTLESQUARE, false);
		}
		else setErrorAndSend();
		break;
	}
	case error: return;
	default: // we shouldn't be able to get here, but just in case set state to error
		setErrorAndSend();
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

/// <summary>
/// Reset the state of chess_state and fsm, initializes the board to the starting position
/// </summary>
static void reset() {
	state = white;
	init_starting_board();
	chess_state.reset();
	fsm.reset();
}

/// <summary>
/// Reset the state of chess_state and fsm, initializes the board to an empty position
/// </summary>
static void clean_state() {
	state = white;
	init_empty_board();
	chess_state.reset();
	fsm.reset();
}
