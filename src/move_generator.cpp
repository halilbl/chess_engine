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

std::array<u64, MAX_SUBSETS_OF_BISHOP_OCP> MoveGenerator::get_bishop_subsets(size_t sq) const { //Testing purposes
	return bishop_lookup_table[sq];
}

u64 MoveGenerator::get_bishop_magic(size_t sq) const { //Testing purposes
	return bishop_magic[sq];
}

u64 MoveGenerator::compute_occupancy_mask(int sq, std::array<std::array<int,2>,4> deltas) { //occupancy bitboard without blockers
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

			bool next_sq = (next_nr >= 0 && next_nr < RANK_COUNT && next_nf >= 0 && next_nf < FILE_COUNT);

			if (!next_sq) break; //omit edge squares

			int new_sq = nr * 8 + nf;

			mask |= 1ULL << new_sq;

			nr = next_nr;
			nf = next_nf;
		}
	}

	return mask;
}

u64 MoveGenerator::compute_sliding_piece_attack(int sq, u64 ocp, std::array<std::array<int,2>,4> deltas) { //attack bitboard with blockers
	int r = sq / 8;
	int f = sq % 8;

	u64 attacks = 0ULL;

	for (auto& d : deltas) {
		int nr = r + d[0];
		int nf = f + d[1];

		while (nr >= 0 && nr < RANK_COUNT && nf >= 0 && nf < FILE_COUNT) {
			int new_sq = nr * 8 + nf;		

			attacks |= 1ULL << new_sq;

			if (ocp & (1ULL << new_sq)) break;

			nr += d[0];
			nf += d[1];
		}
	}

	return attacks;
}

u64 MoveGenerator::generate_magic(int sq, std::array<std::array<int,2>,4> deltas) { //returns the magic number and fills lookup table, a bruteforce algorithm to map indexes
	u64 mask = compute_occupancy_mask(sq, deltas);

	int relevant_bits = std::popcount(mask);
	int shift = 64 - relevant_bits;
	size_t subset_count = 1ULL << relevant_bits;

	std::array<u64, MAX_SUBSETS_OF_BISHOP_OCP> subsets{}; 
	std::array<u64, MAX_SUBSETS_OF_BISHOP_OCP> attacks{}; 

	u64 subset = 0ULL;
	for (size_t i = 0; i < subset_count; i++) { 
		subsets[i] = subset;
		attacks[i] = compute_sliding_piece_attack(sq, subset, deltas);

		subset = (subset - mask) & mask;
	}

	std::array<u64, MAX_SUBSETS_OF_BISHOP_OCP> table{}; 
	std::array<int, MAX_SUBSETS_OF_BISHOP_OCP> table_epoch{};  
	int epoch = 0;

	while (true) {
		u64 magic = bbu::randu64() & bbu::randu64() & bbu::randu64(); 

		epoch++;
		bool collision_free = true;

		size_t index = 0;
		for (size_t i = 0; i < subset_count; i++) {
			index = (subsets[i] * magic) >> shift;

			if (table_epoch[index] != epoch) { 
				table_epoch[index] = epoch;
				table[index] = attacks[i];
			}
			else if (table[index] != attacks[i]) { 
				collision_free = false;
				break;
			}
		}

		if (collision_free) {
			bishop_lookup_table[sq].fill(0ULL);

			for (size_t i = 0; i < subset_count; i++) { //initializes lookup table for this square
				index = (subsets[i] * magic) >> shift;
				//index can be printed out here to test this funciton
				bishop_lookup_table[sq][index] = attacks[i]; //lookup table gets filled
				
			}

			return magic; //this magic bitboard(unsinged 64 bit integer) will help us to work with the lookup table in O(1) time
		}
	}
}

void MoveGenerator::init_bishop_lists(std::array<std::array<int,2>,4> deltas) { //call this function to initialize everything
	for (int sq = 0; sq < SQUARE_COUNT; sq++) {
		bishop_magic[sq] = generate_magic(sq, deltas); 
		bishop_occupancy_mask[sq] = compute_occupancy_mask(sq, BISHOP_DELTAS);
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
