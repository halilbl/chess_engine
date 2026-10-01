#pragma once

#include "types.h"
#include "constants.h"

namespace bbu {
	void print_bit_board(u64 bb); //prints any given bitboard as a chess board

	u64 randu64(); //returns a random u64 using xor
}
