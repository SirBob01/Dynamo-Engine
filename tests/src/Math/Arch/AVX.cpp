#include <Dynamo.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#if defined(DYNAMO_ARCH_AVX)
#include <Math/Arch/AVX.hpp>

#include "Common.hpp"

TEST_CASE("Vectorize AVX smul", "[Vectorize]") {
    FloatArray src;
    FloatArray dst;
    float scalar = 0.8;
    fill_array(src);
    fill_array(dst);

    BENCHMARK("Vectorize AVX smul benchmark") { Dynamo::Vectorize::AVX::smul(src.data(), scalar, dst.data(), LENGTH); };

    for (unsigned i = 0; i < LENGTH; i++) {
        REQUIRE(dst[i] == scalar * src[i]);
    }
}

TEST_CASE("Vectorize AVX vadd", "[Vectorize]") {
    FloatArray src_a;
    FloatArray src_b;
    FloatArray dst;
    fill_array(src_a);
    fill_array(src_b);
    fill_array(dst);

    BENCHMARK("Vectorize AVX vadd benchmark") {
        Dynamo::Vectorize::AVX::vadd(src_a.data(), src_b.data(), dst.data(), LENGTH);
    };

    for (unsigned i = 0; i < LENGTH; i++) {
        REQUIRE(dst[i] == src_a[i] + src_b[i]);
    }
}

TEST_CASE("Vectorize AVX vsub", "[Vectorize]") {
    FloatArray src_a;
    FloatArray src_b;
    FloatArray dst;
    fill_array(src_a);
    fill_array(src_b);
    fill_array(dst);

    BENCHMARK("Vectorize AVX vsub benchmark") {
        Dynamo::Vectorize::AVX::vsub(src_a.data(), src_b.data(), dst.data(), LENGTH);
    };

    for (unsigned i = 0; i < LENGTH; i++) {
        REQUIRE(dst[i] == src_a[i] - src_b[i]);
    }
}

TEST_CASE("Vectorize AVX vsma", "[Vectorize]") {
    FloatArray src;
    FloatArray dst;
    FloatArray prev_dst;
    float scalar = 0.8;
    fill_array(src);
    fill_array(dst);

    BENCHMARK("Vectorize AVX vsma benchmark") {
        std::copy(dst.begin(), dst.end(), prev_dst.begin());
        Dynamo::Vectorize::AVX::vsma(src.data(), scalar, dst.data(), LENGTH);
    };

    for (unsigned i = 0; i < LENGTH; i++) {
        REQUIRE(dst[i] == prev_dst[i] + src[i] * scalar);
    }
}

TEST_CASE("Vectorize AVX vclamp", "[Vectorize]") {
    FloatArray src;
    FloatArray dst;
    float lo = -1;
    float hi = 1;
    fill_array(src);
    fill_array(dst);

    BENCHMARK("Vectorize AVX clamp benchmark") {
        Dynamo::Vectorize::AVX::vclamp(src.data(), lo, hi, dst.data(), LENGTH);
    };

    for (unsigned i = 0; i < LENGTH; i++) {
        REQUIRE(dst[i] == std::clamp(src[i], lo, hi));
    }
}

#else
TEST_CASE("Vectorize AVX null", "[Vectorize]") { Dynamo::Log::info("AVX instruction set not supported."); }
#endif