#pragma once

#include "types.h"
#include "constants.h"
#include "board.h"
#include "move.h"
#include <utility>
namespace MoveGenerator{
	inline std::array<u64, SQUARE_COUNT> king_occupancy;
	inline std::array<u64, SQUARE_COUNT> knight_occupancy;
	inline std::array<u64, SQUARE_COUNT> bishop_occupancy_mask;

	inline std::array<u64, SQUARE_COUNT> bishop_magic;
	inline std::array<std::array<u64, MAX_BISHOP_OCP>, SQUARE_COUNT> bishop_lookup_table;

	void generate_magic(std::array<std::array<int,2>,4> deltas);	 //removed int sq

	void init_bishop_lists(std::array<std::array<int,2>,4> deltas);

	 u64 get_king_ocp_mask(size_t sq);   //Testing purposes
	 u64 get_knight_ocp_mask(size_t sq); //Testing purposes
	 std::array<u64, MAX_BISHOP_OCP> get_bishop_subsets(size_t sq); //Testing purposes
	 u64 get_bishop_magic(size_t sq) ; //Testing purposes
	 std::array<u64, MAX_BISHOP_OCP> get_bishop_array(size_t sq);

	 u64 compute_occupancy_mask(int sq, std::array<std::array<int,2>,4> deltas);

	 u64 compute_sliding_piece_attack(int sq, u64 ocp, std::array<std::array<int,2>,4>);

	 void make_bishop_lookup_table();

	 void init_king_occupancy_masks();
	 void generate_king_moves(const Color color, const Board& board);
	

	 void init_knight_occupancy_masks();
	 void generate_knight_moves(const Color color, const Board& board);
}