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
	clean_state();
	int w = 31;
	board[w] = p_WHITE_PAWN;
	pin_change(w, true);
	EXPECT_EQ(state, states::white_begin_move);
}
TEST(FSM, WhiteTurnEnemy) {
	clean_state();
	int b = 22;
	board[b] = p_BLACK_PAWN;
	pin_change(b, true);
	EXPECT_EQ(state, states::white_begin_enemy_capture);
}


TEST(FSM, UndoWhiteMove) {
	reset();
	pin_change(63 - 8, true);
	EXPECT_EQ(state, states::white_begin_move);
	pin_change(63 - 8, false);
	EXPECT_EQ(state, states::white);
}
TEST(FSM, FinishWhiteMove) {
	reset();
	pin_change(63 - 8, true);
	EXPECT_EQ(state, states::white_begin_move);
	pin_change(63 - 16, false);
	EXPECT_EQ(state, states::black);
}
TEST(FSM, FinishWhiteCapture) {
	reset();
	board[63 - 17] = 'p';
	pin_change(63 - 8, true);
	EXPECT_EQ(state, states::white_begin_move);
	pin_change(63 - 17, true);
	EXPECT_EQ(state, states::white_begin_capture);
	pin_change(63 - 17, false);
	EXPECT_EQ(state, states::black);
}
TEST(FSM, FinishWhiteEnemyCapture) {
	reset();
	board[63 - 17] = 'p';
	pin_change(63 - 17, true);
	EXPECT_EQ(state, states::white_begin_enemy_capture);
	pin_change(63 - 8, true);
	EXPECT_EQ(state, states::white_begin_capture);
	pin_change(63 - 17, false);
	EXPECT_EQ(state, states::black);
}


