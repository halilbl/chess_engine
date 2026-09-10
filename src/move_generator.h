#pragma once

#include "types.h"
#include "constants.h"
#include "board.h"
#include "move.h"

class MoveGenerator {
private:
	std::array<Move, MAX_MOVES_IN_A_TURN> pseudo_legal_move_list;

	std::array<u64, SQUARE_COUNT> zero_constraint_king_move_masks;
	std::array<u64, SQUARE_COUNT> zero_constraint_knight_move_masks;

public:
	MoveGenerator();

	size_t pseudo_legal_move_list_index;

	Move& operator[](size_t i);

	void init_king_moves();

	void init_knight_moves();
	void generateKnightMoves(const Color color, const Board& board);
};
