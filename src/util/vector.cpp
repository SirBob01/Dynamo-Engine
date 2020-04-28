#include "vector.h"

namespace Dynamo {
    float Vec2D::length_squared() {
        return pow(x, 2) + pow(y, 2);
    }

    float Vec2D::length() {
        return sqrt(length_squared());
    }

    Vec2D Vec2D::left_normal() {
        return {-y, x};
    }

    Vec2D Vec2D::right_normal() {
        return {y, -x};
    }

    Vec2D Vec2D::operator+(const Vec2D &rhs) const {
        return {x + rhs.x, y + rhs.y};
    }

    Vec2D Vec2D::operator-(const Vec2D &rhs) const {
        return {x - rhs.x, y - rhs.y};
    }

    Vec2D Vec2D::operator*(float scalar) const {
        return {scalar * x, scalar * y};
    }

    Vec2D Vec2D::operator/(float scalar) const {
        return {x / scalar, y / scalar};
    }

    Vec2D Vec2D::operator-() const {
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

    SDL_Point Vec2D::convert_to_point() {
        return {
            static_cast<int>(std::round(x)), 
            static_cast<int>(std::round(y))
        };
    }
}