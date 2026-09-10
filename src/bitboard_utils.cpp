#include "bitboard_utils.h"

#include <iostream>
#include <cstring>
#include <bit>

void print_bit_board(u64 bb) {
	char arr[8][8];
	std::memset(arr, '.', sizeof(arr));

	u64 temp = bb;

	while (temp) {
		int sq = std::countr_zero(temp);

		int r = sq / 8;
		int f = sq % 8;

		arr[r][f] = '1';

		temp &= temp - 1;
	}

	for (int i = 7; i >= 0; i--) {
		for (int j = 0; j < 8; j++) {
			std::cout << arr[i][j];
		}

		std::cout << '\n';
	}
}
