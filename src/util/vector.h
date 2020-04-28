#ifndef DYNAMO_VECTOR_H_
#define DYNAMO_VECTOR_H_

#include <SDL2/SDL.h>

#include <cmath>

#include "matrix.h"

namespace Dynamo {
    struct Vec2D {
        float x;
        float y;

        float length_squared();
        float length();

        Vec2D left_normal();
        Vec2D right_normal();

        Vec2D operator+(const Vec2D &rhs) const;
        Vec2D operator-(const Vec2D &rhs) const;
        Vec2D operator*(float scalar) const;
        Vec2D operator/(float scalar) const;
        Vec2D operator-() const; // Negation

        Vec2D &operator+=(const Vec2D &rhs);
        Vec2D &operator-=(const Vec2D &rhs);
        Vec2D &operator*=(float scalar);
        Vec2D &operator/=(float scalar);

        Vec2D &normalize();
        Vec2D &transform(const Mat2D &mat);

        float operator*(const Vec2D &rhs) const; // Dot product
        float cross(const Vec2D &rhs) const; // "Cross-product"

        bool operator==(const Vec2D &rhs) const;
        bool operator!=(const Vec2D &rhs) const;

        SDL_Point convert_to_point();
    };
}

#endif