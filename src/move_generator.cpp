#include "move_generator.h"

#include "bitboard_utils.h"

#include <bit>
#include <iostream>
#include <utility>
#include <vector>

namespace mg = MoveGenerator;

	/*Switched to namespaces instead of a class. This is an anonymous namespace which roughly has the same attributes.An anonmyous namespace variables inherently has internal linkage and they do not have an explicitly defined name. Which means these variables are not and can't be linked to another file, which provides something like encapsulation.*/

	//removed anon namespace because testing needs publicity

u64 mg::get_king_ocp_mask(size_t sq) { //Testing purposes
	return king_occupancy[sq];
}

u64 mg::get_knight_ocp_mask(size_t sq) { //Testing purposes
	return knight_occupancy[sq];
}

u64 mg::get_bishop_attack(size_t sq, size_t index) { //Testing purposes
	return bishop_lookup_table[sq * MAX_BISHOP_OCP + index];
}

u64 mg::get_bishop_magic(size_t sq) { //Testing purposes
	return bishop_magic[sq];
}

void mg::init_sp_occupancy(SlidingPiece p) { //occupancy bitboard without blockers
	for(int sq = 0; sq < SQUARE_COUNT; sq++){
		int r = sq / 8;
		int f = sq % 8;

		u64 mask = 0ULL;
		for (auto& d: p.deltas_view) { //genuine for all sliding pieces via deltas parameter
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
		p.occupancy[sq] = mask;
	}
}

void mg::init_lp_occupancy(LeapingPiece p) { //comptues all possible pseudo-legal moves for king
	for (int sq = 0; sq < 64; sq++) {
		int r = sq / 8;
		int f = sq % 8;

		u64 ocp = 0ULL;
		for (auto& d : p.deltas_view) {
			int nr = r + d[0];
			int nf = f + d[1];

			if (nr >= RANK_COUNT || nr < 0 || nf >= FILE_COUNT || nf < 0) continue;

			int sq_new = (nr * 8) + nf;

			ocp |= 1ULL << sq_new;
		}
		p.occupancy[sq] = ocp;
	}
}

void mg::init_lists() { //call this function to initialize everything
	//sp list per piece is 3
	init_sp_occupancy(bishop);
	init_sp_occupancy(rook);
	generate_magic(bishop);//inits magic and lookup
	generate_magic(rook);
	
	//lp list per piece is 1
	init_lp_occupancy(king);
	init_lp_occupancy(knight);
}

u64 mg::compute_sp_attack(int sq, SlidingPiece p, u64 subset) { //attack bitboard with blockers
	int r = sq / 8;
	int f = sq % 8;

	u64 attacks = 0ULL;

	for (auto& d : p.deltas_view) {
		int nr = r + d[0];
		int nf = f + d[1];

		while (nr >= 0 && nr < RANK_COUNT && nf >= 0 && nf < FILE_COUNT) {
			int new_sq = nr * 8 + nf;		

			attacks |= 1ULL << new_sq;

			if (subset & (1ULL << new_sq)) break;

			nr += d[0];
			nf += d[1];
		}
	}

	return attacks;
}

void mg::generate_king_moves(const Color color, const Board& board, std::array<Move, MAX_MOVES_IN_A_TURN>& movel, size_t& movel_idx) { //computes all possible pseudo-legal moves for king
	u64 own_pieces = board[color];
	u64 king_board = board[Piece::KING] & own_pieces;

	//TODO: if king == 0, countr_zero returns 64 -> out-of-bounds access below. Guard this once captures/missing king become possible.
	int sq = std::countr_zero(king_board);

	u64 attacks = king_occupancy[sq];

	attacks &= ~own_pieces;

	while (attacks) {
		int sq_new = std::countr_zero(attacks);

		Move move;
		move.from = sq;
		move.to = sq_new;

		movel[movel_idx++] = move;

		attacks &= attacks - 1;
	}
}

void mg::generate_knight_moves(const Color color, const Board& board, std::array<Move, MAX_MOVES_IN_A_TURN>& movel, size_t& movel_idx) { //computes all possible pseudo-legal moves for knight
	u64 own_pieces = board[color];
	u64 knight_board = board[Piece::KNIGHT] & own_pieces;

	u64 temp = knight_board;

	while (temp) {
		int sq = std::countr_zero(temp);
		u64 attacks_pos = knight_occupancy[sq];

		attacks_pos = attacks_pos & ~own_pieces;

		while (attacks_pos) {
			int sq_atck = std::countr_zero(attacks_pos);

			Move move;
			move.from = sq;
			move.to = sq_atck;
			
			movel[movel_idx++] = move;

			attacks_pos &= attacks_pos - 1;
		}

		temp &= temp - 1;
	}
}

void MoveGenerator::generate_bishop_moves(const Color color, const Board& board, std::array<Move, MAX_MOVES_IN_A_TURN>& movel, size_t& movel_idx){
	u64 bishop_board = board[Piece::BISHOP] & board[color];

	while(bishop_board){
		int sq = std::countr_zero(bishop_board);

		u64 subset = bishop_occupancy[sq] & board.board_bitboard;

		size_t index = (subset * bishop_magic[sq]) >> (64 - std::popcount(bishop_occupancy[sq]));
		u64 attack = bishop_lookup_table[sq * MAX_BISHOP_OCP + index];
		attack &= ~board[color];

		while(attack){
			int sq_atck = std::countr_zero(attack);

			Move move;
			move.from = sq;
			move.to = sq_atck;

			movel[movel_idx++] = move;

			attack &= attack - 1;
		}

		bishop_board &= bishop_board - 1;
	}

}

void mg::generate_magic(SlidingPiece p) { //returns the magic number and fills lookup table, a bruteforce algorithm to map indexes
	for(int sq = 0; sq < SQUARE_COUNT; sq++){
		u64 mask = p.occupancy[sq];

		int relevant_bits = std::popcount(mask);
		int shift = 64 - relevant_bits;
		size_t subset_count = 1ULL << relevant_bits;

		std::array<u64, MAX_ROOK_OCP> subsets; //MAX_ROOK_OCP is the largest
		std::array<u64, MAX_ROOK_OCP> attacks; 

		subsets.fill(0ULL);
		attacks.fill(0ULL);

		u64 subset = mask;
		for (size_t i = 0; i < subset_count; i++) { 
			subsets[i] = subset;
			attacks[i] = compute_sp_attack(sq, p, subset);

			subset = (subset - 1) & mask;
		}

		std::array<u64, MAX_ROOK_OCP> table; 
		std::array<int, MAX_ROOK_OCP> table_epoch;

		table.fill(0ULL);
		table_epoch.fill(0);

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
				for (size_t i = 0; i < subset_count; i++) { //initializes lookup table for this square
					index = (subsets[i] * magic) >> shift;
					//index can be printed out here to test this funciton
					p.lookup_table[sq * p.row_size + index] = attacks[i]; //lookup table gets filled
					
				}

				p.magic[sq] = magic;
				break;
			}
		}
	}
}
