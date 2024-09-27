#include <Math/Arch/Scalar.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Vectorize Scalar smul", "[Vectorize]") {
    float a[] = {0, 1, 2, 3, 4};
    float b = 2;
    float c[] = {1, 4, 7, 9, 0};

    Dynamo::Vectorize::Scalar::smul(a, b, c, 5);

    REQUIRE(c[0] == 0);
    REQUIRE(c[1] == 2);
    REQUIRE(c[2] == 4);
    REQUIRE(c[3] == 6);
    REQUIRE(c[4] == 8);
}

TEST_CASE("Vectorize Scalar vadd", "[Vectorize]") {
    float a[] = {0, 1, 2, 3, 4};
    float b[] = {1, 2, 3, 4, 5};
    float c[] = {0, 0, 0, 0, 0};

    Dynamo::Vectorize::Scalar::vadd(a, b, c, 5);

    REQUIRE(c[0] == 1);
    REQUIRE(c[1] == 3);
    REQUIRE(c[2] == 5);
    REQUIRE(c[3] == 7);
    REQUIRE(c[4] == 9);
}

TEST_CASE("Vectorize Scalar vsma", "[Vectorize]") {
    float a[] = {0, 1, 2, 3, 4};
    float b = 2;
    float c[] = {1, 4, 7, 9, 0};

    Dynamo::Vectorize::Scalar::vsma(a, b, c, 5);

    REQUIRE(c[0] == 1);
    REQUIRE(c[1] == 6);
    REQUIRE(c[2] == 11);
    REQUIRE(c[3] == 15);
    REQUIRE(c[4] == 8);
}
