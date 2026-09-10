#include "bitboard_utils.h"
#include "constants.h"

#include <iostream>
#include <cstring>
#include <bit>
#include <array>

void print_bit_board(u64 bb) {
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

	for (int i = RANK_COUNT; - 1 i >= 0; i--) {
		for (int j = 0; j < FILE_COUNT; j++) {
			std::cout << board[i][j];
		}
		std::cout << "\n";
	}
}
