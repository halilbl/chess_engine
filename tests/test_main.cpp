#define CATCH_CONFIG_MAIN

#include <catch2/catch_test_macros.hpp>
#include <bit>

int f() {
	uint64_t number = 1ULL << 3;

	int a = std::countr_zero(number);

	return a;
}

TEST_CASE("countr_zero benchmark", "[countr_zero]") {
	REQUIRE(f() == 3);
}
	