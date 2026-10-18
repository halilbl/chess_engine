#pragma once

#include "types.h"
#include "constants.h"
#include "board.h"
#include "move.h"

#include <utility>
#include <span>

//Removed anon namespaces for now. Becuse it contained arrays, tables that i needed to access outside of this .cpp/.h file.
struct SlidingPiece{
	std::span<u64> occupancy;
	std::span<u64> magic;
	std::span<u64> lookup_table; //1d array acts like a 2d table
	std::span<const std::array<int, 2>> deltas_view; //position of const matters

	const size_t row_size; 

	SlidingPiece() = delete;

	SlidingPiece(std::span<u64> ocp, std::span<u64> mgc, std::span<u64> lt, std::span<const std::array<int, 2>> d, const size_t rs) : 
		occupancy(ocp),
		magic(mgc),
		lookup_table(lt),
		deltas_view(d),
		row_size(rs) {}	
		//Explain why std::span should not be passed as a reference in this case in EXPR.md
};

struct LeapingPiece{
	std::span<u64> occupancy;
	std::span<const std::array<int, 2>> deltas_view;

	LeapingPiece(std::span<u64> ocp, std::span<const std::array<int, 2>> d) :
	occupancy(ocp),
	deltas_view(d) {} 
};

namespace MoveGenerator{
	inline std::array<u64, SQUARE_COUNT> king_occupancy;
	inline std::array<u64, SQUARE_COUNT> knight_occupancy;
	inline std::array<u64, SQUARE_COUNT> bishop_occupancy;
	inline std::array<u64, SQUARE_COUNT> rook_occupancy;

	inline std::array<u64, SQUARE_COUNT> bishop_magic;
	inline std::array<u64, SQUARE_COUNT> rook_magic;

	inline std::array<u64, MAX_BISHOP_OCP * SQUARE_COUNT> bishop_lookup_table;
	inline std::array<u64, MAX_ROOK_OCP * SQUARE_COUNT> rook_lookup_table;

	inline SlidingPiece bishop(bishop_occupancy, bishop_magic, bishop_lookup_table, BISHOP_DELTAS, MAX_BISHOP_OCP);
	inline SlidingPiece rook(rook_occupancy, rook_magic, rook_lookup_table, ROOK_DELTAS, MAX_ROOK_OCP);

	inline LeapingPiece king(king_occupancy, KING_DELTAS);
	inline LeapingPiece knight(knight_occupancy, KNIGHT_DELTAS);

	//Had to use inline for all tables/arrays or else it LNK2005 error output. LNK2005 indicates that multiple definitions of a variable is found (ODR violation). Inline tells linker to allow multiple definitons if definitons are the same. Could also utilized extern as programmers did before C++ 17. Extern prevents a variable from being inherently defined and tells linker that the definition is elsewhere.

	u64 get_king_ocp_mask(size_t sq);   //Testing purposes
	u64 get_knight_ocp_mask(size_t sq); //Testing purposes
	u64 get_bishop_attack(size_t sq, size_t index); //Testing purposes
	u64 get_bishop_magic(size_t sq) ; //Testing purposes
	
	void init_sp_occupancy(SlidingPiece p);
	void init_lp_occupancy(LeapingPiece p); //leaping piece -> knight, king
	void init_lists();

	u64 compute_sp_attack(int sq, SlidingPiece p, u64 subset);

	void generate_king_moves(const Color color, const Board& board, std::array<Move, MAX_MOVES_IN_A_TURN>& movel, size_t& movel_idx);
	void generate_knight_moves(const Color color, const Board& board, std::array<Move, MAX_MOVES_IN_A_TURN>& movel, size_t& movel_idx);
	void generate_bishop_moves(const Color color, const Board& board, std::array<Move, MAX_MOVES_IN_A_TURN>& movel, size_t& movel_idx);
	void generate_magic(SlidingPiece p);
}