#include <Dynamo.hpp>
#include <catch2/catch_test_macros.hpp>

#include "Common.hpp"

TEST_CASE("To degrees", "[Math]") {
    float rad = M_PI;
    float deg = 180;
    REQUIRE_THAT(Dynamo::to_degrees(rad), Approx(deg));

    rad = M_PI / 4;
    deg = 45;
    REQUIRE_THAT(Dynamo::to_degrees(rad), Approx(deg));

    rad = 2 * M_PI;
    deg = 360;
    REQUIRE_THAT(Dynamo::to_degrees(rad), Approx(deg));
}

TEST_CASE("To radians", "[Math]") {
    float deg = 180;
    float rad = M_PI;
    REQUIRE_THAT(Dynamo::to_radians(deg), Approx(rad));

    deg = 45;
    rad = M_PI / 4.0;
    REQUIRE_THAT(Dynamo::to_radians(deg), Approx(rad));

    deg = 360;
    rad = 2 * M_PI;
    REQUIRE_THAT(Dynamo::to_radians(deg), Approx(rad));
}

TEST_CASE("Lerp", "[Math]") {
    REQUIRE_THAT(Dynamo::lerp(0, 1, 0), Approx(0));
    REQUIRE_THAT(Dynamo::lerp(0, 1, 1), Approx(1));
    REQUIRE_THAT(Dynamo::lerp(0, 1, 0.3), Approx(0.3));
}
