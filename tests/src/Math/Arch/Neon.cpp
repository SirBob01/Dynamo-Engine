#include <Dynamo.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#if defined(DYNAMO_ARCH_NEON)
#include <Math/Arch/Neon.hpp>

#include "Common.hpp"

TEST_CASE("Vectorize Neon smul", "[Vectorize]") {
    FloatArray src;
    FloatArray dst;
    float scalar = 0.8;
    fill_array(src);
    fill_array(dst);

    BENCHMARK("Vectorize Neon smul benchmark") {
        Dynamo::Vectorize::Neon::smul(src.data(), scalar, dst.data(), LENGTH);
    };

    for (unsigned i = 0; i < LENGTH; i++) {
        REQUIRE(dst[i] == scalar * src[i]);
    }
}

TEST_CASE("Vectorize Neon vadd", "[Vectorize]") {
    FloatArray src_a;
    FloatArray src_b;
    FloatArray dst;
    fill_array(src_a);
    fill_array(src_b);
    fill_array(dst);

    BENCHMARK("Vectorize Neon vadd benchmark") {
        Dynamo::Vectorize::Neon::vadd(src_a.data(),
                                      src_b.data(),
                                      dst.data(),
                                      LENGTH);
    };

    for (unsigned i = 0; i < LENGTH; i++) {
        REQUIRE(dst[i] == src_a[i] + src_b[i]);
    }
}

TEST_CASE("Vectorize Neon vsub", "[Vectorize]") {
    FloatArray src_a;
    FloatArray src_b;
    FloatArray dst;
    fill_array(src_a);
    fill_array(src_b);
    fill_array(dst);

    BENCHMARK("Vectorize Neon vsub benchmark") {
        Dynamo::Vectorize::Neon::vsub(src_a.data(),
                                      src_b.data(),
                                      dst.data(),
                                      LENGTH);
    };

    for (unsigned i = 0; i < LENGTH; i++) {
        REQUIRE(dst[i] == src_a[i] - src_b[i]);
    }
}

TEST_CASE("Vectorize Neon vsma", "[Vectorize]") {
    FloatArray src;
    FloatArray dst;
    FloatArray prev_dst;
    float scalar = 0.8;
    fill_array(src);
    fill_array(dst);

    BENCHMARK("Vectorize Neon vsma benchmark") {
        std::copy(dst.begin(), dst.end(), prev_dst.begin());
        Dynamo::Vectorize::Neon::vsma(src.data(), scalar, dst.data(), LENGTH);
    };

    for (unsigned i = 0; i < LENGTH; i++) {
        REQUIRE(dst[i] == prev_dst[i] + src[i] * scalar);
    }
}

TEST_CASE("Vectorize Neon vclamp", "[Vectorize]") {
    FloatArray src;
    FloatArray dst;
    float lo = -1;
    float hi = 1;
    fill_array(src);
    fill_array(dst);

    BENCHMARK("Vectorize Neon clamp benchmark") {
        Dynamo::Vectorize::Neon::vclamp(src.data(), lo, hi, dst.data(), LENGTH);
    };

    for (unsigned i = 0; i < LENGTH; i++) {
        REQUIRE(dst[i] == std::clamp(src[i], lo, hi));
    }
}

#else
TEST_CASE("Vectorize Neon null", "[Vectorize]") {
    Dynamo::Log::info("Neon instruction set not supported.");
}
#endif