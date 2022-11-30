#include <Dynamo.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cmath>

#include "Common.hpp"

TEST_CASE("Quaternion axis-angle", "[Quaternion]") {
    Dynamo::Quaternion z({1, 0, 0}, M_PI_4);
    REQUIRE_THAT(z.x, Approx(std::sin(M_PI_4 * 0.5)));
    REQUIRE_THAT(z.y, Approx(0));
    REQUIRE_THAT(z.z, Approx(0));
    REQUIRE_THAT(z.w, Approx(std::cos(M_PI_4 * 0.5)));
}

TEST_CASE("Quaternion clone", "[Quaternion]") {
    Dynamo::Quaternion a(1, 2, 3, 4);
    Dynamo::Quaternion b = a.clone();
    REQUIRE(a.x == b.x);
    REQUIRE(a.y == b.y);
    REQUIRE(a.z == b.z);
    REQUIRE(a.w == b.w);
}

TEST_CASE("Quaternion length squared", "[Quaternion]") {
    Dynamo::Quaternion z(1, 2, 3, 4);
    REQUIRE(z.length_squared() == 30);
}

TEST_CASE("Quaternion length", "[Quaternion]") {
    Dynamo::Quaternion z(1, 2, 3, 4);
    REQUIRE_THAT(z.length(), Approx((float)std::sqrt(30)));
}

TEST_CASE("Quaternion reciprocal", "[Quaternion]") {
    Dynamo::Quaternion z(1, 2, 3, 4);
    Dynamo::Quaternion reciprocal = z.reciprocal();
    REQUIRE_THAT(reciprocal.x, Approx(-1.0 / 30.0));
    REQUIRE_THAT(reciprocal.y, Approx(-2.0 / 30.0));
    REQUIRE_THAT(reciprocal.z, Approx(-3.0 / 30.0));
    REQUIRE_THAT(reciprocal.w, Approx(4.0 / 30.0));
}

TEST_CASE("Quaternion conjugate", "[Quaternion]") {
    Dynamo::Quaternion z(1, 2, 3, 4);
    Dynamo::Quaternion conjugate = z.conjugate();
    REQUIRE(conjugate.x == -1.0);
    REQUIRE(conjugate.y == -2.0);
    REQUIRE(conjugate.z == -3.0);
    REQUIRE(conjugate.w == 4.0);
}

TEST_CASE("Quaternion negation", "[Quaternion]") {
    Dynamo::Quaternion a(1, 2, 3, 4);
    Dynamo::Quaternion b = -a;
    REQUIRE(b.x == -1);
    REQUIRE(b.y == -2);
    REQUIRE(b.z == -3);
    REQUIRE(b.w == -4);
}

TEST_CASE("Quaternion add", "[Quaternion]") {
    Dynamo::Quaternion a(1, 2, 3, 4);
    Dynamo::Quaternion b(5, 6, 7, 8);
    Dynamo::Quaternion c = a + b;
    REQUIRE(c.x == 6);
    REQUIRE(c.y == 8);
    REQUIRE(c.z == 10);
    REQUIRE(c.w == 12);
}

TEST_CASE("Quaternion subtract", "[Quaternion]") {
    Dynamo::Quaternion a(1, 2, 3, 4);
    Dynamo::Quaternion b(5, 6, 7, 8);
    Dynamo::Quaternion c = a - b;
    REQUIRE(c.x == -4);
    REQUIRE(c.y == -4);
    REQUIRE(c.z == -4);
    REQUIRE(c.w == -4);
}

TEST_CASE("Quaternion multiply", "[Quaternion]") {
    Dynamo::Quaternion a(1, 2, 3, 4);
    Dynamo::Quaternion b(5, 6, 7, 8);
    Dynamo::Quaternion c = a * b;
    REQUIRE(c.x == 24);
    REQUIRE(c.y == 48);
    REQUIRE(c.z == 48);
    REQUIRE(c.w == -6);
}

TEST_CASE("Quaternion multiply scalar", "[Quaternion]") {
    Dynamo::Quaternion a(1, 2, 3, 4);
    Dynamo::Quaternion c = a * 3;
    REQUIRE(c.x == 3.0);
    REQUIRE(c.y == 6.0);
    REQUIRE(c.z == 9.0);
    REQUIRE(c.w == 12.0);
}

TEST_CASE("Quaternion divide scalar", "[Quaternion]") {
    Dynamo::Quaternion a(3, 6, 9, 12);
    Dynamo::Quaternion c = a / 3;
    REQUIRE(c.x == 1);
    REQUIRE(c.y == 2);
    REQUIRE(c.z == 3);
    REQUIRE(c.w == 4);
}

TEST_CASE("Quaternion add in-place", "[Quaternion]") {
    Dynamo::Quaternion a(1, 2, 3, 4);
    Dynamo::Quaternion b(5, 6, 7, 8);
    a += b;
    REQUIRE(a.x == 6);
    REQUIRE(a.y == 8);
    REQUIRE(a.z == 10);
    REQUIRE(a.w == 12);
}

TEST_CASE("Quaternion subtract in-place", "[Quaternion]") {
    Dynamo::Quaternion a(1, 2, 3, 4);
    Dynamo::Quaternion b(5, 6, 7, 8);
    a -= b;
    REQUIRE(a.x == -4);
    REQUIRE(a.y == -4);
    REQUIRE(a.z == -4);
    REQUIRE(a.w == -4);
}

TEST_CASE("Quaternion multiply in-place", "[Quaternion]") {
    Dynamo::Quaternion a(1, 2, 3, 4);
    Dynamo::Quaternion b(5, 6, 7, 8);
    a *= b;
    REQUIRE(a.x == 24);
    REQUIRE(a.y == 48);
    REQUIRE(a.z == 48);
    REQUIRE(a.w == -6);
}

TEST_CASE("Quaternion multiply scalar in-place", "[Quaternion]") {
    Dynamo::Quaternion a(1, 2, 3, 4);
    a *= 3;
    REQUIRE(a.x == 3.0);
    REQUIRE(a.y == 6.0);
    REQUIRE(a.z == 9.0);
    REQUIRE(a.w == 12.0);
}

TEST_CASE("Quaternion divide scalar in-place", "[Quaternion]") {
    Dynamo::Quaternion a(3, 6, 9, 12);
    a /= 3;
    REQUIRE(a.x == 1);
    REQUIRE(a.y == 2);
    REQUIRE(a.z == 3);
    REQUIRE(a.w == 4);
}

TEST_CASE("Quaternion rotate 90 along x-axis", "[Quaternion]") {
    Dynamo::Quaternion r({1, 0, 0}, M_PI_2);
    Dynamo::Vec3 v(0, 1, 0);
    Dynamo::Vec3 t = r.rotate(v);
    REQUIRE_THAT(t.x, Approx(0));
    REQUIRE_THAT(t.y, Approx(0));
    REQUIRE_THAT(t.z, Approx(1.0));
}

TEST_CASE("Quaternion rotate 45 along y-axis", "[Quaternion]") {
    Dynamo::Quaternion r({0, 1, 0}, M_PI_4);
    Dynamo::Vec3 v(1, 0, 0);
    Dynamo::Vec3 t = r.rotate(v);
    REQUIRE_THAT(t.x, Approx(M_SQRT1_2));
    REQUIRE_THAT(t.y, Approx(0));
    REQUIRE_THAT(t.z, Approx(-M_SQRT1_2));
}

TEST_CASE("Quaternion normalize", "[Quaternion]") {
    Dynamo::Quaternion z(1, 2, 3, 4);
    z.normalize();
    REQUIRE_THAT(z.x, Approx(1.0 / std::sqrt(30.0)));
    REQUIRE_THAT(z.y, Approx(2.0 / std::sqrt(30.0)));
    REQUIRE_THAT(z.z, Approx(3.0 / std::sqrt(30.0)));
    REQUIRE_THAT(z.w, Approx(4.0 / std::sqrt(30.0)));
}

TEST_CASE("Quaternion forward", "[Quaternion]") {
    Dynamo::Quaternion r({0, 1, 0}, M_PI_4);
    Dynamo::Vec3 forward = r.forward();
    REQUIRE_THAT(forward.x, Approx(M_SQRT1_2));
    REQUIRE_THAT(forward.y, Approx(0));
    REQUIRE_THAT(forward.z, Approx(M_SQRT1_2));
}

TEST_CASE("Quaternion up", "[Quaternion]") {
    Dynamo::Quaternion r({0, 1, 0}, M_PI_4);
    Dynamo::Vec3 forward = r.up();
    REQUIRE_THAT(forward.x, Approx(0));
    REQUIRE_THAT(forward.y, Approx(1));
    REQUIRE_THAT(forward.z, Approx(0));
}

TEST_CASE("Quaternion right", "[Quaternion]") {
    Dynamo::Quaternion r({0, 1, 0}, M_PI_4);
    Dynamo::Vec3 forward = r.right();
    REQUIRE_THAT(forward.x, Approx(M_SQRT1_2));
    REQUIRE_THAT(forward.y, Approx(0));
    REQUIRE_THAT(forward.z, Approx(-M_SQRT1_2));
}

TEST_CASE("Quaternion equality", "[Quaternion]") {
    Dynamo::Quaternion a(1, 2, 3, 4);
    Dynamo::Quaternion b(1, 2, 3, 4);
    REQUIRE(a == b);
}

TEST_CASE("Quaternion inequality", "[Quaternion]") {
    Dynamo::Quaternion a(1, 2, 3, 4);
    Dynamo::Quaternion b(3, 2, 1, 0);
    Dynamo::Quaternion c(1, 4, 7, 9);
    Dynamo::Quaternion d = b + c;
    REQUIRE(a != b);
    REQUIRE(a != c);
    REQUIRE(a != d);
}