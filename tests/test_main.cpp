#define CATCH_CONFIG_MAIN

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>

#include "move_generator.h"
#include "constants.h"
#include "types.h"
#include "bitboard_utils.h"

#include <bit>
#include <set>

namespace mg = MoveGenerator;

TEST_CASE("compute_sliding_piece_attack() for bishop", "[move_generator]"){
    int sq = GENERATE(range(0,64));
    auto mask = GENERATE(take(200, random(0ULL, ~0ULL)));

    CAPTURE(sq, mask);
    CHECK(mg::compute_sliding_piece_attack(sq, mask, BISHOP_DELTAS) == bbu::ray_walk(sq, mask, BISHOP_DELTAS, false));
}

TEST_CASE("compute_occupancy_mask() for bishop", "[move_generator]"){
    int sq = GENERATE(range(0,64));

    CAPTURE(sq);
    CHECK(mg::compute_occupancy_mask(sq, BISHOP_DELTAS) == bbu::ray_walk(sq, 0ULL, BISHOP_DELTAS, true));
}

TEST_CASE("generate_magic() for bishop", "[move_generator]"){
    mg::init_bishop_lists(BISHOP_DELTAS);

        int sq = GENERATE(range(0, SQUARE_COUNT));

        u64 mask = mg::compute_occupancy_mask(sq, BISHOP_DELTAS);
        u64 subset = mask;

        int rb = std::popcount(mask);
        int subset_count = 1ULL << rb;

        auto arr = mg::get_bishop_array(sq);
        CAPTURE(sq, mask, subset);

        for(int i = 0; i < subset_count; i++){ 
            size_t index = (mg::bishop_magic[sq] * subset) >> (64 - rb);

            CHECK(arr[index] == bbu::ray_walk(sq, subset, BISHOP_DELTAS, false));
            subset = (subset - 1) & mask;
        }
    
}