#include <Dynamo.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cmath>

#include "Common.hpp"

TEST_CASE("Fourier transform", "[Fourier]") {
    Dynamo::ComplexSignal signal(
        {
            Dynamo::Complex(-1, 0),
            Dynamo::Complex(2, 0),
            Dynamo::Complex(3, 0),
            Dynamo::Complex(0, 0),

            Dynamo::Complex(-3, 0),
            Dynamo::Complex(1, 0),
            Dynamo::Complex(0, 0),
            Dynamo::Complex(2, 0),
        },
        2);
    Dynamo::ComplexSignal freqs(signal.channels(), signal.frames());

    Dynamo::fourier_transform(signal, freqs, signal.frames());
    Dynamo::Complex a0 = freqs.at(0, 0);
    Dynamo::Complex b0 = freqs.at(1, 0);
    Dynamo::Complex c0 = freqs.at(2, 0);
    Dynamo::Complex d0 = freqs.at(3, 0);

    Dynamo::Complex a1 = freqs.at(0, 1);
    Dynamo::Complex b1 = freqs.at(1, 1);
    Dynamo::Complex c1 = freqs.at(2, 1);
    Dynamo::Complex d1 = freqs.at(3, 1);

    // First channel
    REQUIRE_THAT(a0.re, Approx(4));
    REQUIRE_THAT(a0.im, Approx(0));

    REQUIRE_THAT(b0.re, Approx(-4));
    REQUIRE_THAT(b0.im, Approx(-2));

    REQUIRE_THAT(c0.re, Approx(0));
    REQUIRE_THAT(c0.im, Approx(0));

    REQUIRE_THAT(d0.re, Approx(-4));
    REQUIRE_THAT(d0.im, Approx(2));

    // Second channel
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
    Dynamo::ComplexSignal freqs(
        {
            Dynamo::Complex(4, 0),
            Dynamo::Complex(-4, -2),
            Dynamo::Complex(0, 0),
            Dynamo::Complex(-4, 2),

            Dynamo::Complex(0, 0),
            Dynamo::Complex(-3, 1),
            Dynamo::Complex(-6, 0),
            Dynamo::Complex(-3, -1),
        },
        2);
    Dynamo::ComplexSignal signal(freqs.channels(), freqs.frames());

    Dynamo::inverse_fourier_transform(freqs, signal, freqs.frames());
    Dynamo::Complex a0 = signal.at(0, 0);
    Dynamo::Complex b0 = signal.at(1, 0);
    Dynamo::Complex c0 = signal.at(2, 0);
    Dynamo::Complex d0 = signal.at(3, 0);

    Dynamo::Complex a1 = signal.at(0, 1);
    Dynamo::Complex b1 = signal.at(1, 1);
    Dynamo::Complex c1 = signal.at(2, 1);
    Dynamo::Complex d1 = signal.at(3, 1);

    // First channel
    REQUIRE_THAT(a0.re, Approx(-1));
    REQUIRE_THAT(a0.im, Approx(0));

    REQUIRE_THAT(b0.re, Approx(2));
    REQUIRE_THAT(b0.im, Approx(0));

    REQUIRE_THAT(c0.re, Approx(3));
    REQUIRE_THAT(c0.im, Approx(0));

    REQUIRE_THAT(d0.re, Approx(0));
    REQUIRE_THAT(d0.im, Approx(0));

    // Second channel
    REQUIRE_THAT(a1.re, Approx(-3));
    REQUIRE_THAT(a1.im, Approx(0));

    REQUIRE_THAT(b1.re, Approx(1));
    REQUIRE_THAT(b1.im, Approx(0));

    REQUIRE_THAT(c1.re, Approx(0));
    REQUIRE_THAT(c1.im, Approx(0));

    REQUIRE_THAT(d1.re, Approx(2));
    REQUIRE_THAT(d1.im, Approx(0));
}