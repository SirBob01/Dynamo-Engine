#include "vec2d.h"

namespace Dynamo::Physics {
    float Mat2D::determinant() const {
        return a * d - b * c;
    }

    const Mat2D Mat2D::operator+(const Mat2D &rhs) const {
        return {a+rhs.a, b+rhs.b, 
                c+rhs.c, d+rhs.d};
    }

    const Mat2D Mat2D::operator-(const Mat2D &rhs) const {
        return {a-rhs.a, b-rhs.b, 
                c-rhs.c, d-rhs.d};
    }

    const Mat2D Mat2D::operator*(const Mat2D &rhs) const {
        return {a*rhs.a + b*rhs.c, a*rhs.b + b*rhs.d,
                c*rhs.a + d*rhs.c, c*rhs.b + d*rhs.d};
    }

    const Mat2D Mat2D::operator*(float scalar) const {
        return {a * scalar, b * scalar,
                c * scalar, d * scalar};
    }

    const Mat2D Mat2D::operator/(float scalar) const {
        return {a / scalar, b / scalar,
                c / scalar, d / scalar};
    }

    const Mat2D Mat2D::operator-() const {
        return {-a, -b, -c, -d};
    };

    const Mat2D Mat2D::inverse() const {
        Mat2D inv = {d, -b, -c, a};
        return inv / determinant();
    }

    Mat2D &Mat2D::operator+=(const Mat2D &rhs) {
        a += rhs.a;
        b += rhs.b;
        c += rhs.c;
        d += rhs.d;
        return *this;
    };

    Mat2D &Mat2D::operator-=(const Mat2D &rhs) {
        a -= rhs.a;
        b -= rhs.b;
        c -= rhs.c;
        d -= rhs.d;
        return *this;
    };

    Mat2D &Mat2D::operator*=(const Mat2D &rhs) {
        float new_a = a*rhs.a + b*rhs.c;
        float new_b = a*rhs.b + b*rhs.d;
        float new_c = c*rhs.a + d*rhs.c;
        float new_d = c*rhs.b + d*rhs.d;
        a = new_a;
        b = new_b;
        c = new_c;
        d = new_d;
        return *this;
    };

    Mat2D &Mat2D::operator*=(float scalar) {
        a *= scalar;
        b *= scalar;
        c *= scalar;
        d *= scalar;
        return *this;
    }

    Mat2D &Mat2D::operator/=(float scalar) {
        a /= scalar;
        b /= scalar;
        c /= scalar;
        d /= scalar;
        return *this;
    }

    bool Mat2D::operator==(const Mat2D &rhs) const {
        return a == rhs.a && b == rhs.b && c == rhs.c && d == rhs.d;
    }

    bool Mat2D::operator!=(const Mat2D &rhs) const {
        return a != rhs.a || b != rhs.b || c != rhs.c || d != rhs.d;
    }

    float Vec2D::length_squared() {
        return pow(x, 2) + pow(y, 2);
    }

    float Vec2D::length() {
        return sqrt(length_squared());
    }

    const Vec2D Vec2D::left_normal() {
        return {-y, x};
    }

    const Vec2D Vec2D::right_normal() {
        return {y, -x};
    }

    const Vec2D Vec2D::operator+(const Vec2D &rhs) const {
        return {x + rhs.x, y + rhs.y};
    }

    const Vec2D Vec2D::operator-(const Vec2D &rhs) const {
        return {x - rhs.x, y - rhs.y};
    }

    const Vec2D Vec2D::operator*(float scalar) const {
        return {scalar * x, scalar * y};
    }

    const Vec2D Vec2D::operator/(float scalar) const {
        return {x / scalar, y / scalar};
    }

    const Vec2D Vec2D::operator-() const {
        return {-x, -y};
    }

    Vec2D &Vec2D::operator+=(const Vec2D &rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    Vec2D &Vec2D::operator-=(const Vec2D &rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    Vec2D &Vec2D::operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vec2D &Vec2D::operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    Vec2D &Vec2D::normalize() {
        float mag = length();
        if(mag != 0.0) {
            x /= mag;
            y /= mag;
        }
        return *this;
    }

    Vec2D &Vec2D::transform(const Mat2D &rhs) {
        float new_x = rhs.a*x + rhs.b*y;
        float new_y = rhs.c*x + rhs.d*y;
        x = new_x;
        y = new_y;
        return *this;
    }

    float Vec2D::operator*(const Vec2D &rhs) const {
        return x * rhs.x + y * rhs.y;
    }

    float Vec2D::cross(const Vec2D &rhs) const {
        return x * rhs.y - y * rhs.x;
    }

    bool Vec2D::operator==(const Vec2D &rhs) const {
        return (x == rhs.x && y == rhs.y);
    }

    bool Vec2D::operator!=(const Vec2D &rhs) const {
        return (x != rhs.x || y != rhs.y);
    }
}