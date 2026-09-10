#include "board.h"

Board::Board() {
	piece_bitboards[idx(Piece::PAWN)] = PAWNS_INITIAL_POS;
	piece_bitboards[idx(Piece::KNIGHT)] = KNIGHTS_INITIAL_POS;
	piece_bitboards[idx(Piece::BISHOP)] = BISHOPS_INITIAL_POS;
	piece_bitboards[idx(Piece::ROOK)] = ROOKS_INITIAL_POS;
	piece_bitboards[idx(Piece::QUEEN)] = QUEENS_INITIAL_POS;
	piece_bitboards[idx(Piece::KING)] = KINGS_INITIAL_POS;

	color_bitboards[idx(Color::WHITE)] = WHITE_INITIAL_POS;
	color_bitboards[idx(Color::BLACK)] = BLACK_INITIAL_POS;

	turn = Color::WHITE;
	board = INITAL_BOARD;
}

u64 Board::operator[](Piece piece) const {
	return piece_bitboards[idx(piece)];
}

u64 Board::operator[](Color color) const {
	return color_bitboards[idx(color)];
}

u64& Board::operator[](Piece piece) {
	return piece_bitboards[idx(piece)];
}

u64& Board::operator[](Color color) {
	return piece_bitboards[idx(color)];
}
