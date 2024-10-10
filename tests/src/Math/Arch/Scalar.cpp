#include <Dynamo.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#include "Common.hpp"

TEST_CASE("Vectorize Scalar smul", "[Vectorize]") {
    FloatArray src;
    FloatArray dst;
    float scalar = 0.8;
    fill_array(src);
    fill_array(dst);

    BENCHMARK("Vectorize Scalar smul benchmark") {
        Dynamo::Vectorize::Scalar::smul(src.data(), scalar, dst.data(), LENGTH);
    };

    for (unsigned i = 0; i < LENGTH; i++) {
        REQUIRE(dst[i] == scalar * src[i]);
    }
}

TEST_CASE("Vectorize Scalar vadd", "[Vectorize]") {
    FloatArray src_a;
    FloatArray src_b;
    FloatArray dst;
    fill_array(src_a);
    fill_array(src_b);
    fill_array(dst);

    BENCHMARK("Vectorize Scalar vadd benchmark") {
        Dynamo::Vectorize::Scalar::vadd(src_a.data(), src_b.data(), dst.data(), LENGTH);
    };

    for (unsigned i = 0; i < LENGTH; i++) {
        REQUIRE(dst[i] == src_a[i] + src_b[i]);
    }
}

TEST_CASE("Vectorize Scalar vsub", "[Vectorize]") {
    FloatArray src_a;
    FloatArray src_b;
    FloatArray dst;
    fill_array(src_a);
    fill_array(src_b);
    fill_array(dst);

    BENCHMARK("Vectorize Scalar vsub benchmark") {
        Dynamo::Vectorize::Scalar::vsub(src_a.data(), src_b.data(), dst.data(), LENGTH);
    };

    for (unsigned i = 0; i < LENGTH; i++) {
        REQUIRE(dst[i] == src_a[i] - src_b[i]);
    }
}

TEST_CASE("Vectorize Scalar vsma", "[Vectorize]") {
    FloatArray src;
    FloatArray dst;
    FloatArray prev_dst;
    float scalar = 0.8;
    fill_array(src);
    fill_array(dst);

    BENCHMARK("Vectorize Scalar vsma benchmark") {
        std::copy(dst.begin(), dst.end(), prev_dst.begin());
        Dynamo::Vectorize::Scalar::vsma(src.data(), scalar, dst.data(), LENGTH);
    };

    for (unsigned i = 0; i < LENGTH; i++) {
        REQUIRE(dst[i] == prev_dst[i] + src[i] * scalar);
    }
}

TEST_CASE("Vectorize Scalar vclamp", "[Vectorize]") {
    FloatArray src;
    FloatArray dst;
    float lo = -1;
    float hi = 1;
    fill_array(src);
    fill_array(dst);

    BENCHMARK("Vectorize Scalar clamp benchmark") {
        Dynamo::Vectorize::Scalar::vclamp(src.data(), lo, hi, dst.data(), LENGTH);
    };

    for (unsigned i = 0; i < LENGTH; i++) {
        REQUIRE(dst[i] == std::clamp(src[i], lo, hi));
    }
}
