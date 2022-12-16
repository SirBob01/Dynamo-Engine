#include <Dynamo.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cmath>

#include "Common.hpp"

using ComplexChannel = std::vector<Dynamo::Complex>;

TEST_CASE("Fourier transform", "[Fourier]") {
    // First channel
    ComplexChannel signal0 = {
        Dynamo::Complex(-1, 0),
        Dynamo::Complex(2, 0),
        Dynamo::Complex(3, 0),
        Dynamo::Complex(0, 0),
    };
    Dynamo::Fourier::transform(signal0.data(), signal0.size());
    Dynamo::Complex a0 = signal0[0];
    Dynamo::Complex b0 = signal0[1];
    Dynamo::Complex c0 = signal0[2];
    Dynamo::Complex d0 = signal0[3];

    REQUIRE_THAT(a0.re, Approx(4));
    REQUIRE_THAT(a0.im, Approx(0));

    REQUIRE_THAT(b0.re, Approx(-4));
    REQUIRE_THAT(b0.im, Approx(-2));

    REQUIRE_THAT(c0.re, Approx(0));
    REQUIRE_THAT(c0.im, Approx(0));

    REQUIRE_THAT(d0.re, Approx(-4));
    REQUIRE_THAT(d0.im, Approx(2));

    // Second channel
    ComplexChannel signal1 = {
        Dynamo::Complex(-3, 0),
        Dynamo::Complex(1, 0),
        Dynamo::Complex(0, 0),
        Dynamo::Complex(2, 0),
    };
    Dynamo::Fourier::transform(signal1.data(), signal1.size());
    Dynamo::Complex a1 = signal1[0];
    Dynamo::Complex b1 = signal1[1];
    Dynamo::Complex c1 = signal1[2];
    Dynamo::Complex d1 = signal1[3];

    REQUIRE_THAT(a1.re, Approx(0));
    REQUIRE_THAT(a1.im, Approx(0));

    REQUIRE_THAT(b1.re, Approx(-3));
    REQUIRE_THAT(b1.im, Approx(1));

    REQUIRE_THAT(c1.re, Approx(-6));
    REQUIRE_THAT(c1.im, Approx(0));

    REQUIRE_THAT(d1.re, Approx(-3));
    REQUIRE_THAT(d1.im, Approx(-1));
}

TEST_CASE("Inverse Fourier transform", "[Fourier]") {
    // First channel
    ComplexChannel signal0 = {
        Dynamo::Complex(4, 0),
        Dynamo::Complex(-4, -2),
        Dynamo::Complex(0, 0),
        Dynamo::Complex(-4, 2),
    };
    Dynamo::Fourier::inverse(signal0.data(), signal0.size());
    Dynamo::Complex a0 = signal0[0];
    Dynamo::Complex b0 = signal0[1];
    Dynamo::Complex c0 = signal0[2];
    Dynamo::Complex d0 = signal0[3];

    REQUIRE_THAT(a0.re, Approx(-1));
    REQUIRE_THAT(a0.im, Approx(0));

    REQUIRE_THAT(b0.re, Approx(2));
    REQUIRE_THAT(b0.im, Approx(0));

    REQUIRE_THAT(c0.re, Approx(3));
    REQUIRE_THAT(c0.im, Approx(0));

    REQUIRE_THAT(d0.re, Approx(0));
    REQUIRE_THAT(d0.im, Approx(0));

    // Second channel
    ComplexChannel signal1 = {
        Dynamo::Complex(0, 0),
        Dynamo::Complex(-3, 1),
        Dynamo::Complex(-6, 0),
        Dynamo::Complex(-3, -1),
    };
    Dynamo::Fourier::inverse(signal1.data(), signal1.size());
    Dynamo::Complex a1 = signal1[0];
    Dynamo::Complex b1 = signal1[1];
    Dynamo::Complex c1 = signal1[2];
    Dynamo::Complex d1 = signal1[3];

    REQUIRE_THAT(a1.re, Approx(-3));
    REQUIRE_THAT(a1.im, Approx(0));

    REQUIRE_THAT(b1.re, Approx(1));
    REQUIRE_THAT(b1.im, Approx(0));

    REQUIRE_THAT(c1.re, Approx(0));
    REQUIRE_THAT(c1.im, Approx(0));

    REQUIRE_THAT(d1.re, Approx(2));
    REQUIRE_THAT(d1.im, Approx(0));
}
