#pragma once

#include "types.h"
#include "constants.h"

#include <array>

namespace bbu {
	void print_bit_board(u64 bb); //prints any given bitboard as a chess board

	u64 randu64(); //returns a random u64 using xor

	u64 ray_walk(int sq, u64 ocp, std::array<std::array<int, 2>, 4> deltas, bool omit_edge); //attack bitboard of a sliding piece on sq, rays stop at the first blocker in ocp
}
