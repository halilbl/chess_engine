#include "bitboard_utils.h"
#include "constants.h"

#include <iostream>
#include <bit>
#include <array>
#include <random>

void bbu::print_bit_board(u64 bb) { //prints the board out to visualize the chess board
	std::array<std::array<char, 8>, 8> board;

	for (auto& row : board) {
		row.fill('.');
	}

	u64 bitboard = bb;

	while (bitboard) {
		int sq = std::countr_zero(bitboard);

		int r = sq / RANK_COUNT;
		int f = sq % FILE_COUNT;

		board[r][f] = '1';

		bitboard &= bitboard - 1;
	}

	for (int i = RANK_COUNT - 1; i >= 0; i--) {
		for (int j = 0; j < FILE_COUNT; j++) {
			std::cout << board[i][j];
		}
		std::cout << "\n";
	}

	std::cout << "\n\n";
}

u64 bbu::randu64() {
	static std::random_device rd; 
	static std::mt19937_64 gen(rd());

	u64 N = gen();

	return N;
}

u64 bbu::ray_walk(int sq, u64 ocp, std::array<std::array<int, 2>, 4> deltas, bool omit_edge) { //reference ray walk, blocker squares are included
	int r = sq / 8;
	int f = sq % 8;

	u64 attacks = 0ULL;
	for (auto& d : deltas) {
		int nr = r + d[0];
		int nf = f + d[1];

		while (nr >= 0 && nr < RANK_COUNT && nf >= 0 && nf < FILE_COUNT) {
			int next_nr = nr + d[0];
			int next_nf = nf + d[1];

			bool is_last_square = !(next_nr >= 0 && next_nr < RANK_COUNT && next_nf >= 0 && next_nf < FILE_COUNT);

			if (omit_edge && is_last_square) break; //edge square in the direction of the ray is left out (occupancy mask)

			u64 bit = 1ULL << (nr * 8 + nf);

			attacks |= bit;

			if (ocp & bit) break; //blocker: attacked, nothing behind it

			nr = next_nr;
			nf = next_nf;
		}
	}

	return attacks;
}