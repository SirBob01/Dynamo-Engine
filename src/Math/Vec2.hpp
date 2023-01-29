#pragma once

#include <cmath>
#include <functional>

#include "../Types.hpp"
#include "Matrix.hpp"

namespace Dynamo {
    /**
     * @brief 2D vector
     *
     */
    struct Vec2 {
        f32 x, y;

        /**
         * @brief Construct a new Vec2 object
         *
         * @param x
         * @param y
         */
        constexpr Vec2(f32 x = 0, f32 y = 0) : x(x), y(y) {}

        /**
         * @brief Calculate the squared length
         *
         * @return f32
         */
        inline f32 length_squared() const { return x * x + y * y; }

        /**
         * @brief Calculate the length
         *
         * @return f32
         */
        inline f32 length() const { return std::sqrt(length_squared()); }

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
        inline Vec2 operator*(f32 scalar) const {
            return Vec2(x * scalar, y * scalar);
        }

        /**
         * @brief Divide by a scalar
         *
         * @param scalar
         * @return Vec2
         */
        inline Vec2 operator/(f32 scalar) const {
            const f32 inv = 1.0f / scalar;
            return *this * inv;
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
        inline Vec2 &operator*=(f32 scalar) {
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
        inline Vec2 &operator/=(f32 scalar) {
            f32 inv = 1.0 / scalar;
            return *this *= inv;
        }

        /**
         * @brief Normalize this vector in-place
         *
         * @return Vec2&
         */
        inline Vec2 &normalize() { return *this /= length(); }

        /**
         * @brief Apply a transform with a 3x3 matrix
         *
         * @param mat
         * @return Vec2&
         */
        inline Vec2 &transform(const Mat3 &mat) {
            const auto &vals = mat.values;
            f32 w = 1.0 / (vals[6] * x + vals[7] * y + vals[8]);
            x = (vals[0] * x + vals[1] * y + vals[2]) * w;
            y = (vals[3] * x + vals[4] * y + vals[5]) * w;
            return *this;
        }

        /**
         * @brief Dot product
         *
         * @param rhs
         * @return f32
         */
        inline f32 operator*(const Vec2 &rhs) const {
            return x * rhs.x + y * rhs.y;
        }

        /**
         * @brief 2D cross product
         *
         * @param rhs
         * @return f32
         */
        inline f32 cross(const Vec2 &rhs) const {
            return x * rhs.y - y * rhs.x;
        }

        /**
         * @brief Equality operator
         *
         * @param rhs
         * @return true
         * @return false
         */
        inline b8 operator==(const Vec2 &rhs) const {
            return x == rhs.x && y == rhs.y;
        }

        /**
         * @brief Inequality operator
         *
         * @param rhs
         * @return true
         * @return false
         */
        inline b8 operator!=(const Vec2 &rhs) const { return !(*this == rhs); }
    };
} // namespace Dynamo

/**
 * @brief Hash function implementation for Vec2
 *
 * @tparam
 */
template <>
struct std::hash<Dynamo::Vec2> {
    inline size_t operator()(const Dynamo::Vec2 &point) const {
        Dynamo::i64 tx = point.x * 73856093;
        Dynamo::i64 ty = point.y * 19349663;
        return tx ^ ty;
    }
};
