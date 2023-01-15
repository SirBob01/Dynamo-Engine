#include <Dynamo.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Matrix add", "[Matrix]") {
    Dynamo::Matrix<3> a;
    Dynamo::Matrix<3> b;
    Dynamo::Matrix<3> c = a + b;
    REQUIRE(c.values[0] == 2);
    REQUIRE(c.values[1] == 0);
    REQUIRE(c.values[2] == 0);
    REQUIRE(c.values[3] == 0);
    REQUIRE(c.values[4] == 2);
    REQUIRE(c.values[5] == 0);
    REQUIRE(c.values[6] == 0);
    REQUIRE(c.values[7] == 0);
    REQUIRE(c.values[8] == 2);
}

TEST_CASE("Matrix subtract", "[Matrix]") {
    Dynamo::Matrix<3> a;
    Dynamo::Matrix<3> b;
    Dynamo::Matrix<3> c = a - b;
    Dynamo::Matrix<3> d(0);
    REQUIRE(c == d);
}

TEST_CASE("Matrix multiply", "[Matrix]") {
    Dynamo::Matrix<3> i;
    Dynamo::Matrix<3> a(3);
    Dynamo::Matrix<3> b(2);
    Dynamo::Matrix<3> c = a * b;
    Dynamo::Matrix<3> d(18);
    REQUIRE(a * i == a);
    REQUIRE(b * i == b);
    REQUIRE(c == d);
}

TEST_CASE("Matrix multiply by scalar", "[Matrix]") {
    Dynamo::Matrix<3> a(2);
    Dynamo::Matrix<3> b = a * 3.2;
    Dynamo::Matrix<3> c(6.4);
    REQUIRE(b == c);
}

TEST_CASE("Matrix divide by scalar", "[Matrix]") {
    Dynamo::Matrix<3> a(6.4f);
    Dynamo::Matrix<3> b = a / 3.2f;
    Dynamo::Matrix<3> c(2);
    REQUIRE(b == c);
}

TEST_CASE("Matrix negation", "[Matrix]") {
    Dynamo::Matrix<3> a(2);
    Dynamo::Matrix<3> b = -a;
    Dynamo::Matrix<3> c(-2);
    REQUIRE(b == c);
}

TEST_CASE("Matrix add in-place", "[Matrix]") {
    Dynamo::Matrix<3> a;
    Dynamo::Matrix<3> b;
    a += b;
    REQUIRE(a.values[0] == 2);
    REQUIRE(a.values[1] == 0);
    REQUIRE(a.values[2] == 0);
    REQUIRE(a.values[3] == 0);
    REQUIRE(a.values[4] == 2);
    REQUIRE(a.values[5] == 0);
    REQUIRE(a.values[6] == 0);
    REQUIRE(a.values[7] == 0);
    REQUIRE(a.values[8] == 2);
}

TEST_CASE("Matrix subtract in-place", "[Matrix]") {
    Dynamo::Matrix<3> a;
    Dynamo::Matrix<3> b;
    Dynamo::Matrix<3> d(0);
    a -= b;
    REQUIRE(a == d);
}

TEST_CASE("Matrix multiply in-place", "[Matrix]") {
    Dynamo::Matrix<3> a(3);
    Dynamo::Matrix<3> b(2);
    Dynamo::Matrix<3> d(18);
    a *= b;
    REQUIRE(a == d);
}

TEST_CASE("Matrix multiply by scalar in-place", "[Matrix]") {
    Dynamo::Matrix<3> a(2);
    Dynamo::Matrix<3> b(6.4);
    a *= 3.2;
    REQUIRE(a == b);
}

TEST_CASE("Matrix divide by scalar in-place", "[Matrix]") {
    Dynamo::Matrix<3> a(6.4f);
    Dynamo::Matrix<3> b(2);
    a /= 3.2f;
    REQUIRE(a == b);
}

TEST_CASE("Matrix equality", "[Matrix]") {
    Dynamo::Matrix<3> a(-3.2);
    Dynamo::Matrix<3> b(-3.2);
    REQUIRE(a == b);
}

TEST_CASE("Matrix inequality", "[Matrix]") {
    Dynamo::Matrix<3> a;
    Dynamo::Matrix<3> b(2);
    Dynamo::Matrix<3> c(4);
    Dynamo::Matrix<3> d = b + c;
    REQUIRE(a != b);
    REQUIRE(a != c);
    REQUIRE(a != d);
}
