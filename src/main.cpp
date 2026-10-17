#include <iostream>

#include "board.h"
#include "move_generator.h"
#include "bitboard_utils.h"
#include "types.h"

#include <random>
#include <cmath>
#include <bitset>
#include <type_traits>
#include <bit>

namespace mg = MoveGenerator;

int main() {

	mg::init_lists();
	Board board{};

	std::array<Move, MAX_MOVES_IN_A_TURN> movel; //if an object is to be contained, it has to have a default constructor
	size_t moveli = 0;

	mg::generate_bishop_moves(Color::WHITE, board, movel, moveli);
	mg::generate_king_moves(Color::WHITE, board, movel, moveli);
	mg::generate_knight_moves(Color::WHITE, board, movel, moveli);

	bitboard b = bbu::parse_fen("8/8/1K6/pq1p4/k7/8/8/8 w - - 2 57");
	
	bbu::print_bit_board(b);

	return 0;
}
