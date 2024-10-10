#include <Dynamo.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Find LSB", "[Bits]") {
    REQUIRE(Dynamo::find_lsb(0b10010) == 1);
    REQUIRE(Dynamo::find_lsb(0b10000) == 4);
    REQUIRE(Dynamo::find_lsb(0b1000001) == 0);
    REQUIRE(Dynamo::find_lsb(0) == 0);
    REQUIRE(Dynamo::find_lsb(1) == 0);
}

TEST_CASE("Reverse bits", "[Bits]") {
    REQUIRE(Dynamo::reverse_bits(0b10000) == 0b00001000000000000000000000000000);
    REQUIRE(Dynamo::reverse_bits(0b10001) == 0b10001000000000000000000000000000);
    REQUIRE(Dynamo::reverse_bits(0b101000) == 0b00010100000000000000000000000000);
    REQUIRE(Dynamo::reverse_bits(0b00001000000000000000000000000000) == 0b10000);
    REQUIRE(Dynamo::reverse_bits(0b10001000000000000000000000000000) == 0b10001);
    REQUIRE(Dynamo::reverse_bits(0b00010100000000000000000000000000) == 0b101000);
}

TEST_CASE("Round to next power of 2", "[Bits]") {
    REQUIRE(Dynamo::round_pow2(0) == 0);
    REQUIRE(Dynamo::round_pow2(1) == 1);
    REQUIRE(Dynamo::round_pow2(2) == 2);
    REQUIRE(Dynamo::round_pow2(3) == 4);
    REQUIRE(Dynamo::round_pow2(4) == 4);
    for (int i = 5; i <= 8; i++) {
        REQUIRE(Dynamo::round_pow2(i) == 8);
    }
    for (int i = 9; i <= 16; i++) {
        REQUIRE(Dynamo::round_pow2(i) == 16);
    }
    for (int i = 17; i <= 32; i++) {
        REQUIRE(Dynamo::round_pow2(i) == 32);
    }
}