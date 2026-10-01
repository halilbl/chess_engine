#include "bitboard_utils.h"
#include "constants.h"

#include <iostream>
#include <bit>
#include <array>
#include <random>

void bbu::print_bit_board(u64 bb) {
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
	std::random_device rd; //TODO:with -O0 it takes ~5 seconds to execute the first two lines, make it static or smth else
	std::mt19937_64 gen(rd());

	u64 N = gen();

	return N;
}
