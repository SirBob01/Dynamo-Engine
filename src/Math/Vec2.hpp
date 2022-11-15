#pragma once

#include <cmath>

#include "Mat3.hpp"

namespace Dynamo {
    /**
     * @brief Vector in 2-dimensions
     *
     */
    struct Vec2 {
        float x, y;

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
        inline float length() const { return std::sqrt(length_squared()); }

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
            const float inv = 1.0f / scalar;
            return Vec2(x * inv, y * inv);
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
            float inv = 1.0 / scalar;
            x *= inv;
            y *= inv;
            return *this;
        }

        /**
         * @brief Normalize this vector in-place
         *
         * @return Vec2&
         */
        inline Vec2 &normalize() {
            const float inv_magnitude = 1.0 / length();
            x *= inv_magnitude;
            y *= inv_magnitude;
            return *this;
        }

        /**
         * @brief Apply a transform with a 3x3 matrix
         *
         * @param mat
         * @return Vec2&
         */
        inline Vec2 &transform(const Mat3 &mat) {
            const auto &vals = mat.values;
            float w = 1.0 / (vals[6] * x + vals[7] * y + vals[8]);
            x = (vals[0] * x + vals[1] * y + vals[2]) * w;
            y = (vals[3] * x + vals[4] * y + vals[5]) * w;
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