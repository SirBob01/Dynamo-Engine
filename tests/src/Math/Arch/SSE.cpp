#include <Dynamo.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#if defined(DYNAMO_ARCH_SSE)
#include <Math/Arch/SSE.hpp>

#include "Common.hpp"

TEST_CASE("Vectorize SSE smul", "[Vectorize]") {
    FloatArray src;
    FloatArray dst;
    float scalar = 0.8;
    fill_array(src);
    fill_array(dst);

    BENCHMARK("Vectorize SSE smul benchmark") {
        Dynamo::Vectorize::SSE::smul(src.data(), scalar, dst.data(), LENGTH);
    };

    for (unsigned i = 0; i < LENGTH; i++) {
        REQUIRE(dst[i] == scalar * src[i]);
    }
}

TEST_CASE("Vectorize SSE vadd", "[Vectorize]") {
    FloatArray src_a;
    FloatArray src_b;
    FloatArray dst;
    fill_array(src_a);
    fill_array(src_b);
    fill_array(dst);

    BENCHMARK("Vectorize SSE vadd benchmark") {
        Dynamo::Vectorize::SSE::vadd(src_a.data(),
                                     src_b.data(),
                                     dst.data(),
                                     LENGTH);
    };

    for (unsigned i = 0; i < LENGTH; i++) {
        REQUIRE(dst[i] == src_a[i] + src_b[i]);
    }
}

TEST_CASE("Vectorize SSE vsub", "[Vectorize]") {
    FloatArray src_a;
    FloatArray src_b;
    FloatArray dst;
    fill_array(src_a);
    fill_array(src_b);
    fill_array(dst);

    BENCHMARK("Vectorize SSE vsub benchmark") {
        Dynamo::Vectorize::SSE::vsub(src_a.data(),
                                     src_b.data(),
                                     dst.data(),
                                     LENGTH);
    };

    for (unsigned i = 0; i < LENGTH; i++) {
        REQUIRE(dst[i] == src_a[i] - src_b[i]);
    }
}

TEST_CASE("Vectorize SSE vsma", "[Vectorize]") {
    FloatArray src;
    FloatArray dst;
    FloatArray prev_dst;
    float scalar = 0.8;
    fill_array(src);
    fill_array(dst);

    BENCHMARK("Vectorize SSE vsma benchmark") {
        std::copy(dst.begin(), dst.end(), prev_dst.begin());
        Dynamo::Vectorize::SSE::vsma(src.data(), scalar, dst.data(), LENGTH);
    };

    for (unsigned i = 0; i < LENGTH; i++) {
        REQUIRE(dst[i] == prev_dst[i] + src[i] * scalar);
    }
}

TEST_CASE("Vectorize SSE vclamp", "[Vectorize]") {
    FloatArray src;
    FloatArray dst;
    float lo = -1;
    float hi = 1;
    fill_array(src);
    fill_array(dst);

    BENCHMARK("Vectorize SSE clamp benchmark") {
        Dynamo::Vectorize::SSE::vclamp(src.data(), lo, hi, dst.data(), LENGTH);
    };

    for (unsigned i = 0; i < LENGTH; i++) {
        REQUIRE(dst[i] == std::clamp(src[i], lo, hi));
    }
}

#else
TEST_CASE("Vectorize SSE null", "[Vectorize]") {
    Dynamo::Log::info("SSE instruction set not supported.");
}
#endif