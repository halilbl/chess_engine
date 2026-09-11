#include <iostream>

#include "board.h"
#include "move_generator.h"
#include "bitboard_utils.h"

int main() {

	Board board{};
	MoveGenerator mg{};

	mg.init_king_moves();

	print_bit_board(mg.king_move_mask(36));
	

	return 0;
}
