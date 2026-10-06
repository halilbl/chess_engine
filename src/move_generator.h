#pragma once

#include "types.h"
#include "constants.h"
#include "board.h"
#include "move.h"
#include <utility>
namespace MoveGenerator{

	/*Removed all static/inline keyords. Because, keyword static means something entirely different outside of a class.Static keyword outside of a class means that the spesific object/variable */

	u64 generate_magic(int sq, std::array<std::array<int,2>,4> deltas);	

	void init_bishop_lists(std::array<std::array<int,2>,4> deltas);

	 u64 get_king_ocp_mask(size_t sq);   //Testing purposes
	 u64 get_knight_ocp_mask(size_t sq); //Testing purposes
	 std::array<u64, MAX_BISHOP_OCP> get_bishop_subsets(size_t sq); //Testing purposes
	 u64 get_bishop_magic(size_t sq) ; //Testing purposes
	 std::array<u64, MAX_BISHOP_OCP> get_bishop_array(size_t sq);

	 u64 compute_occupancy_mask(int sq, std::array<std::array<int,2>,4> deltas);

	 size_t pseudo_legal_move_list_index;

	 u64 compute_sliding_piece_attack(int sq, u64 ocp, std::array<std::array<int,2>,4>);

	 void make_bishop_lookup_table();

	 void init_king_occupancy_masks();
	 void generate_king_moves(const Color color, const Board& board);
	

	 void init_knight_occupancy_masks();
	 void generate_knight_moves(const Color color, const Board& board);
}