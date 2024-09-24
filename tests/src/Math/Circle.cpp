#include <Dynamo.hpp>
#include <catch2/catch_test_macros.hpp>
#include <unordered_set>

TEST_CASE("Circle circumference", "[Circle]") {
    Dynamo::Circle a({0, 0}, 5);
    float circumference = M_PI * 10;
    REQUIRE(a.circumference() == circumference);
    REQUIRE(a.valid());
}

TEST_CASE("Circle diameter", "[Circle]") {
    Dynamo::Circle a({0, 0}, 5);
    float diameter = 10;
    REQUIRE(a.diameter() == diameter);
    REQUIRE(a.valid());
}

TEST_CASE("Circle area", "[Circle]") {
    Dynamo::Circle a({0, 0}, 5);
    float area = M_PI * 25;
    REQUIRE(a.area() == area);
    REQUIRE(a.valid());
}

TEST_CASE("Circle intersects", "[Circle]") {
    Dynamo::Circle a({0, 0}, 5);
    Dynamo::Circle b({5, 6}, 3);
    REQUIRE(a.intersects(b));
    REQUIRE(b.intersects(a));
    REQUIRE(a.valid());
    REQUIRE(b.valid());
}

TEST_CASE("Circle no intersects", "[Circle]") {
    Dynamo::Circle a({0, 0}, 3);
    Dynamo::Circle b({-3, 5}, 1);
    REQUIRE(!a.intersects(b));
    REQUIRE(!b.intersects(a));
    REQUIRE(a.valid());
    REQUIRE(b.valid());
}

TEST_CASE("Circle contains point inside", "[Circle]") {
    Dynamo::Circle a({0, 0}, 5);
    Dynamo::Vec2 p({1, 1});
    REQUIRE(a.contains(p));
    REQUIRE(a.valid());
}

TEST_CASE("Circle contains point outside", "[Circle]") {
    Dynamo::Circle a({0, 0}, 5);
    Dynamo::Vec2 p({3, 6});
    REQUIRE(!a.contains(p));
    REQUIRE(a.valid());
}

TEST_CASE("Circle equality", "[Circle]") {
    Dynamo::Circle a({-2, 1}, 5);
    Dynamo::Circle b({-2, 1}, 5);
    REQUIRE(a == b);
    REQUIRE(a.valid());
    REQUIRE(b.valid());
}

TEST_CASE("Circle inequality", "[Circle]") {
    Dynamo::Circle a({0, 0}, 4);
    Dynamo::Circle b({-5, 3}, 3);
    Dynamo::Circle c({1, 0}, 4);

    REQUIRE(a != b);
    REQUIRE(a != c);
    REQUIRE(b != c);

    REQUIRE(a.valid());
    REQUIRE(b.valid());
    REQUIRE(c.valid());
}

TEST_CASE("Circle valid", "[Circle]") {
    Dynamo::Circle a({0, 0}, 0);
    Dynamo::Circle b({0, 0}, 5);
    REQUIRE(a.valid());
    REQUIRE(b.valid());
}

TEST_CASE("Circle invalid", "[Circle]") {
    Dynamo::Circle a({3, 0}, -1);
    REQUIRE(!a.valid());
}

TEST_CASE("Circle hash", "[Circle]") {
    REQUIRE_NOTHROW(std::unordered_set<Dynamo::Circle>());
}
