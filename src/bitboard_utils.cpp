#include "bitboard_utils.h"
#include "constants.h"

#include <iostream>
#include <bit>
#include <array>
#include <random>

void bbu::print_bit_board(bitboard& bb) { //prints the board out to visualize the chess board
	std::array<std::array<char, 8>, 8> board;

	for (auto& row : board) {
		row.fill('.');
	}

	u64 b = bb.board;

	while (b) {
		int sq = std::countr_zero(b);

		int r = sq / RANK_COUNT;
		int f = sq % FILE_COUNT;

		char c = bb.generate_char(sq);
		board[r][f] = c;

		b &= b - 1;
	}

	for (int i = RANK_COUNT - 1; i >= 0; i--) {
		for (int j = 0; j < FILE_COUNT; j++) {
			std::cout << board[i][j];
		}
		std::cout << "\n";
	}

	std::cout << "\n\n";
}

u64 bbu::randu64() {
	static std::random_device rd; 
	static std::mt19937_64 gen(rd());

	u64 N = gen();

	return N;
}

u64 bbu::ray_walk(int sq, u64 ocp, std::array<std::array<int, 2>, 4> deltas, bool omit_edge) { //reference ray walk, blocker squares are included
	int r = sq / 8;
	int f = sq % 8;

	u64 attacks = 0ULL;
	for (auto& d : deltas) {
		int nr = r + d[0];
		int nf = f + d[1];

		while (nr >= 0 && nr < RANK_COUNT && nf >= 0 && nf < FILE_COUNT) {
			int next_nr = nr + d[0];
			int next_nf = nf + d[1];

			bool is_last_square = !(next_nr >= 0 && next_nr < RANK_COUNT && next_nf >= 0 && next_nf < FILE_COUNT);

			if (omit_edge && is_last_square) break; //edge square in the direction of the ray is left out (occupancy mask)

			u64 bit = 1ULL << (nr * 8 + nf);

			attacks |= bit;

			if (ocp & bit) break; //blocker: attacked, nothing behind it

			nr = next_nr;
			nf = next_nf;
		}
	}

	return attacks;
}

bitboard bbu::parse_fen(std::string fen){
	u64 board = 0ULL;

	//lowercase for black uppercase for white
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

	char divider = '/'; // '/' for seperating ranks top to bottom
	int rank = 7;
	int file = 0;
	for(int i = 0; i < fen.size(); i++){
		int sq = rank * 8 + file;

		if(fen[i] == ' ') break; //castling rights etc. is ignored for now

		if(fen[i] == divider) {
			rank--;
			file = 0;
			continue;
		}

		//starting from msb, top of the board
		if(fen[i] <= 56 && fen[i] >= 49){ //utilizing ascii table for integers in fen
			int num = (int)fen[i] - 48;

			file += num; //empty squares
			continue;
		}

		switch(fen[i]) {
			case 'p': p = p | 1ULL << sq; board =  board | 1ULL << sq; break;
			case 'r': r = r | 1ULL << sq; board =  board | 1ULL << sq; break;
			case 'n': n = n | 1ULL << sq; board =  board | 1ULL << sq; break;
			case 'b': b = b | 1ULL << sq; board =  board | 1ULL << sq; break;
			case 'q': q = q | 1ULL << sq; board =  board | 1ULL << sq; break;
			case 'k': k = k | 1ULL << sq; board =  board | 1ULL << sq; break;

			case 'P': P = P | 1ULL << sq; board =  board | 1ULL << sq; break;
			case 'R': R = R | 1ULL << sq; board =  board | 1ULL << sq; break;
			case 'N': N = N | 1ULL << sq; board =  board | 1ULL << sq; break;
			case 'B': B = B | 1ULL << sq; board =  board | 1ULL << sq; break;
			case 'Q': Q = Q | 1ULL << sq; board =  board | 1ULL << sq; break;
			case 'K': K = K | 1ULL << sq; board =  board | 1ULL << sq; break;
		}
		file++;
	}

	return bitboard{board,p,r,n,b,q,k,P,R,N,B,Q,K};
}

bitboard::bitboard(u64 bb)  :
	board(bb),
	piece_data(false) {}

bitboard::bitboard(u64 b, u64 pp, u64 rr, u64 nn, u64 bb, u64 qq, u64 kk, u64 PP, u64 RR, u64 NN, u64 BB, u64 QQ, u64 KK) :
			board(b), p(pp), r(rr), n(nn), b(bb), q(qq), k(kk), P(PP), R(RR), N(NN), B(BB), Q(QQ), K(KK), piece_data(true) {}

char bitboard::generate_char(int sq){
	if(piece_data){
		if(this->B & 1ULL << sq) return 'B';
		if(this->b & 1ULL << sq) return 'b';
		if(this->N & 1ULL << sq) return 'N';
		if(this->n & 1ULL << sq) return 'n';
		if(this->Q & 1ULL << sq) return 'Q';
		if(this->q & 1ULL << sq) return 'q';
		if(this->R & 1ULL << sq) return 'R';
		if(this->r & 1ULL << sq) return 'r';
		if(this->P & 1ULL << sq) return 'P';
		if(this->p & 1ULL << sq) return 'p';
		if(this->K & 1ULL << sq) return 'K';
		if(this->k & 1ULL << sq) return 'k';
	}

	return '1';
}