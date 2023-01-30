#include <Dynamo.hpp>
#include <catch2/catch_test_macros.hpp>

using QT = Dynamo::QuadTree<Dynamo::Vec2, 5, 5>;

TEST_CASE("QuadTree insertion", "[QuadTree]") {
    Dynamo::Vec2 a(1, 3);
    QT t({0, 0}, {128, 128});
    t.insert(a);

    REQUIRE(t.contains(a));
}
