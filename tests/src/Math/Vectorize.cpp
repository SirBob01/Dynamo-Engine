#include <Dynamo.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Vectorize Instruction Set", "[Vectorize]") {
#if defined(DYNAMO_ARCH_NEON)
    Dynamo::Log::info("Vectorize Neon");
#elif defined(DYNAMO_ARCH_AVX)
    Dynamo::Log::info("Vectorize AVX");
#elif defined(DYNAMO_ARCH_SSE)
    Dynamo::Log::info("Vectorize SSE");
#else
    Dynamo::Log::info("Vectorize SIMD not available");
#endif
}

TEST_CASE("Vectorize smul", "[Vectorize]") {
    float a[] = {0, 1, 2, 3, 4};
    float b = 2;
    float c[] = {1, 4, 7, 9, 0};

    Dynamo::Vectorize::smul(a, b, c, 5);

    REQUIRE(c[0] == 0);
    REQUIRE(c[1] == 2);
    REQUIRE(c[2] == 4);
    REQUIRE(c[3] == 6);
    REQUIRE(c[4] == 8);
}

TEST_CASE("Vectorize vadd", "[Vectorize]") {
    float a[] = {0, 1, 2, 3, 4};
    float b[] = {1, 2, 3, 4, 5};
    float c[] = {0, 0, 0, 0, 0};

    Dynamo::Vectorize::vadd(a, b, c, 5);

    REQUIRE(c[0] == 1);
    REQUIRE(c[1] == 3);
    REQUIRE(c[2] == 5);
    REQUIRE(c[3] == 7);
    REQUIRE(c[4] == 9);
}

TEST_CASE("Vectorize vsma", "[Vectorize]") {
    float a[] = {0, 1, 2, 3, 4};
    float b = 2;
    float c[] = {1, 4, 7, 9, 0};

    Dynamo::Vectorize::vsma(a, b, c, 5);

    REQUIRE(c[0] == 1);
    REQUIRE(c[1] == 6);
    REQUIRE(c[2] == 11);
    REQUIRE(c[3] == 15);
    REQUIRE(c[4] == 8);
}

TEST_CASE("Vectorize vclip", "[Vectorize]") {
    float a[] = {-2, 0, 1, 4, 0.5};
    float lo = -1;
    float hi = 1;
    float c[] = {0, 0, 0, 0, 0};

    Dynamo::Vectorize::vclip(a, lo, hi, c, 5);

    REQUIRE(c[0] == -1);
    REQUIRE(c[1] == 0);
    REQUIRE(c[2] == 1);
    REQUIRE(c[3] == 1);
    REQUIRE(c[4] == 0.5);
}