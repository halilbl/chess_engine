#include "move_generator.h"

#include <bit>

MoveGenerator::MoveGenerator() {
	pseudo_legal_move_list_index = 0;
}

Move& MoveGenerator::operator[](size_t i) {
	return pseudo_legal_move_list[i];
}

void MoveGenerator::init_king_moves() {
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

void MoveGenerator::init_knight_moves() {
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

void MoveGenerator::generateKnightMoves(const Color color, const Board& board) { //öncelikle her tahtanın her karesi için atın saldırabileceği noktaları hesaplayacağız.
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
