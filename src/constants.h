#pragma once

#include "types.h"

#include <array>

constexpr int PIECE_TYPE_COUNT = 6;
constexpr int COLOR_TYPE_COUNT = 2;

constexpr int SQUARE_COUNT = 64;

constexpr int FILE_COUNT = 8;
constexpr int RANK_COUNT = 8;

//POSITION CONSTANTS
constexpr u64 PAWNS_INITIAL_POS = 0x00FF00000000FF00ULL;
constexpr u64 BISHOPS_INITIAL_POS = 0x2400000000000024ULL;
constexpr u64 KNIGHTS_INITIAL_POS = 0x4200000000000042ULL;
constexpr u64 ROOKS_INITIAL_POS = 0x8100000000000081ULL;
constexpr u64 QUEENS_INITIAL_POS = 0x0800000000000008ULL;
constexpr u64 KINGS_INITIAL_POS = 0x1000000000000010ULL;
constexpr u64 INITAL_BOARD = 0xFFFF00000000FFFFULL;

constexpr u64 WHITE_INITIAL_POS = 0x000000000000FFFFULL;
constexpr u64 BLACK_INITIAL_POS = 0xFFFF000000000000ULL;

constexpr int MAX_MOVES_IN_A_TURN = 218;
constexpr int MAX_AMOUNT_OF_SAME_PIECE = 10;

constexpr int KING_DELTAS[8][2] = {
    {1,1}, {1,-1}, {-1,1}, {-1,-1}, {1,0}, {-1,0}, {0,1}, {0,-1}
};

constexpr int KNIGHT_DELTAS[8][2] = {
        {1,2},{2,1},{2,-1},{1,-2},{-1,-2},{-2,-1},{-2,1},{-1,2}
};

constexpr int BISHOP_DELTAS[4][2] = { //directions of a bishop
    {1,1}, {1,-1}, {-1,-1}, {-1,1}
};

constexpr size_t bishop_rook_deltas_size = 4;

constexpr int MAX_SUBSETS_OF_BISHOP_OCP = 512; //maximum of 9 squares available (considering omitting edge squares), hence maximum subset amount  = 2^9
