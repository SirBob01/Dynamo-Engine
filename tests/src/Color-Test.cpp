#include <Dynamo.hpp>
#include <catch2/catch_test_macros.hpp>

#include "Common.hpp"

TEST_CASE("Color Construction", "[Color]") {
    Dynamo::Color a(0, 0.5, 1);
    Dynamo::Color b(0.3, 0.2, 0.8, 0.5);

    REQUIRE_THAT(a.r, Approx(0));
    REQUIRE_THAT(a.g, Approx(0.5));
    REQUIRE_THAT(a.b, Approx(1));
    REQUIRE_THAT(a.a, Approx(1));

    REQUIRE_THAT(b.r, Approx(0.3));
    REQUIRE_THAT(b.g, Approx(0.2));
    REQUIRE_THAT(b.b, Approx(0.8));
    REQUIRE_THAT(b.a, Approx(0.5));
}

TEST_CASE("Color lerp", "[Color]") {
    Dynamo::Color a(0, 0, 0, 0);
    Dynamo::Color b(1, 1, 1, 1);
    Dynamo::Color c = a.lerp(b, 0.3);

    REQUIRE_THAT(c.r, Approx(0.3));
    REQUIRE_THAT(c.g, Approx(0.3));
    REQUIRE_THAT(c.b, Approx(0.3));
    REQUIRE_THAT(c.a, Approx(0.3));
}

TEST_CASE("Color inversion", "[Color]") {
    Dynamo::Color color(0, 0.5, 1);
    Dynamo::Color invert = color.invert();

    REQUIRE_THAT(invert.r, Approx(1));
    REQUIRE_THAT(invert.g, Approx(0.5));
    REQUIRE_THAT(invert.b, Approx(0));
    REQUIRE_THAT(invert.a, Approx(1));
}

TEST_CASE("Color to hex", "[Color]") {
    Dynamo::Color a(0, 0, 0, 0);
    REQUIRE(a.to_hex() == 0);

    Dynamo::Color b(1, 0, 0, 1);
    REQUIRE(b.to_hex() == 0xFF0000FF);

    Dynamo::Color c(0, 1, 0, 1);
    REQUIRE(c.to_hex() == 0x00FF00FF);

    Dynamo::Color d(0, 0, 1, 1);
    REQUIRE(d.to_hex() == 0x0000FFFF);
}

TEST_CASE("Color to array", "[Color]") {
    Dynamo::Color a(0.2, 0.4, 1, 0.5);
    std::array<float, 4> arr = a.to_array();

    REQUIRE_THAT(arr[0], Approx(0.2));
    REQUIRE_THAT(arr[1], Approx(0.4));
    REQUIRE_THAT(arr[2], Approx(1));
    REQUIRE_THAT(arr[3], Approx(0.5));
}
