#define CATCH_CONFIG_MAIN

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>

#include "move_generator.h"
#include "constants.h"
#include "types.h"
#include "bitboard_utils.h"

#include <bit>
#include <set>
#include <mutex>

namespace mg = MoveGenerator;

TEST_CASE("compute_sliding_piece_attack() for bishop", "[move_generator]"){
    int sq = GENERATE(range(0,64));
    auto mask = GENERATE(take(200, random(0ULL, ~0ULL)));

    CAPTURE(sq, mask);
    CHECK(mg::compute_sp_attack(sq, mg::bishop, mask) == bbu::ray_walk(sq, mask, BISHOP_DELTAS, false));
}

TEST_CASE("compute_occupancy_mask() for bishop", "[move_generator]"){
    static std::once_flag flag;
    
    std::call_once(flag, []() {
        mg::init_lists();
    });

    int sq = GENERATE(range(0,64));

    CAPTURE(sq);
    CHECK(mg::bishop_occupancy[sq] == bbu::ray_walk(sq, 0ULL, BISHOP_DELTAS, true));
}

TEST_CASE("generate_magic() for bishop", "[move_generator]"){
    static std::once_flag flag;
    
    std::call_once(flag, []() {
        mg::init_lists();
    });

    int sq = GENERATE(range(0,64));

    u64 mask = mg::bishop_occupancy[sq];
    u64 subset = mask;

    int rb = std::popcount(mask);
    int sc = 1ULL << rb;

    for(int i = 0; i < sc; i++){
        size_t index = (mg::bishop_magic[sq] * subset) >> (64 - rb);

        CHECK(mg::bishop_lookup_table[sq * MAX_BISHOP_OCP + index] == bbu::ray_walk(sq, subset, BISHOP_DELTAS, false));  
        
        subset = (subset - 1) & mask;
    }
}
