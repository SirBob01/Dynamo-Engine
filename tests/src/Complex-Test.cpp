#include <Dynamo.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cmath>

#include "Common.hpp"

TEST_CASE("Complex number length squared", "[Complex]") {
    Dynamo::Complex z(3, 4);
    REQUIRE(z.length_squared() == 25);
}

TEST_CASE("Complex number length", "[Complex]") {
    Dynamo::Complex z(3, 4);
    REQUIRE(z.length() == 5);
}

TEST_CASE("Complex number reciprocal", "[Complex]") {
    Dynamo::Complex z(3, 4);
    Dynamo::Complex reciprocal = z.reciprocal();
    REQUIRE(reciprocal.re == 3 / 25.0f);
    REQUIRE(reciprocal.im == -4 / 25.0f);
}

TEST_CASE("Complex number conjugate", "[Complex]") {
    Dynamo::Complex z(3, 4);
    Dynamo::Complex conjugate = z.conjugate();
    REQUIRE(conjugate.re == 3);
    REQUIRE(conjugate.im == -4);
}

TEST_CASE("Complex number exp", "[Complex]") {
    Dynamo::Complex z(1, 0.75 * M_PI);
    Dynamo::Complex exp = z.exp();
    REQUIRE_THAT(exp.re, Approx(-M_E * M_SQRT1_2));
    REQUIRE_THAT(exp.im, Approx(M_E * M_SQRT1_2));
}

TEST_CASE("Complex number euler's identity", "[Complex]") {
    Dynamo::Complex z(0, M_PI);
    Dynamo::Complex exp = z.exp();
    REQUIRE(exp.re == -1);
    REQUIRE_THAT(exp.im, Approx(0));
}

TEST_CASE("Complex number negation", "[Complex]") {
    Dynamo::Complex a(1, 2);
    Dynamo::Complex b = -a;
    REQUIRE(b.re == -1);
    REQUIRE(b.im == -2);
}

TEST_CASE("Complex number add", "[Complex]") {
    Dynamo::Complex a(1, 2);
    Dynamo::Complex b(3, 4);
    Dynamo::Complex c = a + b;
    REQUIRE(c.re == 4);
    REQUIRE(c.im == 6);
}

TEST_CASE("Complex number subtract", "[Complex]") {
    Dynamo::Complex a(1, 2);
    Dynamo::Complex b(3, 4);
    Dynamo::Complex c = a - b;
    REQUIRE(c.re == -2);
    REQUIRE(c.im == -2);
}

TEST_CASE("Complex number multiply", "[Complex]") {
    Dynamo::Complex a(1, 2);
    Dynamo::Complex b(3, 4);
    Dynamo::Complex c = a * b;
    REQUIRE(c.re == -5);
    REQUIRE(c.im == 10);
}

TEST_CASE("Complex number multiply scalar", "[Complex]") {
    Dynamo::Complex a(-1, 2);
    Dynamo::Complex c = a * 3.2;
    REQUIRE(c.re == -3.2f);
    REQUIRE(c.im == 6.4f);
}

TEST_CASE("Complex number divide", "[Complex]") {
    Dynamo::Complex a(-5, 10);
    Dynamo::Complex b(3, 4);
    Dynamo::Complex c = a / b;
    REQUIRE_THAT(c.re, Approx(1.0f));
    REQUIRE_THAT(c.im, Approx(2.0f));
}

TEST_CASE("Complex number divide scalar", "[Complex]") {
    Dynamo::Complex a(-3.2, 6.4);
    Dynamo::Complex c = a / 3.2;
    REQUIRE(c.re == -1.0f);
    REQUIRE(c.im == 2.0f);
}

TEST_CASE("Complex number add in-place", "[Complex]") {
    Dynamo::Complex a(1, 2);
    Dynamo::Complex b(3, 4);
    a += b;
    REQUIRE(a.re == 4);
    REQUIRE(a.im == 6);
}

TEST_CASE("Complex number subtract in-place", "[Complex]") {
    Dynamo::Complex a(1, 2);
    Dynamo::Complex b(3, 4);
    a -= b;
    REQUIRE(a.re == -2);
    REQUIRE(a.im == -2);
}

TEST_CASE("Complex number multiply in-place", "[Complex]") {
    Dynamo::Complex a(1, 2);
    Dynamo::Complex b(3, 4);
    a *= b;
    REQUIRE(a.re == -5);
    REQUIRE(a.im == 10);
}

TEST_CASE("Complex number multiply scalar in-place", "[Complex]") {
    Dynamo::Complex a(-1, 2);
    a *= 3.2;
    REQUIRE(a.re == -3.2f);
    REQUIRE(a.im == 6.4f);
}

TEST_CASE("Complex number divide in-place", "[Complex]") {
    Dynamo::Complex a(-5, 10);
    Dynamo::Complex b(3, 4);
    a /= b;
    REQUIRE_THAT(a.re, Approx(1.0f));
    REQUIRE_THAT(a.im, Approx(2.0f));
}

TEST_CASE("Complex number divide scalar in-place", "[Complex]") {
    Dynamo::Complex a(-3.2, 6.4);
    a /= 3.2;
    REQUIRE(a.re == -1.0f);
    REQUIRE(a.im == 2.0f);
}

TEST_CASE("Complex number equality", "[Complex]") {
    Dynamo::Complex a(-3.2, 6.4);
    Dynamo::Complex b(-3.2, 6.4);
    REQUIRE(a == b);
}

TEST_CASE("Complex number inequality", "[Complex]") {
    Dynamo::Complex a(1, 2);
    Dynamo::Complex b(3, 2);
    Dynamo::Complex c(1, 4);
    Dynamo::Complex d = b + c;
    REQUIRE(a != b);
    REQUIRE(a != c);
    REQUIRE(a != d);
}

TEST_CASE("Complex number hash", "[Complex]") {
    REQUIRE_NOTHROW(std::unordered_set<Dynamo::Complex>());
}
