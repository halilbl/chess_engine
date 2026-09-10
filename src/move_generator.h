#pragma once

#include "types.h"
#include "constants.h"
#include "board.h"
#include "move.h"

class MoveGenerator {
private:
	Move move_list[MAX_MOVES_IN_A_TURN];

	u64 knight_attack_pos[64];

public:
	MoveGenerator();

	size_t move_list_index;

	Move& operator[](size_t i);

	void init_knight_attacks();
	void generateKnightMoves(const Color color, const Board& board);
};
