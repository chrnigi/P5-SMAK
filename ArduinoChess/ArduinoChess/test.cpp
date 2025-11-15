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
