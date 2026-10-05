#pragma once

#include "types.h"
#include "constants.h"
#include "board.h"
#include "move.h"
#include <utility>

class MoveGenerator {
private:
	std::array<Move, MAX_MOVES_IN_A_TURN> pseudo_legal_move_list;/*I know and feel that this is not the right way of doing it, this is probably going to cause some storage problems.For now it is a TODO but i am aware of the problem.*/

	std::array<u64, SQUARE_COUNT> zero_constraint_king_move_masks;
	std::array<u64, SQUARE_COUNT> zero_constraint_knight_move_masks;

	inline static std::array<u64, SQUARE_COUNT> bishop_magic;
	inline static std::array<u64, SQUARE_COUNT> bishop_occupancy_mask;
	inline static std::array<u64, SQUARE_COUNT> bishop_attack;
	inline static std::array<std::array<u64, MAX_SUBSETS_OF_BISHOP_OCP>, SQUARE_COUNT> bishop_lookup_table;

public:
	MoveGenerator();

	Move& operator[](size_t i); //Testing purposes*
	
	u64 generate_magic(int sq, std::array<std::array<int,2>,4> deltas);	

	void init_bishop_lists(std::array<std::array<int,2>,4> deltas);

	u64 get_king_ocp_mask(size_t sq) const;   //Testing purposes
	u64 get_knight_ocp_mask(size_t sq) const; //Testing purposes
	std::array<u64, MAX_SUBSETS_OF_BISHOP_OCP> get_bishop_subsets(size_t sq) const; //Testing purposes
	u64 get_bishop_magic(size_t sq) const; //Testing purposes

	u64 compute_occupancy_mask(int sq, std::array<std::array<int,2>,4> deltas);

	size_t pseudo_legal_move_list_index;

	u64 compute_sliding_piece_attack(int sq, u64 ocp, std::array<std::array<int,2>,4>);

	void make_bishop_lookup_table();

	void init_king_occupancy_masks();
	void generate_king_moves(const Color color, const Board& board);
	

	void init_knight_occupancy_masks();
	void generate_knight_moves(const Color color, const Board& board);
};
