#include "pch.h"
static const bool up = true;
static const bool down = false;

static int pin(std::string s) {
	char file_char = s.at(0);
	int file =  (file_char - 'a');
	
	char rank_char = s.at(1);
	int rank = rank_char - '0';
	int r = (8-rank) << 3;

	return (r | file);
}

TEST(pin, pin_to_string) {
	int p = 18;
	EXPECT_EQ(pin(pin_to_square(p)), p);
	for (int i = 0; i < 63; i++)
		EXPECT_EQ(pin(pin_to_square(i)), i);
}

TEST(state, reset) {
	reset();
	EXPECT_EQ(state, states::white);
	EXPECT_EQ(chess_state.black_kingside, true);
	EXPECT_EQ(chess_state.black_queenside, true);
	EXPECT_EQ(chess_state.white_kingside, true);
	EXPECT_EQ(chess_state.white_queenside, true);
	EXPECT_EQ(chess_state.en_passant, false);
	EXPECT_EQ(chess_state.en_passant_square, 0);
	EXPECT_EQ(chess_state.ply, 0);
	EXPECT_EQ(chess_state.ply_since_ponr, 0);
	for (int i = 0; i < 64; i++) {
		ASSERT_EQ(board[i], starting_board_initializer[i]);
	}
}
TEST(state, clean_state) {
	clean_state();
	EXPECT_EQ(state, states::white);
	EXPECT_EQ(chess_state.black_kingside, true);
	EXPECT_EQ(chess_state.black_queenside, true);
	EXPECT_EQ(chess_state.white_kingside, true);
	EXPECT_EQ(chess_state.white_queenside, true);
	EXPECT_EQ(chess_state.en_passant, false);
	EXPECT_EQ(chess_state.en_passant_square, 0);
	EXPECT_EQ(chess_state.ply, 0);
	EXPECT_EQ(chess_state.ply_since_ponr, 0);
	for (int i = 0; i < 64; i++) {
		ASSERT_EQ(board[i], empty_board_initializer[i]);
	}
}

TEST(FSM_white, WhiteTurn) {
	clean_state();
	int w = 31;
	board[w] = p_WHITE_PAWN;
	pin_change(w, true);
	EXPECT_EQ(state, states::white_begin_move);
}
TEST(FSM_white, WhiteTurnEnemy) {
	clean_state();
	int b = 22;
	board[b] = p_BLACK_PAWN;
	pin_change(b, true);
	EXPECT_EQ(state, states::white_begin_enemy_capture);
}

TEST(FSM_white, UndoWhiteMove) {
	reset();
	pin_change(63 - 8, true);
	EXPECT_EQ(state, states::white_begin_move);
	pin_change(63 - 8, false);
	EXPECT_EQ(state, states::white);
}
TEST(FSM_white, FinishWhiteMove) {
	reset();
	pin_change(63 - 8, true);
	EXPECT_EQ(state, states::white_begin_move);
	pin_change(63 - 16, false);
	EXPECT_EQ(state, states::black);
}

TEST(FSM_white, StartWhiteCapture) {
	reset();
	int target = pin("a3");
	int from = pin("b2");
	board[target] = p_BLACK_PAWN;
	pin_change(from, true);
	pin_change(target, true);
	EXPECT_EQ(state, states::white_begin_capture);
}
TEST(FSM_white, StartWhiteEnemyCapture) {
	reset();
	int target = pin("a3");
	int from = pin("b2");
	board[target] = p_BLACK_PAWN;
	pin_change(target, true);
	EXPECT_EQ(state, states::white_begin_enemy_capture);
	pin_change(from, true);
	EXPECT_EQ(state, states::white_begin_capture);
}
TEST(FSM_white, FinishWhiteCapture) {
	reset();
	board[63 - 17] = 'p';
	pin_change(63 - 8, true);
	EXPECT_EQ(state, states::white_begin_move);
	pin_change(63 - 17, true);
	EXPECT_EQ(state, states::white_begin_capture);
	pin_change(63 - 17, false);
	EXPECT_EQ(state, states::black);
}
TEST(FSM_white, FinishWhiteEnemyCapture) {
	reset();
	board[63 - 17] = 'p';
	pin_change(63 - 17, true);
	EXPECT_EQ(state, states::white_begin_enemy_capture);
	pin_change(63 - 8, true);
	EXPECT_EQ(state, states::white_begin_capture);
	pin_change(63 - 17, false);
	EXPECT_EQ(state, states::black);
}

TEST(FSM_black, black_begin_move) {
	clean_state();
	state = states::black;
	int from = pin("e5");
	board[from] = p_BLACK_PAWN;
	pin_change(from, true);
	EXPECT_EQ(state, states::black_begin_move);
}
TEST(FSM_black, black_begin_enemy_capture) {
	clean_state();
	state = states::black;
	int from = pin("d4");
	board[from] = p_WHITE_PAWN;
	pin_change(from, true);
	EXPECT_EQ(state, states::black_begin_enemy_capture);
}

TEST(FSM_black, UndoBlackMove) {
	clean_state();
	state = states::black;
	int from = pin("e5");
	board[from] = p_BLACK_PAWN;
	pin_change(from, up);
	pin_change(from, down);
	EXPECT_EQ(state, states::black);
}
TEST(FSM_black, FinalizeBlackMove) {
	clean_state();
	state = states::black;
	int from = pin("e5");
	int target = pin("e4");
	board[from] = p_BLACK_PAWN;
	pin_change(from, up);
	pin_change(target, down);
	EXPECT_EQ(state, states::white);
}

TEST(FSM_black, black_begin_capture) {
	reset();
	state = states::black;
	int target = pin("a6");
	int from = pin("b7");
	board[target] = p_WHITE_PAWN;
	pin_change(from, true);
	pin_change(target, true);
	EXPECT_EQ(state, states::black_begin_capture);
}
TEST(FSM_black, StartBlackEnemyCapture) {
	reset();
	state = states::black;
	int target = pin("a6");
	int from = pin("b7");
	board[target] = p_WHITE_PAWN;
	pin_change(target, true);
	EXPECT_EQ(state, states::black_begin_enemy_capture);
	pin_change(from, true);
	EXPECT_EQ(state, states::black_begin_capture);
}

TEST(FSM_black, FinishBlackCapture) {
	reset();
	state = states::black;
	int target = pin("a6");
	int from = pin("b7");
	board[target] = p_WHITE_PAWN;
	pin_change(from, up);
	EXPECT_EQ(state, states::black_begin_move);
	pin_change(target, up);
	EXPECT_EQ(state, states::black_begin_capture);
	pin_change(target, down);
	EXPECT_EQ(state, states::white);
}
TEST(FSM_black, FinishBlackEnemyCapture) {
	reset();
	state = states::black;
	int target = pin("a6");
	int from = pin("b7");
	board[target] = p_WHITE_PAWN;
	pin_change(target, up);
	EXPECT_EQ(state, states::black_begin_enemy_capture);
	pin_change(from, up);
	EXPECT_EQ(state, states::black_begin_capture);
	pin_change(target, down);
	EXPECT_EQ(state, states::white);
}

TEST(FSM_promotion, WhitePromotion) {
	clean_state();
	int target = pin("a8");
	int from = pin("a7");
	board[from] = p_WHITE_PAWN;
	pin_change(from, up);
	pin_change(target, down);
	EXPECT_EQ(state, states::black);
	EXPECT_EQ(board[from], p_EMPTY_SQUARE);
	EXPECT_EQ(board[target], p_WHITE_QUEEN);

	for (int i = 0; i < 8; i++) // run all files
	{
		clean_state();
		int target = pin("a8") + i;
		int from = pin("a7") + i;
		board[from] = p_WHITE_PAWN;
		pin_change(from, up);
		pin_change(target, down);
		EXPECT_EQ(state, states::black);
		EXPECT_EQ(board[from], p_EMPTY_SQUARE);
		EXPECT_EQ(board[target], p_WHITE_QUEEN);
	}
}
TEST(FSM_promotion, BlackPromotion) {
	clean_state();
	state = states::black;
	int target = pin("a1");
	int from = pin("a2");
	board[from] = p_BLACK_PAWN;
	pin_change(from, up);
	pin_change(target, down);
	EXPECT_EQ(state, states::white) << "Expected turn changed from black to white";
	EXPECT_EQ(board[from], p_EMPTY_SQUARE) << "Expected [from] square to be empty";
	EXPECT_EQ(board[target], p_BLACK_QUEEN) << "Expected [target] square to have a queen";

	for (int i = 0; i < 8; i++) // run all files
	{
		clean_state();
		state = states::black;
		int target = pin("a1") + i;
		int from = pin("a2") + i;
		board[from] = p_BLACK_PAWN;
		pin_change(from, up);
		pin_change(target, down);
		EXPECT_EQ(state, states::white);
		EXPECT_EQ(board[from], p_EMPTY_SQUARE);
		EXPECT_EQ(board[target], p_BLACK_QUEEN);
	}
}
TEST(FSM_promotion, WhiteCapturePromotion) {
	clean_state();
	int target = pin("a8");
	int from = pin("b7");
	board[from] = p_WHITE_PAWN;
	board[target] = p_BLACK_PAWN;
	pin_change(from, up);
	pin_change(target, up);
	pin_change(target, down);
	EXPECT_EQ(state, states::black);
	EXPECT_EQ(board[from], p_EMPTY_SQUARE);
	EXPECT_EQ(board[target], p_WHITE_QUEEN);
}
TEST(FSM_promotion, WhiteEnemyCapturePromotion) {
	clean_state();
	int target = pin("a8");
	int from = pin("b7");
	board[from] = p_WHITE_PAWN;
	board[target] = p_BLACK_PAWN;
	pin_change(target, up);
	EXPECT_EQ(state, states::white_begin_enemy_capture);
	pin_change(from, up);
	EXPECT_EQ(state, states::white_begin_capture);
	pin_change(target, down);
	EXPECT_EQ(state, states::black);
	EXPECT_EQ(board[from], p_EMPTY_SQUARE);
	EXPECT_EQ(board[target], p_WHITE_QUEEN);
}
TEST(FSM_promotion, BlackCapturePromotion) {
	clean_state();
	state = states::black;
	int target = pin("a1");
	int from = pin("a2");
	board[from] = p_BLACK_PAWN;
	board[target] = p_WHITE_PAWN;
	pin_change(from, up);
	pin_change(target, up);
	pin_change(target, down);
	EXPECT_EQ(state, states::white) << "Expected turn changed from black to white";
	EXPECT_EQ(board[from], p_EMPTY_SQUARE) << "Expected [from] square to be empty";
	EXPECT_EQ(board[target], p_BLACK_QUEEN) << "Expected [target] square to have a queen";
}
TEST(FSM_promotion, BlackEnemyCapturePromotion) {
	clean_state();
	state = states::black;
	int target = pin("a1");
	int from = pin("a2");
	board[from] = p_BLACK_PAWN;
	board[target] = p_WHITE_PAWN;
	pin_change(target, up);
	EXPECT_EQ(state, states::black_begin_enemy_capture);
	pin_change(from, up);
	EXPECT_EQ(state, states::black_begin_capture);
	pin_change(target, down);
	EXPECT_EQ(state, states::white) << "Expected turn changed from black to white";
	EXPECT_EQ(board[from], p_EMPTY_SQUARE) << "Expected [from] square to be empty";
	EXPECT_EQ(board[target], p_BLACK_QUEEN) << "Expected [target] square to have a queen";
}
TEST(FSM_promotion, White_NotPromotion) {
	clean_state();
	int target = pin("a8");
	int from = pin("a7");
	board[from] = p_WHITE_BISHOP;
	pin_change(from, up);
	pin_change(target, down);
	EXPECT_EQ(state, states::black);
	EXPECT_EQ(board[from], p_EMPTY_SQUARE);
	EXPECT_EQ(board[target], p_WHITE_BISHOP);

	for (char piece : {p_WHITE_BISHOP, p_WHITE_KING, p_WHITE_KNIGHT, p_WHITE_QUEEN, p_WHITE_ROOK}) // run all pieces except pawn
	{
		clean_state();
		chess_state.white_kingside = chess_state.white_queenside = false; // remove castling option
		int target = pin("a8");
		int from = pin("a7");
		board[from] = piece;
		pin_change(from, up);
		pin_change(target, down);
		EXPECT_EQ(state, states::black);
		EXPECT_EQ(board[from], p_EMPTY_SQUARE);
		EXPECT_EQ(board[target], piece);
	}
}

TEST(FSM_en_passant, LongEnPassant) {
	clean_state();
	int from = pin("a2");
	int to = pin("a4");
	int en_passant_expected = pin("a3");
	board[from] = p_WHITE_PAWN;
	pin_change(from, up);
	pin_change(to, down);
	ASSERT_EQ(state, states::black);
	EXPECT_TRUE(chess_state.en_passant);
	ASSERT_EQ(chess_state.en_passant_square, en_passant_expected);

	from = pin("h7");
	to = pin("h5");
	en_passant_expected = pin("h6");
	board[from] = p_BLACK_PAWN;
	pin_change(from, up);
	pin_change(to, down);
	EXPECT_TRUE(chess_state.en_passant);
	ASSERT_EQ(chess_state.en_passant_square, en_passant_expected);

	//not en passant
	from = pin("a4");
	to = pin("a5");
	en_passant_expected = pin("a4");
	pin_change(from, up);
	pin_change(to, down);
	EXPECT_FALSE(chess_state.en_passant);

	from = pin("h5");
	to = pin("h4");
	en_passant_expected = pin("h5");
	pin_change(from, up);
	pin_change(to, down);
	EXPECT_FALSE(chess_state.en_passant);

	//set en passant
	from = pin("g2");
	to = pin("g4");
	en_passant_expected = pin("g3");
	board[from] = p_WHITE_PAWN;
	pin_change(from, up);
	pin_change(to, down);
	ASSERT_EQ(state, states::black);
	ASSERT_TRUE(chess_state.en_passant);
	ASSERT_EQ(chess_state.en_passant_square, en_passant_expected);
	//capture
	from = pin("h4");
	to = pin("g3");
	int expected_empty = pin("g4");
	pin_change(from, up);
	pin_change(expected_empty, up);
	pin_change(to, down);
	ASSERT_EQ(state, states::white);
	ASSERT_EQ(board[from], p_EMPTY_SQUARE);
	ASSERT_EQ(board[to], p_BLACK_PAWN);
	ASSERT_EQ(board[expected_empty], p_EMPTY_SQUARE);


	//setup for en passant, move turn
	state = states::black;
	from = pin("b7");
	to = pin("b5");
	board[from] = p_BLACK_PAWN;
	en_passant_expected = pin("b6");
	pin_change(from, up);
	pin_change(to, down);
	ASSERT_TRUE(chess_state.en_passant);
	ASSERT_EQ(chess_state.en_passant_square, en_passant_expected);
	//capture
	from = pin("a5");
	to = pin("b6");
	expected_empty = pin("b5");
	pin_change(expected_empty, up);
	pin_change(from, up);
	pin_change(to, down);
	ASSERT_EQ(state, states::black);
	ASSERT_EQ(board[from], p_EMPTY_SQUARE);
	ASSERT_EQ(board[to], p_WHITE_PAWN);
	ASSERT_EQ(board[expected_empty], p_EMPTY_SQUARE);
}
TEST(FSM_en_passant, test_all_en_passant_squares) {
	reset();
	int to, from, en_passant;
	ASSERT_EQ(state, states::white);
	for (int i = 0; i < 8; i++) 
	{
		// WHITE
		from = pin("a2") + i;
		to = pin("a4") + i;
		en_passant = pin("a3") + i;
		pin_change(from, up);
		pin_change(to, down);
		ASSERT_EQ(state, states::black);
		EXPECT_TRUE(chess_state.en_passant);
		EXPECT_EQ(chess_state.en_passant_square, en_passant);

		// BLACK
		from = pin("a7") + i;
		to = pin("a5") + i;
		en_passant = pin("a6") + i;
		pin_change(from, up);
		pin_change(to, down);
		ASSERT_EQ(state, states::white);
		EXPECT_TRUE(chess_state.en_passant);
		EXPECT_EQ(chess_state.en_passant_square, en_passant);
	}
	ASSERT_EQ(chess_state.ply, 16);
}

static const int countPieces() {
	int counter = 0;
	for (char c : board) 
		if (c != p_EMPTY_SQUARE) counter += 1;
	return counter;
}
TEST(FSM_castling, black_kingside_castle_1__krkr) {
	clean_state();
	state = states::black;
	board[BLACK_ROOK_KINGSIDE_STARTINGSQUARE] = p_BLACK_ROOK;
	board[BLACK_KING_STARTINGSQUARE] = p_BLACK_KING;
	ASSERT_TRUE(chess_state.black_kingside);
	ASSERT_EQ(countPieces(), 2) << "There should only be 2 pieces on the board";
	pin_change(pin("e8"), up);
	ASSERT_EQ(state, states::black_castling);
	pin_change(pin("h8"), up);
	ASSERT_EQ(state, states::black_castling_kingside_KINGUP_ROOKUP);
	pin_change(pin("g8"), down);
	ASSERT_EQ(state, states::black_castling_kingside_kingdown_ROOKUP);
	pin_change(pin("f8"), down);
	ASSERT_EQ(state, states::white);
	ASSERT_FALSE(chess_state.en_passant);
	ASSERT_FALSE(chess_state.black_kingside);
	ASSERT_FALSE(chess_state.black_queenside);
	ASSERT_EQ(board[pin("g8")], p_BLACK_KING);
	ASSERT_EQ(board[pin("f8")], p_BLACK_ROOK);
	ASSERT_EQ(countPieces(), 2) << "There should only be 2 pieces on the board";
}
TEST(FSM_castling, black_kingside_castle_2__krrk) {
	clean_state();
	state = states::black;
	board[BLACK_ROOK_KINGSIDE_STARTINGSQUARE] = p_BLACK_ROOK;
	board[BLACK_KING_STARTINGSQUARE] = p_BLACK_KING;
	pin_change(pin("e8"), up);
	ASSERT_EQ(state, states::black_castling);
	pin_change(pin("h8"), up);
	ASSERT_EQ(state, states::black_castling_kingside_KINGUP_ROOKUP);
	pin_change(pin("f8"), down);
	ASSERT_EQ(state, states::black_castling_kingside_KINGUP_rookdown);
	pin_change(pin("g8"), down);
	ASSERT_EQ(state, states::white);
	ASSERT_FALSE(chess_state.en_passant);
	ASSERT_FALSE(chess_state.black_kingside);
	ASSERT_FALSE(chess_state.black_queenside);
	ASSERT_EQ(board[pin("g8")], p_BLACK_KING);
	ASSERT_EQ(board[pin("f8")], p_BLACK_ROOK);
	ASSERT_EQ(countPieces(), 2) << "There should only be 2 pieces on the board";
}
TEST(FSM_castling, black_kingside_castle_3__kkrr) {
	clean_state();
	state = states::black;
	board[BLACK_ROOK_KINGSIDE_STARTINGSQUARE] = p_BLACK_ROOK;
	board[BLACK_KING_STARTINGSQUARE] = p_BLACK_KING;
	pin_change(pin("e8"), up);
	ASSERT_EQ(state, states::black_castling);
	pin_change(pin("g8"), down);
	ASSERT_EQ(state, states::black_castling_kingside_kingdown);
	pin_change(pin("h8"), up);
	ASSERT_EQ(state, states::black_castling_kingside_kingdown_ROOKUP);
	pin_change(pin("f8"), down);
	ASSERT_EQ(state, states::white);
	ASSERT_FALSE(chess_state.en_passant);
	ASSERT_FALSE(chess_state.black_kingside);
	ASSERT_FALSE(chess_state.black_queenside);
	ASSERT_EQ(board[pin("g8")], p_BLACK_KING);
	ASSERT_EQ(board[pin("f8")], p_BLACK_ROOK);
	ASSERT_EQ(countPieces(), 2) << "There should only be 2 pieces on the board";
}
TEST(FSM_castling, black_queenside_castle_1__krkr) {
	clean_state();
	state = states::black;
	board[BLACK_ROOK_QUEENSIDE_STARTINGSQUARE] = p_BLACK_ROOK;
	board[BLACK_KING_STARTINGSQUARE] = p_BLACK_KING;
	ASSERT_TRUE(chess_state.black_queenside);
	ASSERT_EQ(countPieces(), 2) << "There should only be 2 pieces on the board";
	pin_change(pin("e8"), up);
	ASSERT_EQ(state, states::black_castling);
	pin_change(pin("a8"), up);
	ASSERT_EQ(state, states::black_castling_queenside_KINGUP_ROOKUP);
	pin_change(pin("c8"), down);
	ASSERT_EQ(state, states::black_castling_queenside_kingdown_ROOKUP);
	pin_change(pin("d8"), down);
	ASSERT_EQ(state, states::white);
	ASSERT_FALSE(chess_state.en_passant);
	ASSERT_FALSE(chess_state.black_kingside);
	ASSERT_FALSE(chess_state.black_queenside);
	ASSERT_EQ(board[pin("c8")], p_BLACK_KING);
	ASSERT_EQ(board[pin("d8")], p_BLACK_ROOK);
	ASSERT_EQ(countPieces(), 2) << "There should only be 2 pieces on the board";
}
TEST(FSM_castling, black_queenside_castle_2__krrk) {
	clean_state();
	state = states::black;
	board[BLACK_ROOK_QUEENSIDE_STARTINGSQUARE] = p_BLACK_ROOK;
	board[BLACK_KING_STARTINGSQUARE] = p_BLACK_KING;
	pin_change(pin("e8"), up);
	ASSERT_EQ(state, states::black_castling);
	pin_change(pin("a8"), up);
	ASSERT_EQ(state, states::black_castling_queenside_KINGUP_ROOKUP);
	pin_change(pin("d8"), down);
	ASSERT_EQ(state, states::black_castling_queenside_KINGUP_rookdown);
	pin_change(pin("c8"), down);
	ASSERT_EQ(state, states::white);
	ASSERT_FALSE(chess_state.en_passant);
	ASSERT_FALSE(chess_state.black_kingside);
	ASSERT_FALSE(chess_state.black_queenside);
	ASSERT_EQ(board[pin("c8")], p_BLACK_KING);
	ASSERT_EQ(board[pin("d8")], p_BLACK_ROOK);
	ASSERT_EQ(countPieces(), 2) << "There should only be 2 pieces on the board";
}
TEST(FSM_castling, black_queenside_castle_3__kkrr) {
	clean_state();
	state = states::black;
	board[BLACK_ROOK_QUEENSIDE_STARTINGSQUARE] = p_BLACK_ROOK;
	board[BLACK_KING_STARTINGSQUARE] = p_BLACK_KING;
	pin_change(pin("e8"), up);
	ASSERT_EQ(state, states::black_castling);
	pin_change(pin("c8"), down);
	ASSERT_EQ(state, states::black_castling_queenside_kingdown);
	pin_change(pin("a8"), up);
	ASSERT_EQ(state, states::black_castling_queenside_kingdown_ROOKUP);
	pin_change(pin("d8"), down);
	ASSERT_EQ(state, states::white);
	ASSERT_FALSE(chess_state.en_passant);
	ASSERT_FALSE(chess_state.black_kingside);
	ASSERT_FALSE(chess_state.black_queenside);
	ASSERT_EQ(board[pin("c8")], p_BLACK_KING);
	ASSERT_EQ(board[pin("d8")], p_BLACK_ROOK);
	ASSERT_EQ(countPieces(), 2) << "There should only be 2 pieces on the board";
}

// white
TEST(FSM_castling, white_kingside_castle_1__krkr) {
	clean_state();
	board[WHITE_ROOK_KINGSIDE_STARTINGSQUARE] = p_WHITE_ROOK;
	board[WHITE_KING_STARTINGSQUARE] = p_WHITE_KING;
	ASSERT_TRUE(chess_state.white_kingside);
	ASSERT_EQ(countPieces(), 2) << "There should only be 2 pieces on the board";
	pin_change(pin("e1"), up);
	ASSERT_EQ(state, states::white_castling);
	pin_change(pin("h1"), up);
	ASSERT_EQ(state, states::white_castling_kingside_KINGUP_ROOKUP);
	pin_change(pin("g1"), down);
	ASSERT_EQ(state, states::white_castling_kingside_kingdown_ROOKUP);
	pin_change(pin("f1"), down);
	ASSERT_EQ(state, states::black);
	ASSERT_FALSE(chess_state.en_passant);
	ASSERT_FALSE(chess_state.white_kingside);
	ASSERT_FALSE(chess_state.white_queenside);
	ASSERT_EQ(board[pin("g1")], p_WHITE_KING);
	ASSERT_EQ(board[pin("f1")], p_WHITE_ROOK);
	ASSERT_EQ(countPieces(), 2) << "There should only be 2 pieces on the board";
}
TEST(FSM_castling, white_kingside_castle_2__krrk) {
	clean_state();
	board[WHITE_ROOK_KINGSIDE_STARTINGSQUARE] = p_WHITE_ROOK;
	board[WHITE_KING_STARTINGSQUARE] = p_WHITE_KING;
	pin_change(pin("e1"), up);
	ASSERT_EQ(state, states::white_castling);
	pin_change(pin("h1"), up);
	ASSERT_EQ(state, states::white_castling_kingside_KINGUP_ROOKUP);
	pin_change(pin("f1"), down);
	ASSERT_EQ(state, states::white_castling_kingside_KINGUP_rookdown);
	pin_change(pin("g1"), down);
	ASSERT_EQ(state, states::black);
	ASSERT_FALSE(chess_state.en_passant);
	ASSERT_FALSE(chess_state.white_kingside);
	ASSERT_FALSE(chess_state.white_queenside);
	ASSERT_EQ(board[pin("g1")], p_WHITE_KING);
	ASSERT_EQ(board[pin("f1")], p_WHITE_ROOK);
	ASSERT_EQ(countPieces(), 2) << "There should only be 2 pieces on the board";
}
TEST(FSM_castling, white_kingside_castle_3__kkrr) {
	clean_state();
	board[WHITE_ROOK_KINGSIDE_STARTINGSQUARE] = p_WHITE_ROOK;
	board[WHITE_KING_STARTINGSQUARE] = p_WHITE_KING;
	pin_change(pin("e1"), up);
	ASSERT_EQ(state, states::white_castling);
	pin_change(pin("g1"), down);
	ASSERT_EQ(state, states::white_castling_kingside_kingdown);
	pin_change(pin("h1"), up);
	ASSERT_EQ(state, states::white_castling_kingside_kingdown_ROOKUP);
	pin_change(pin("f1"), down);
	ASSERT_EQ(state, states::black);
	ASSERT_FALSE(chess_state.en_passant);
	ASSERT_FALSE(chess_state.white_kingside);
	ASSERT_FALSE(chess_state.white_queenside);
	ASSERT_EQ(board[pin("g1")], p_WHITE_KING);
	ASSERT_EQ(board[pin("f1")], p_WHITE_ROOK);
	ASSERT_EQ(countPieces(), 2) << "There should only be 2 pieces on the board";
}
TEST(FSM_castling, white_queenside_castle_1__krkr) {
	clean_state();
	board[WHITE_ROOK_QUEENSIDE_STARTINGSQUARE] = p_WHITE_ROOK;
	board[WHITE_KING_STARTINGSQUARE] = p_WHITE_KING;
	ASSERT_TRUE(chess_state.white_queenside);
	ASSERT_EQ(countPieces(), 2) << "There should only be 2 pieces on the board";
	pin_change(pin("e1"), up);
	ASSERT_EQ(state, states::white_castling);
	pin_change(pin("a1"), up);
	ASSERT_EQ(state, states::white_castling_queenside_KINGUP_ROOKUP);
	pin_change(pin("c1"), down);
	ASSERT_EQ(state, states::white_castling_queenside_kingdown_ROOKUP);
	pin_change(pin("d1"), down);
	ASSERT_EQ(state, states::black);
	ASSERT_FALSE(chess_state.en_passant);
	ASSERT_FALSE(chess_state.white_kingside);
	ASSERT_FALSE(chess_state.white_queenside);
	ASSERT_EQ(board[pin("c1")], p_WHITE_KING);
	ASSERT_EQ(board[pin("d1")], p_WHITE_ROOK);
	ASSERT_EQ(countPieces(), 2) << "There should only be 2 pieces on the board";
}
TEST(FSM_castling, white_queenside_castle_2__krrk) {
	clean_state();
	board[WHITE_ROOK_QUEENSIDE_STARTINGSQUARE] = p_WHITE_ROOK;
	board[WHITE_KING_STARTINGSQUARE] = p_WHITE_KING;
	pin_change(pin("e1"), up);
	ASSERT_EQ(state, states::white_castling);
	pin_change(pin("a1"), up);
	ASSERT_EQ(state, states::white_castling_queenside_KINGUP_ROOKUP);
	pin_change(pin("d1"), down);
	ASSERT_EQ(state, states::white_castling_queenside_KINGUP_rookdown);
	pin_change(pin("c1"), down);
	ASSERT_EQ(state, states::black);
	ASSERT_FALSE(chess_state.en_passant);
	ASSERT_FALSE(chess_state.white_kingside);
	ASSERT_FALSE(chess_state.white_queenside);
	ASSERT_EQ(board[pin("c1")], p_WHITE_KING);
	ASSERT_EQ(board[pin("d1")], p_WHITE_ROOK);
	ASSERT_EQ(countPieces(), 2) << "There should only be 2 pieces on the board";
}
TEST(FSM_castling, white_queenside_castle_3__kkrr) {
	clean_state();
	board[WHITE_ROOK_QUEENSIDE_STARTINGSQUARE] = p_WHITE_ROOK;
	board[WHITE_KING_STARTINGSQUARE] = p_WHITE_KING;
	pin_change(pin("e1"), up);
	ASSERT_EQ(state, states::white_castling);
	pin_change(pin("c1"), down);
	ASSERT_EQ(state, states::white_castling_queenside_kingdown);
	pin_change(pin("a1"), up);
	ASSERT_EQ(state, states::white_castling_queenside_kingdown_ROOKUP);
	pin_change(pin("d1"), down);
	ASSERT_EQ(state, states::black);
	ASSERT_FALSE(chess_state.en_passant);
	ASSERT_FALSE(chess_state.white_kingside);
	ASSERT_FALSE(chess_state.white_queenside);
	ASSERT_EQ(board[pin("c1")], p_WHITE_KING);
	ASSERT_EQ(board[pin("d1")], p_WHITE_ROOK);
	ASSERT_EQ(countPieces(), 2) << "There should only be 2 pieces on the board";
}

TEST(FSM_castling, cancel_into_move) {
	clean_state();
	board[WHITE_ROOK_QUEENSIDE_STARTINGSQUARE] = p_WHITE_ROOK;
	board[WHITE_KING_STARTINGSQUARE] = p_WHITE_KING;
	pin_change(pin("e1"), up);
	ASSERT_EQ(state, states::white_castling);
	pin_change(pin("e2"), down);
	ASSERT_EQ(state, states::black) << "Should move from white castling to blacks turn since the king performed a move";

	clean_state();
	state = states::black;
	board[BLACK_ROOK_QUEENSIDE_STARTINGSQUARE] = p_BLACK_ROOK;
	board[BLACK_KING_STARTINGSQUARE] = p_BLACK_KING;
	pin_change(pin("e8"), up);
	ASSERT_EQ(state, states::black_castling);
	pin_change(pin("e7"), down);
	ASSERT_EQ(state, states::white) << "Should move from black castling to whites turn since the king performed a move";
}
TEST(FSM_castling, cancel_into_capture) {
	clean_state();
	board[pin("e2")] = p_BLACK_KNIGHT;
	board[WHITE_KING_STARTINGSQUARE] = p_WHITE_KING;
	pin_change(pin("e1"), up);
	ASSERT_EQ(state, states::white_castling);
	pin_change(pin("e2"), up);
	ASSERT_EQ(state, states::white_begin_capture);
	pin_change(pin("e2"), down);
	ASSERT_EQ(state, states::black);
	ASSERT_EQ(board[pin("e1")], p_EMPTY_SQUARE);
	ASSERT_EQ(board[pin("e2")], p_WHITE_KING);
	ASSERT_EQ(countPieces(), 1);

	clean_state();
	state = states::black;
	board[pin("e7")] = p_WHITE_KNIGHT;
	board[BLACK_KING_STARTINGSQUARE] = p_BLACK_KING;
	pin_change(pin("e8"), up);
	ASSERT_EQ(state, states::black_castling);
	pin_change(pin("e7"), up);
	ASSERT_EQ(state, states::black_begin_capture);
	pin_change(pin("e7"), down);
	ASSERT_EQ(state, states::white);
	ASSERT_EQ(board[pin("e8")], p_EMPTY_SQUARE);
	ASSERT_EQ(board[pin("e7")], p_BLACK_KING);
	ASSERT_EQ(countPieces(), 1);
}

TEST(FSM_castling, cancel_into_undo) {
	clean_state();
	board[WHITE_KING_STARTINGSQUARE] = p_WHITE_KING;
	pin_change(pin("e1"), up);
	ASSERT_EQ(state, states::white_castling);
	pin_change(pin("e1"), down);
	EXPECT_EQ(state, states::white);

	clean_state();
	state = states::black;
	board[BLACK_KING_STARTINGSQUARE] = p_BLACK_KING;
	pin_change(pin("e8"), up);
	ASSERT_EQ(state, states::black_castling);
	pin_change(pin("e8"), down);
	EXPECT_EQ(state, states::black);
}
