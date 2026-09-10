#pragma once

#include <array>

#include "types.h"
#include "constants.h"

class Board {
private:
	std::array<u64, PIECE_TYPE_COUNT> piece_bitboards;
	std::array<u64, COLOR_TYPE_COUNT> color_bitboards;

	u64 board;

public:
	Board();

	u64 operator[](Piece piece) const;
	u64 operator[](Color color) const;
	u64& operator[](Piece piece);
	u64& operator[](Color color);

	Color turn;
};
