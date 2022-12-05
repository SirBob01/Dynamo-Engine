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

TEST_CASE("Barycentric inside", "[Math]") {
    Dynamo::Vec2 point(0, 0);
    Dynamo::Vec2 a(-1, -0.5);
    Dynamo::Vec2 b(1, -0.5);
    Dynamo::Vec2 c(0, 1);

    Dynamo::Vec3 coords = Dynamo::barycentric(point, a, b, c);
    Dynamo::Vec2 comb = a * coords.x + b * coords.y + c * coords.z;
    REQUIRE_THAT(comb.x, Approx(point.x));
    REQUIRE_THAT(comb.y, Approx(point.y));
}

TEST_CASE("Barycentric outside", "[Math]") {
    Dynamo::Vec2 point(-2, 1);
    Dynamo::Vec2 a(-1, -0.5);
    Dynamo::Vec2 b(1, -0.5);
    Dynamo::Vec2 c(0, 1);

    Dynamo::Vec3 coords = Dynamo::barycentric(point, a, b, c);
    Dynamo::Vec2 comb = a * coords.x + b * coords.y + c * coords.z;
    REQUIRE_THAT(comb.x, Approx(point.x));
    REQUIRE_THAT(comb.y, Approx(point.y));
}

TEST_CASE("Barycentric on vertex", "[Math]") {
    Dynamo::Vec2 point(-1, -0.5);
    Dynamo::Vec2 a(-1, -0.5);
    Dynamo::Vec2 b(1, -0.5);
    Dynamo::Vec2 c(0, 1);

    Dynamo::Vec3 coords = Dynamo::barycentric(point, a, b, c);
    Dynamo::Vec2 comb = a * coords.x + b * coords.y + c * coords.z;
    REQUIRE_THAT(comb.x, Approx(point.x));
    REQUIRE_THAT(comb.y, Approx(point.y));

    // No contribution from the other vertices
    REQUIRE_THAT(coords.x, Approx(1));
    REQUIRE_THAT(coords.y, Approx(0));
    REQUIRE_THAT(coords.z, Approx(0));
}
