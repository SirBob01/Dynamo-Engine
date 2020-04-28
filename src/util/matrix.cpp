#include "matrix.h"

namespace Dynamo {
    float Mat2D::determinant() const {
        return a * d - b * c;
    }

    Mat2D Mat2D::operator+(const Mat2D &rhs) const {
        return {a+rhs.a, b+rhs.b, 
                c+rhs.c, d+rhs.d};
    }

    Mat2D Mat2D::operator-(const Mat2D &rhs) const {
        return {a-rhs.a, b-rhs.b, 
                c-rhs.c, d-rhs.d};
    }

    Mat2D Mat2D::operator*(const Mat2D &rhs) const {
        return {a*rhs.a + b*rhs.c, a*rhs.b + b*rhs.d,
                c*rhs.a + d*rhs.c, c*rhs.b + d*rhs.d};
    }

    Mat2D Mat2D::operator*(float scalar) const {
        return {a * scalar, b * scalar,
                c * scalar, d * scalar};
    }

    Mat2D Mat2D::operator/(float scalar) const {
        return {a / scalar, b / scalar,
                c / scalar, d / scalar};
    }

    Mat2D Mat2D::operator-() const {
        return {-a, -b, -c, -d};
    };

    Mat2D Mat2D::inverse() const {
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
}