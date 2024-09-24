#include <Dynamo.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("TypeId unique assignment", "[TypeId]") {
    REQUIRE(Dynamo::TypeId::get<int>() == 0);
    REQUIRE(Dynamo::TypeId::get<char>() == 1);
    REQUIRE(Dynamo::TypeId::get<int>() == 0);
    REQUIRE(Dynamo::TypeId::get<Dynamo::Vec2>() == 2);
    REQUIRE(Dynamo::TypeId::get<float>() == 3);
    REQUIRE(Dynamo::TypeId::get<float>() == 3);
}
