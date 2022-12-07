#include <Dynamo.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Segment2 length squared", "[Segment2]") {
    Dynamo::Segment2 a({0, 0}, {2, 0});
    REQUIRE(a.length_squared() == 4);
}

TEST_CASE("Segment2 length", "[Segment2]") {
    Dynamo::Segment2 a({0, 0}, {2, 0});
    REQUIRE(a.length() == 2);
}

TEST_CASE("Segment2 reverse", "[Segment2]") {
    Dynamo::Segment2 a({0, 0}, {2, 0});
    Dynamo::Segment2 b = -a;

    REQUIRE(a.a == b.b);
    REQUIRE(a.b == b.a);
}

TEST_CASE("Segment2 equality", "[Segment2]") {
    Dynamo::Segment2 a({0, 0}, {2, 4});
    Dynamo::Segment2 b({0, 0}, {2, 4});
    REQUIRE(a == b);
}

TEST_CASE("Segment2 inequality", "[Segment2]") {
    Dynamo::Segment2 a({0, 0}, {2, 4});
    Dynamo::Segment2 b({1, 0}, {2, 3});

    REQUIRE(a != b);
}

TEST_CASE("Segment2 hash", "[Segment2]") {
    REQUIRE_NOTHROW(std::unordered_set<Dynamo::Segment2>());
}
