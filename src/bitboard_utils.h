#pragma once

#include "types.h"
#include "constants.h"

#include <array>
#include <string>

struct bitboard{
		u64 board;

		u64 p = 0ULL;
		u64 r = 0ULL;
		u64 n = 0ULL; //n stands for knight in fen notation
		u64 b = 0ULL;
		u64 q = 0ULL;
		u64 k = 0ULL;

		u64 P = 0ULL;
		u64 R = 0ULL;
		u64 N = 0ULL; 
		u64 B = 0ULL;
		u64 Q = 0ULL;
		u64 K = 0ULL;

		bool piece_data;

		char generate_char(int sq);

		bitboard(u64 bb);

		bitboard(u64 b, u64 pp, u64 rr, u64 nn, u64 bb, u64 qq, u64 kk, u64 PP, u64 RR, u64 NN, u64 BB, u64 QQ, u64 KK);
	};	

namespace bbu {
	void print_bit_board(bitboard& bb); //prints any given bitboard as a chess board

	u64 randu64(); //returns a random u64 using xor

	u64 ray_walk(int sq, u64 ocp, std::array<std::array<int, 2>, 4> deltas, bool omit_edge); //attack bitboard of a sliding piece on sq, rays stop at the first blocker in ocp

	bitboard parse_fen(std::string fen);
}
