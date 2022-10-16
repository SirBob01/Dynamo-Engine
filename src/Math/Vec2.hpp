#pragma once

#include <cmath>

#include "Mat3.hpp"

namespace Dynamo {
    /**
     * @brief Vector in 2-dimensions
     *
     */
    struct Vec2 {
        float x;
        float y;

        /**
         * @brief Construct a new Vec2 object
         *
         * @param x
         * @param y
         */
        Vec2(float x = 0, float y = 0) : x(x), y(y) {}

        /**
         * @brief Clone this vector
         *
         * @return Vec2
         */
        inline Vec2 clone() { return Vec2(x, y); }

        /**
         * @brief Calculate the squared length
         *
         * @return float
         */
        inline float length_squared() const { return x * x + y * y; }

        /**
         * @brief Calculate the length
         *
         * @return float
         */
        inline float length() const { return sqrt(length_squared()); }

        /**
         * @brief Get the left normal perpendicular to this vector
         *
         * @return Vec2
         */
        inline Vec2 left_normal() const { return Vec2(-y, x); }

        /**
         * @brief Get the right normal perpendicular to this vector
         *
         * @return Vec2
         */
        inline Vec2 right_normal() const { return Vec2(y, -x); }

        /**
         * @brief Add another vector
         *
         * @param rhs
         * @return Vec2
         */
        inline Vec2 operator+(const Vec2 &rhs) const {
            return Vec2(x + rhs.x, y + rhs.y);
        }

        /**
         * @brief Subtract another vector
         *
         * @param rhs
         * @return Vec2
         */
        inline Vec2 operator-(const Vec2 &rhs) const {
            return Vec2(x - rhs.x, y - rhs.y);
        }

        /**
         * @brief Multiply by a scalar
         *
         * @param scalar
         * @return Vec2
         */
        inline Vec2 operator*(float scalar) const {
            return Vec2(x * scalar, y * scalar);
        }

        /**
         * @brief Divide by a scalar
         *
         * @param scalar
         * @return Vec2
         */
        inline Vec2 operator/(float scalar) const {
            return Vec2(x / scalar, y / scalar);
        }

        /**
         * @brief Negation
         *
         * @return Vec2
         */
        inline Vec2 operator-() const { return Vec2(-x, -y); }

        /**
         * @brief Add another vector in-place
         *
         * @param rhs
         * @return Vec2&
         */
        inline Vec2 &operator+=(const Vec2 &rhs) {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }

        /**
         * @brief Subtract another vector in-place
         *
         * @param rhs
         * @return Vec2&
         */
        inline Vec2 &operator-=(const Vec2 &rhs) {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }

        /**
         * @brief Multiply by a scalar in-place
         *
         * @param scalar
         * @return Vec2&
         */
        inline Vec2 &operator*=(float scalar) {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        /**
         * @brief Divide by a scalar in-place
         *
         * @param scalar
         * @return Vec2&
         */
        inline Vec2 &operator/=(float scalar) {
            x /= scalar;
            y /= scalar;
            return *this;
        }

        /**
         * @brief Normalize this vector in-place
         *
         * @return Vec2&
         */
        inline Vec2 &normalize() {
            const float magnitude = length();
            x /= magnitude;
            y /= magnitude;
            return *this;
        }

        /**
         * @brief Apply a transform with a 3x3 matrix
         *
         * @param mat
         * @return Vec2&
         */
        inline Vec2 &transform(const Mat3 &mat) {
            float w = mat.g * x + mat.h * y + mat.i;
            x = (mat.a * x + mat.b * y + mat.c) / w;
            y = (mat.d * x + mat.e * y + mat.f) / w;
            return *this;
        }

        /**
         * @brief Dot product
         *
         * @param rhs
         * @return float
         */
        inline float operator*(const Vec2 &rhs) const {
            return x * rhs.x + y * rhs.y;
        }

        /**
         * @brief 2D cross product
         *
         * @param rhs
         * @return float
         */
        inline float cross(const Vec2 &rhs) const {
            return x * rhs.y - y * rhs.x;
        }

        /**
         * @brief Equality operator
         *
         * @param rhs
         * @return true
         * @return false
         */
        inline bool operator==(const Vec2 &rhs) const {
            return x == rhs.x && y == rhs.y;
        }

        /**
         * @brief Inequality operator
         *
         * @param rhs
         * @return true
         * @return false
         */
        inline bool operator!=(const Vec2 &rhs) const {
            return !(*this == rhs);
        }
    };
} // namespace Dynamo