#include <Dynamo.hpp>
#include <catch2/catch_test_macros.hpp>

DYN_DEFINE_ID_TYPE(Id_0);
DYN_DEFINE_ID_TYPE(Id_1);

TEST_CASE("IdGenerator generate", "[IdGenerator]") {
    Id_0 a0 = Dynamo::IdGenerator<Id_0>::generate();
    Id_0 b0 = Dynamo::IdGenerator<Id_0>::generate();

    Id_1 a1 = Dynamo::IdGenerator<Id_1>::generate();
    Id_1 b1 = Dynamo::IdGenerator<Id_1>::generate();

    REQUIRE(Dynamo::IdGenerator<Id_0>::key(a0) == 0);
    REQUIRE(Dynamo::IdGenerator<Id_0>::key(b0) == 1);

    REQUIRE(Dynamo::IdGenerator<Id_1>::key(a1) == 0);
    REQUIRE(Dynamo::IdGenerator<Id_1>::key(b1) == 1);
}
