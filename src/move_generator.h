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
	inline static std::array<std::array<u64, MAX_SUBSETS_OF_BISHOP_OCP>, SQUARE_COUNT> bishop_move_lookup_table;//stores move boards for each square obtained by subsets
	inline static std::array<std::array<u64, MAX_SUBSETS_OF_BISHOP_OCP>, SQUARE_COUNT> bishop_subset_lookup_table;//stores subsets for each square obtained by subsets
	inline static std::array<u64, SQUARE_COUNT> bishop_occupancy_mask;

public:
	MoveGenerator();

	Move& operator[](size_t i); //Testing purposes*
	
	void init_magic();	
	u64 generate_magic(const int sq);
	int generate_magic_index(const int sq, const u64 subset);

	u64 get_king_ocp_mask(size_t sq) const;   //Testing purposes
	u64 get_knight_ocp_mask(size_t sq) const; //Testing purposes
	std::array<u64, MAX_SUBSETS_OF_BISHOP_OCP> get_bishop_move(size_t sq) const; //Testing purposes
	std::array<u64, MAX_SUBSETS_OF_BISHOP_OCP> get_bishop_subset(size_t sq) const; //Testing purposes
	u64 get_bishop_magic(size_t sq) const; //Testing purposes
	u64 get_bishop_ocp_mask(size_t sq) const;

	void init_occupancy_mask(std::array<std::array<int,2>,4> deltas);

	size_t pseudo_legal_move_list_index;

	void init_lookup_table(std::array<std::array<int,2>, 4>);
	//TODO: make this function static, it only needs to be called once

	void generate_bishop_moves(const Color color, const Board& board);

	void init_king_occupancy_masks();
	void generate_king_moves(const Color color, const Board& board);
	

	void init_knight_occupancy_masks();
	void generate_knight_moves(const Color color, const Board& board);
};
