#pragma once

#include <cstdint>
#include <cstddef>

using u64 = uint64_t;
using u32 = uint32_t;
using u16 = uint16_t;
using u8  = uint8_t;

enum class Piece : u8 {
	PAWN, BISHOP, KNIGHT, ROOK, QUEEN, KING
};

enum class Color : u8 {
	BLACK, WHITE
};

constexpr size_t idx(Piece p) { return static_cast<size_t>(p); }
constexpr size_t idx(Color c) { return static_cast<size_t>(c); }
