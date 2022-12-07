#include <Dynamo.hpp>
#include <catch2/catch_test_macros.hpp>

#include "Common.hpp"

TEST_CASE("Triangle circumcircle", "[Triangle]") {
    Dynamo::Vec2 a(8, 5);
    Dynamo::Vec2 b(5, 5);
    Dynamo::Vec2 c(5, 9);

    Dynamo::Triangle2 triangle(a, b, c);
    Dynamo::Circle circle = triangle.circumcircle();

    REQUIRE_THAT(circle.center.x, Approx(6.5));
    REQUIRE_THAT(circle.center.y, Approx(7));
    REQUIRE_THAT(circle.radius, Approx(2.5));
}

TEST_CASE("Triangle barycentric inside", "[Triangle]") {
    Dynamo::Vec2 point(0, 0);
    Dynamo::Vec2 a(-1, -0.5);
    Dynamo::Vec2 b(1, -0.5);
    Dynamo::Vec2 c(0, 1);

    Dynamo::Triangle2 triangle(a, b, c);
    Dynamo::Vec3 coords = triangle.barycentric(point);
    Dynamo::Vec2 comb = a * coords.x + b * coords.y + c * coords.z;
    REQUIRE_THAT(comb.x, Approx(point.x));
    REQUIRE_THAT(comb.y, Approx(point.y));
}

TEST_CASE("Triangle barycentric outside", "[Triangle]") {
    Dynamo::Vec2 point(-2, 1);
    Dynamo::Vec2 a(-1, -0.5);
    Dynamo::Vec2 b(1, -0.5);
    Dynamo::Vec2 c(0, 1);

    Dynamo::Triangle2 triangle(a, b, c);
    Dynamo::Vec3 coords = triangle.barycentric(point);
    Dynamo::Vec2 comb = a * coords.x + b * coords.y + c * coords.z;
    REQUIRE_THAT(comb.x, Approx(point.x));
    REQUIRE_THAT(comb.y, Approx(point.y));
}

TEST_CASE("Triangle barycentric on vertex", "[Triangle]") {
    Dynamo::Vec2 point(-1, -0.5);
    Dynamo::Vec2 a(-1, -0.5);
    Dynamo::Vec2 b(1, -0.5);
    Dynamo::Vec2 c(0, 1);

    Dynamo::Triangle2 triangle(a, b, c);
    Dynamo::Vec3 coords = triangle.barycentric(point);
    Dynamo::Vec2 comb = a * coords.x + b * coords.y + c * coords.z;
    REQUIRE_THAT(comb.x, Approx(point.x));
    REQUIRE_THAT(comb.y, Approx(point.y));

    // No contribution from the other vertices
    REQUIRE_THAT(coords.x, Approx(1));
    REQUIRE_THAT(coords.y, Approx(0));
    REQUIRE_THAT(coords.z, Approx(0));
}

TEST_CASE("Triangle winding clockwise", "[Triangle]") {
    Dynamo::Vec2 a(0, 0);
    Dynamo::Vec2 b(0, 1);
    Dynamo::Vec2 c(1, 0);
    Dynamo::Triangle2 triangle(a, b, c);
    REQUIRE(triangle.winding() < 0);
}

TEST_CASE("Triangle winding collinear", "[Triangle]") {
    Dynamo::Vec2 a(0, 0);
    Dynamo::Vec2 b(0, 1);
    Dynamo::Vec2 c(0, 2);
    Dynamo::Triangle2 triangle(a, b, c);
    REQUIRE(triangle.winding() == 0);
}

TEST_CASE("Triangle winding anti-clockwise", "[Triangle]") {
    Dynamo::Vec2 a(0, 0);
    Dynamo::Vec2 b(1, 0);
    Dynamo::Vec2 c(0, 1);
    Dynamo::Triangle2 triangle(a, b, c);
    REQUIRE(triangle.winding() > 0);
}

TEST_CASE("Triangle equality", "[Triangle]") {
    Dynamo::Vec2 a(0, 0);
    Dynamo::Vec2 b(1, 0);
    Dynamo::Vec2 c(0, 1);

    Dynamo::Triangle2 triangle1(a, b, c);
    Dynamo::Triangle2 triangle2(a, b, c);

    REQUIRE(triangle1 == triangle2);
}

TEST_CASE("Triangle inequality", "[Triangle]") {
    Dynamo::Vec2 a(0, 0);
    Dynamo::Vec2 b(1, 0);
    Dynamo::Vec2 c(0, 1);

    Dynamo::Triangle2 triangle1(a, b, c);
    Dynamo::Triangle2 triangle2(c, a, b);

    REQUIRE(triangle1 != triangle2);
}