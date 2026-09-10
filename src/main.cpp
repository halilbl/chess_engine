#include <iostream>

#include "board.h"
#include "move_generator.h"
#include "bitboard_utils.h"

int main() {

	Board board{};
	MoveGenerator move_gen{};

	move_gen.init_knight_attacks();
	move_gen.generateKnightMoves(Color::WHITE, board);

	for (size_t i = 0; i < move_gen.move_list_index; i++) {
		std::cout << move_gen[i].from << "-->" << move_gen[i].to << '\n';
	}

	return 0;
}
