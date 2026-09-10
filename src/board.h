#pragma once

#include "types.h"
#include "constants.h"

class Board {
private:
	u64 piece_bitboards[PIECE_TYPE_AMOUNT];
	u64 color_bitboards[COLOR_TYPE_AMOUNT];

	u64 board;

	int move_counter;

public:
	Board();

	u64 operator[](Piece piece) const;
	u64 operator[](Color color) const;

	Color turn;
};
