#include <Dynamo.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Triangulation", "[Delaunay]") {
    std::vector<Dynamo::Vec2> points = {
        {0, 0},
        {5, 0},
        {5, 5},
        {0, 5},
        {2.5, 2.5},
    };
    std::vector<Dynamo::Triangle2> triangles =
        Dynamo::Delaunay::triangulate(points);
    REQUIRE(triangles.size() == 4);
}

TEST_CASE("Triangulation invalid", "[Delaunay]") {
    std::vector<Dynamo::Vec2> points = {
        {0, 0},
        {5, 0},
    };
    REQUIRE_THROWS(Dynamo::Delaunay::triangulate(points));
}

TEST_CASE("Point bounding box", "[Delaunay]") {
    std::vector<Dynamo::Vec2> points = {
        {3, 5},
        {1, 2},
        {0, 0},
        {12, 19},
        {-100, -1000},
        {169, 256},
        {49, -50},
        {50, 12},
    };

    Dynamo::Box2 bounds = Dynamo::Delaunay::calculate_bounding_volume(points);
    for (const Dynamo::Vec2 &point : points) {
        REQUIRE(bounds.contains(point));
    }
}

TEST_CASE("Super triangle", "[Delaunay]") {
    std::vector<Dynamo::Vec2> points = {
        {3, 5},
        {1, 2},
        {0, 0},
        {12, 19},
        {-100, -1000},
        {169, 256},
        {49, -50},
        {50, 12},
    };
    Dynamo::Box2 bounds = Dynamo::Delaunay::calculate_bounding_volume(points);
    Dynamo::Triangle2 triangle =
        Dynamo::Delaunay::calculate_super_triangle(bounds);

    for (const Dynamo::Vec2 &point : points) {
        Dynamo::Vec3 coords = triangle.barycentric(point);
        REQUIRE(coords.x > 0);
        REQUIRE(coords.y > 0);
        REQUIRE(coords.z > 0);
    }
}
