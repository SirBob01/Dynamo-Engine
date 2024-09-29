#include <Dynamo.hpp>
#include <catch2/catch_test_macros.hpp>

#if defined(DYNAMO_ARCH_SSE)
#include <Math/Arch/SSE.hpp>

TEST_CASE("Vectorize SSE smul", "[Vectorize]") {
    float a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    float b = 2;
    float c[] = {1, 4, 7, 9, 0, 3, 2, 5, 2, 4};

    Dynamo::Vectorize::SSE::smul(a, b, c, 10);

    REQUIRE(c[0] == 0);
    REQUIRE(c[1] == 2);
    REQUIRE(c[2] == 4);
    REQUIRE(c[3] == 6);
    REQUIRE(c[4] == 8);
    REQUIRE(c[5] == 10);
    REQUIRE(c[6] == 12);
    REQUIRE(c[7] == 14);
    REQUIRE(c[8] == 16);
    REQUIRE(c[9] == 18);
}

TEST_CASE("Vectorize SSE vadd", "[Vectorize]") {
    float a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    float b[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    float c[] = {1, 4, 7, 9, 0, 3, 2, 5, 2, 4};

    Dynamo::Vectorize::SSE::vadd(a, b, c, 10);

    REQUIRE(c[0] == 1);
    REQUIRE(c[1] == 3);
    REQUIRE(c[2] == 5);
    REQUIRE(c[3] == 7);
    REQUIRE(c[4] == 9);
    REQUIRE(c[5] == 11);
    REQUIRE(c[6] == 13);
    REQUIRE(c[7] == 15);
    REQUIRE(c[8] == 17);
    REQUIRE(c[9] == 19);
}

TEST_CASE("Vectorize SSE vsma", "[Vectorize]") {
    float a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    float b = 2;
    float c[] = {1, 4, 7, 9, 0, 3, 2, 5, 2, 4};

    Dynamo::Vectorize::SSE::vsma(a, b, c, 10);

    REQUIRE(c[0] == 1);
    REQUIRE(c[1] == 6);
    REQUIRE(c[2] == 11);
    REQUIRE(c[3] == 15);
    REQUIRE(c[4] == 8);
    REQUIRE(c[5] == 13);
    REQUIRE(c[6] == 14);
    REQUIRE(c[7] == 19);
    REQUIRE(c[8] == 18);
    REQUIRE(c[9] == 22);
}

TEST_CASE("Vectorize SSE vclamp", "[Vectorize]") {
    float a[] = {-2, 0, 1, 4, 0.5, 1, 5, -0.5, -3, -0.25};
    float lo = -1;
    float hi = 1;
    float c[] = {1, 4, 7, 9, 0, 3, 2, 5, 2, 4};

    Dynamo::Vectorize::SSE::vclamp(a, lo, hi, c, 10);

    REQUIRE(c[0] == -1);
    REQUIRE(c[1] == 0);
    REQUIRE(c[2] == 1);
    REQUIRE(c[3] == 1);
    REQUIRE(c[4] == 0.5);
    REQUIRE(c[5] == 1);
    REQUIRE(c[6] == 1);
    REQUIRE(c[7] == -0.5);
    REQUIRE(c[8] == -1);
    REQUIRE(c[9] == -0.25);
}
#else
TEST_CASE("Vectorize SSE null", "[Vectorize]") {
    Dynamo::Log::info("SSE instruction set not supported.");
}
#endif