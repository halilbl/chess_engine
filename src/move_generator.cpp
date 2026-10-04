#include "move_generator.h"

#include "bitboard_utils.h"

#include <bit>
#include <iostream>
#include <utility>

MoveGenerator::MoveGenerator() {
	pseudo_legal_move_list_index = 0;
}

Move& MoveGenerator::operator[](size_t i) { //Testing purposes
	return pseudo_legal_move_list[i];
}

u64 MoveGenerator::get_king_ocp_mask(size_t sq) const { //Testing purposes
	return zero_constraint_king_move_masks[sq];
}

u64 MoveGenerator::get_knight_ocp_mask(size_t sq) const { //Testing purposes
	return zero_constraint_knight_move_masks[sq];
}

std::array<u64, MAX_SUBSETS_OF_BISHOP_OCP> MoveGenerator::get_bishop_move(size_t sq) const { //Testing purposes
	return bishop_move_lookup_table[sq];
}

std::array<u64, MAX_SUBSETS_OF_BISHOP_OCP> MoveGenerator::get_bishop_subset(size_t sq) const { //Testing purposes
	return bishop_subset_lookup_table[sq];
}

u64 MoveGenerator::get_bishop_magic(size_t sq) const { //Testing purposes
	return bishop_magic[sq];
}

u64 MoveGenerator::get_bishop_ocp_mask(size_t sq) const { //Testing purposes
	return bishop_occupancy_mask[sq];
}

void MoveGenerator::init_occupancy_mask(std::array<std::array<int,2>,4> deltas) { //occupancy bitboard without blockers

	for(int sq = 0; sq < SQUARE_COUNT;sq++){
		int r = sq / 8;
		int f = sq % 8;

		u64 mask = 0ULL;
		for (auto& d: deltas) { //genuine for all sliding pieces via deltas parameter
			int dr = d[0];
			int df = d[1];

			int nr = r + dr;
			int nf = f + df;

			while (true) {
				int next_nr = nr + dr;
				int next_nf = nf + df;

				bool is_next_in_range = (next_nr >= 0 && next_nr < RANK_COUNT && next_nf >= 0 && next_nf < FILE_COUNT);
				
				if(!is_next_in_range) break; //omit edge squares, trade off is storing less data but having to make more operations in the future for the edge square

				int new_sq = nr * 8 + nf;

				mask |= 1ULL << new_sq;

				nr = next_nr;
				nf = next_nf;
			}
		}

		bishop_occupancy_mask[sq] = mask;
	}

	
}

u64 MoveGenerator::generate_magic(const int sq){ //brute force alogrithm that returns a magic bitboard that helps to generate collision free indexes
	u64 magic = bbu::randu64() & bbu::randu64() & bbu::randu64();
	u64 ocp_mask = bishop_occupancy_mask[sq];

	while(std::popcount((magic * ocp_mask) & 0xFF00000000000000ULL) < 6){ //if there are not enough bits to shift then compute another magic bitboard
		magic = bbu::randu64() & bbu::randu64() & bbu::randu64();
	}

	int relevant_bits = std::popcount(ocp_mask);
	int subset_count = 1ULL << relevant_bits; //2^relevant_bits

	std::array<bool, MAX_SUBSETS_OF_BISHOP_OCP> state_table{}; //1 if the slot is full 0 is empty

	u64 subset = 0ULL;
	for(int i = 0; i < subset_count;i++){
		int index = (magic * subset) >> (64 - relevant_bits); //Can't call generate_magic_index() here because that function requires bishop_magic to be initialized. We haven't initialized it yet and for it to be initialized it needs to call generate_magic() function. Too many dependency problems get fixed by explicitly implementing the bitwise operation.

		if(state_table[index] == 1) {//if collision -> throw current magic away and test a new one
			magic = bbu::randu64() & bbu::randu64() & bbu::randu64();

			while(std::popcount((magic * ocp_mask) & 0xFF00000000000000ULL) < 6){
				magic = bbu::randu64() & bbu::randu64() & bbu::randu64();
			}

			i = -1; //start the loop all over again with the new magic

			state_table.fill(0);//take array to initial state

			continue;
		}

		state_table[index] = 1;
		subset = (subset - ocp_mask) & ocp_mask;//ripple-carry
	}

	return magic; //TODO: research universal hashing for a more optimized way of brute force hash key computation
}

void MoveGenerator::init_magic(){//initializes bishop_magic array
	for(int sq = 0; sq < SQUARE_COUNT; sq++){
		u64 magic = generate_magic(sq);

		bishop_magic[sq] = magic;
	}	
}

int MoveGenerator::generate_magic_index(const int sq, u64 subset){ 
	u64 magic = bishop_magic[sq];
	u64 ocp_mask = bishop_occupancy_mask[sq];

	int shift = 64 - std::popcount(ocp_mask);

	int index = (magic * subset) >> shift;

	return index;
}

void MoveGenerator::init_lookup_table(std::array<std::array<int,2>,4> deltas) { //attack bitboard with blockers
	//compute subset->compute move board-> map subset to move list with magic numbers

	for(int sq = 0; sq < SQUARE_COUNT;sq++){
		int r = sq / 8;
		int f = sq % 8;

		u64 ocp_mask = bishop_occupancy_mask[sq];
		int relevant_bits = std::popcount(ocp_mask);  //returns the amount of bits that are 1
		int subset_count = 1ULL  << relevant_bits; //2^relevant_bits

		u64 subset = 0ULL;

		for(int counter = 0; counter < subset_count; counter++){
			u64 move_board = 0ULL; 

			for(auto& d: deltas){
				int dr = d[0];
				int df = d[1];

				int nr = r;
				int nf = f;
				while(1){
					nr += dr;
					nf += df;

					if(nr < 0 || nr >= RANK_COUNT || nf < 0 || nf >= FILE_COUNT){//out of bonds
						
						break;
					}

					int new_sq = nr * 8 + nf;

					if((subset & (1ULL << new_sq))) {
						move_board |= 1ULL << new_sq;

						break;
					} //doesn't matter the color we take the bit, proper check will be done in move generation phase

					move_board |= 1ULL << new_sq; 
				}
			}
			//magic bitboard and index gen

			int index = generate_magic_index(sq, subset);//index already collision free

			//write data to tables

			bishop_move_lookup_table[sq][index] = move_board;

			bishop_subset_lookup_table[sq][index] = subset;
			subset = (subset - ocp_mask) & ocp_mask;
		}
	}
}

void MoveGenerator::generate_bishop_moves(const Color color, const Board& board){
	//init methods cant be called here since they need to be called only once
	//faced chicken egg problem multiple times amk

	u64 bishop = board[color] & board[Piece::BISHOP]; //TODO:these Move class operators cost almost as a function call overhead from another file, will look into this in the future

	while(bishop){
		int sq = std::countr_zero(bishop);

		u64 ocp_mask = bishop_occupancy_mask[sq];
		u64 subset = ocp_mask & board.board_bitboard;

		int index = generate_magic_index(sq, subset);

		u64 move_board = bishop_move_lookup_table[sq][index]; //all the methods above got executed once to be able to do this in O(1) time

		while(move_board){
			int from = sq;
			int to = std::countr_zero(move_board);

			//writing moves to move list

			if(board[color] & (1ULL << to)) {
				move_board &= move_board - 1;
				continue;
			} //if the piece is the same color then don't include

			pseudo_legal_move_list[pseudo_legal_move_list_index].from = from; 
			pseudo_legal_move_list[pseudo_legal_move_list_index++].to = to; 

			move_board &= move_board - 1;
		}

		bishop &= bishop - 1;
	}

}

void MoveGenerator::init_king_occupancy_masks() { //comptues all possible pseudo-legal moves for king
	for (int sq = 0; sq < 64; sq++) {
		int r = sq / 8;
		int f = sq % 8;

		u64 attacks = 0ULL;
		for (auto& d : KING_DELTAS) {
			int nr = r + d[0];
			int nf = f + d[1];

			if (nr >= RANK_COUNT || nr < 0 || nf >= FILE_COUNT || nf < 0) continue;

			int sq_new = (nr * 8) + nf;

			attacks |= 1ULL << sq_new;
		}
		zero_constraint_king_move_masks[sq] = attacks;
	}
}



void MoveGenerator::generate_king_moves(const Color color, const Board& board) { //computes all possible pseudo-legal moves for king
	u64 own_pieces = board[color];
	u64 king = board[Piece::KING] & own_pieces;

	//TODO: if king == 0, countr_zero returns 64 -> out-of-bounds access below. Guard this once captures/missing king become possible.
	int sq = std::countr_zero(king);

	u64 attacks = zero_constraint_king_move_masks[sq];

	attacks &= ~own_pieces;

	while (attacks) {
		int sq_new = std::countr_zero(attacks);

		pseudo_legal_move_list[pseudo_legal_move_list_index].from = sq;
		pseudo_legal_move_list[pseudo_legal_move_list_index++].to = sq_new;

		attacks &= attacks - 1;
	}
}

void MoveGenerator::init_knight_occupancy_masks() { //computes all possible moves for knight
	for (int sq = 0; sq < 64; sq++) {
		int r = sq / 8;
		int f = sq % 8;

		u64 attacks = 0ULL;
		for (auto& d : KNIGHT_DELTAS) {
			int nr = r + d[0];
			int nf = f + d[1];

			if (nr >= RANK_COUNT || nr < 0 || nf >= FILE_COUNT || nf < 0) continue;

			int sq_new = (nr * 8) + nf;

			attacks |= 1ULL << sq_new;
		}
		zero_constraint_knight_move_masks[sq] = attacks;
	}
}

void MoveGenerator::generate_knight_moves(const Color color, const Board& board) { //computes all possible pseudo-legal moves for knight
	u64 own_pieces = board[color];
	u64 knight = board[Piece::KNIGHT] & own_pieces;

	u64 temp = knight;

	while (temp) {
		int sq = std::countr_zero(temp);
		u64 attacks_pos = zero_constraint_knight_move_masks[sq];

		attacks_pos = attacks_pos & ~own_pieces;

		while (attacks_pos) {
			int sq_atck = std::countr_zero(attacks_pos);

			pseudo_legal_move_list[pseudo_legal_move_list_index].from = sq;
			pseudo_legal_move_list[pseudo_legal_move_list_index++].to = sq_atck;

			attacks_pos &= attacks_pos - 1;
		}

		temp &= temp - 1;
	}
}
