#include <Dynamo.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Box2 construct center and dimensions", "[Box2]") {
    Dynamo::Box2 a({0, 0}, 5, 5);
    REQUIRE(a.min.x == -2.5);
    REQUIRE(a.min.y == -2.5);
    REQUIRE(a.max.x == 2.5);
    REQUIRE(a.max.y == 2.5);
}

TEST_CASE("Box2 center", "[Box2]") {
    Dynamo::Box2 a({-2, -2}, {2, 4});
    Dynamo::Vec2 center = a.center();
    REQUIRE(center.x == 0);
    REQUIRE(center.y == 1);
    REQUIRE(a.valid());
}

TEST_CASE("Box2 width", "[Box2]") {
    Dynamo::Box2 a({-2, -2}, {2, 4});
    float width = a.width();
    REQUIRE(width == 4);
    REQUIRE(a.valid());
}

TEST_CASE("Box2 height", "[Box2]") {
    Dynamo::Box2 a({-2, -2}, {2, 4});
    float height = a.height();
    REQUIRE(height == 6);
    REQUIRE(a.valid());
}

TEST_CASE("Box2 area", "[Box2]") {
    Dynamo::Box2 a({-2, -2}, {2, 4});
    REQUIRE(a.area() == 24);
}

TEST_CASE("Box2 translate", "[Box2]") {
    Dynamo::Box2 a({-2, -2}, {2, 4});
    Dynamo::Vec2 offset(2, 2);
    a.translate(offset);

    Dynamo::Vec2 n_min(0, 0);
    Dynamo::Vec2 n_max(4, 6);
    REQUIRE(a.min == n_min);
    REQUIRE(a.max == n_max);
    REQUIRE(a.valid());
}

TEST_CASE("Box2 translate zero", "[Box2]") {
    Dynamo::Box2 a({-2, -2}, {2, 4});
    Dynamo::Vec2 offset;
    a.translate(offset);

    Dynamo::Vec2 n_min(-2, -2);
    Dynamo::Vec2 n_max(2, 4);
    REQUIRE(a.min == n_min);
    REQUIRE(a.max == n_max);
    REQUIRE(a.valid());
}

TEST_CASE("Box2 clamp", "[Box2]") {
    Dynamo::Box2 a({0, 0}, {4, 5});
    Dynamo::Vec2 p({-1, 4});
    Dynamo::Vec2 clamped = a.clamp(p);
    REQUIRE(clamped.x == 0);
    REQUIRE(clamped.y == 4);
}

TEST_CASE("Box2 intersects superset", "[Box2]") {
    Dynamo::Box2 a({0, 0}, {3, 4});
    Dynamo::Box2 b({1, 1}, {2, 3});
    REQUIRE(a.intersects(b));
    REQUIRE(b.intersects(a));
    REQUIRE(a.valid());
    REQUIRE(b.valid());
}

TEST_CASE("Box2 intersects horizontal", "[Box2]") {
    Dynamo::Box2 a({0, 0}, {3, 4});
    Dynamo::Box2 b({-3, 1}, {1, 3});
    REQUIRE(a.intersects(b));
    REQUIRE(b.intersects(a));
    REQUIRE(a.valid());
    REQUIRE(b.valid());
}

TEST_CASE("Box2 intersects vertical", "[Box2]") {
    Dynamo::Box2 a({0, 0}, {4, 3});
    Dynamo::Box2 b({1, -3}, {3, 1});
    REQUIRE(a.intersects(b));
    REQUIRE(b.intersects(a));
    REQUIRE(a.valid());
    REQUIRE(b.valid());
}

TEST_CASE("Box2 intersects bottom left", "[Box2]") {
    Dynamo::Box2 a({0, 0}, {4, 3});
    Dynamo::Box2 b({-1, -3}, {3, 1});
    REQUIRE(a.intersects(b));
    REQUIRE(b.intersects(a));
    REQUIRE(a.valid());
    REQUIRE(b.valid());
}

TEST_CASE("Box2 intersects bottom right", "[Box2]") {
    Dynamo::Box2 a({0, 0}, {4, 3});
    Dynamo::Box2 b({3, -3}, {5, 1});
    REQUIRE(a.intersects(b));
    REQUIRE(b.intersects(a));
    REQUIRE(a.valid());
    REQUIRE(b.valid());
}

TEST_CASE("Box2 intersects top left", "[Box2]") {
    Dynamo::Box2 a({0, 0}, {4, 3});
    Dynamo::Box2 b({-3, 2}, {1, 5});
    REQUIRE(a.intersects(b));
    REQUIRE(b.intersects(a));
    REQUIRE(a.valid());
    REQUIRE(b.valid());
}

TEST_CASE("Box2 intersects top right", "[Box2]") {
    Dynamo::Box2 a({0, 0}, {4, 3});
    Dynamo::Box2 b({3, 2}, {5, 5});
    REQUIRE(a.intersects(b));
    REQUIRE(b.intersects(a));
    REQUIRE(a.valid());
    REQUIRE(b.valid());
}

TEST_CASE("Box2 no intersects", "[Box2]") {
    Dynamo::Box2 a({0, 0}, {4, 3});
    Dynamo::Box2 b({5, -3}, {5, 1});
    REQUIRE(!a.intersects(b));
    REQUIRE(!b.intersects(a));
    REQUIRE(a.valid());
    REQUIRE(b.valid());
}

TEST_CASE("Box2 contains point inside", "[Box2]") {
    Dynamo::Box2 a({0, 0}, {2, 4});
    Dynamo::Vec2 p({1, 1});
    REQUIRE(a.contains(p));
    REQUIRE(a.valid());
}

TEST_CASE("Box2 contains point outside", "[Box2]") {
    Dynamo::Box2 a({0, 0}, {2, 4});
    Dynamo::Vec2 p({3, 1});
    REQUIRE(!a.contains(p));
    REQUIRE(a.valid());
}

TEST_CASE("Box2 contains box inside", "[Box2]") {
    Dynamo::Box2 a({0, 0}, {3, 4});
    Dynamo::Box2 b({1, 1}, {2, 3});
    REQUIRE(a.contains(b));
    REQUIRE(a.valid());
    REQUIRE(b.valid());
}

TEST_CASE("Box2 contains box outside", "[Box2]") {
    Dynamo::Box2 a({0, 0}, {2, 4});
    Dynamo::Box2 b({3, -3}, {5, 1});
    REQUIRE(!a.contains(b));
    REQUIRE(a.valid());
    REQUIRE(b.valid());
}

TEST_CASE("Box2 equality", "[Box2]") {
    Dynamo::Box2 a({0, 0}, {2, 4});
    Dynamo::Box2 b({0, 0}, {2, 4});
    REQUIRE(a == b);
    REQUIRE(a.valid());
    REQUIRE(b.valid());
}

TEST_CASE("Box2 inequality", "[Box2]") {
    Dynamo::Box2 a({0, 0}, {2, 4});
    Dynamo::Box2 b({-5, 100}, {69, 420});
    Dynamo::Box2 c({1, 0}, {2, 3});

    REQUIRE(a != b);
    REQUIRE(a != c);
    REQUIRE(b != c);

    REQUIRE(a.valid());
    REQUIRE(b.valid());
    REQUIRE(c.valid());
}

TEST_CASE("Box2 valid", "[Box2]") {
    Dynamo::Box2 a({0, 0}, {2, 4});
    REQUIRE(a.valid());
}

TEST_CASE("Box2 invalid x", "[Box2]") {
    Dynamo::Box2 a({3, 0}, {2, 4});
    REQUIRE(!a.valid());
}

TEST_CASE("Box2 invalid y", "[Box2]") {
    Dynamo::Box2 a({0, 5}, {2, 4});
    REQUIRE(!a.valid());
}
