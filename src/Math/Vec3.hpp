#pragma once

#include <cmath>
#include <functional>

#include "../Types.hpp"
#include "Matrix.hpp"

namespace Dynamo {
    /**
     * @brief 3D vector.
     *
     */
    struct Vec3 {
        f32 x, y, z;

        /**
         * @brief Construct a new Vec3 object.
         *
         * @param x
         * @param y
         * @param z
         */
        constexpr Vec3(f32 x = 0, f32 y = 0, f32 z = 0) : x(x), y(y), z(z) {}

        /**
         * @brief Calculate the squared length.
         *
         * @return f32
         */
        inline f32 length_squared() const { return x * x + y * y + z * z; }

        /**
         * @brief Calculate the length.
         *
         * @return f32
         */
        inline f32 length() const { return std::sqrt(length_squared()); }

        /**
         * @brief Add another vector.
         *
         * @param rhs
         * @return Vec3
         */
        inline Vec3 operator+(const Vec3 &rhs) const {
            return Vec3(x + rhs.x, y + rhs.y, z + rhs.z);
        }

        /**
         * @brief Subtract another vector.
         *
         * @param rhs
         * @return Vec3
         */
        inline Vec3 operator-(const Vec3 &rhs) const {
            return Vec3(x - rhs.x, y - rhs.y, z - rhs.z);
        }

        /**
         * @brief Multiply by a scalar.
         *
         * @param scalar
         * @return Vec3
         */
        inline Vec3 operator*(f32 scalar) const {
            return Vec3(x * scalar, y * scalar, z * scalar);
        }

        /**
         * @brief Divide by a scalar.
         *
         * @param scalar
         * @return Vec3
         */
        inline Vec3 operator/(f32 scalar) const {
            f32 inv = 1.0f / scalar;
            return *this * inv;
        }

        /**
         * @brief Negation.
         *
         * @return Vec3
         */
        inline Vec3 operator-() const { return Vec3(-x, -y, -z); }

        /**
         * @brief Add another vector in-place.
         *
         * @param rhs
         * @return Vec3&
         */
        inline Vec3 &operator+=(const Vec3 &rhs) {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }

        /**
         * @brief Subtract another vector in-place.
         *
         * @param rhs
         * @return Vec3&
         */
        inline Vec3 &operator-=(const Vec3 &rhs) {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            return *this;
        }

        /**
         * @brief Multiply by a scalar in-place.
         *
         * @param scalar
         * @return Vec3&
         */
        inline Vec3 &operator*=(f32 scalar) {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        /**
         * @brief Divide by a scalar in-place.
         *
         * @param scalar
         * @return Vec3&
         */
        inline Vec3 &operator/=(f32 scalar) {
            f32 inv = 1.0f / scalar;
            return *this *= inv;
        }

        /**
         * @brief Normalize this vector in-place.
         *
         * @return Vec3&
         */
        inline Vec3 &normalize() { return *this /= length(); }

        /**
         * @brief Apply a transform with a 4x4 matrix.
         *
         * @param mat
         * @return Vec3&
         */
        inline Vec3 &transform(const Mat4 &mat) {
            const auto &vals = mat.values;
            f32 w =
                1.0 / (vals[12] * x + vals[13] * y + vals[14] * z + vals[15]);
            x = (vals[0] * x + vals[1] * y + vals[2] * z + vals[3]) * w;
            y = (vals[4] * x + vals[5] * y + vals[6] * z + vals[7]) * w;
            y = (vals[8] * x + vals[9] * y + vals[10] * z + vals[11]) * w;
            return *this;
        }

        /**
         * @brief Dot product.
         *
         * @param rhs
         * @return f32
         */
        inline f32 operator*(const Vec3 &rhs) const {
            return x * rhs.x + y * rhs.y + z * rhs.z;
        }

        /**
         * @brief 3D cross product.
         *
         * @param rhs
         * @return Vec3
         */
        inline Vec3 cross(const Vec3 &rhs) const {
            f32 n_x = y * rhs.z - z * rhs.y;
            f32 n_y = z * rhs.x - x * rhs.z;
            f32 n_z = x * rhs.y - y * rhs.x;
            return Vec3(n_x, n_y, n_z);
        }

        /**
         * @brief Equality operator.
         *
         * @param rhs
         * @return true
         * @return false
         */
        inline b8 operator==(const Vec3 &rhs) const {
            return x == rhs.x && y == rhs.y && z == rhs.z;
        }

        /**
         * @brief Inequality operator.
         *
         * @param rhs
         * @return true
         * @return false
         */
        inline b8 operator!=(const Vec3 &rhs) const { return !(*this == rhs); }
    };
} // namespace Dynamo

/**
 * @brief Hash function implementation for Vec3.
 *
 * @tparam
 */
template <>
struct std::hash<Dynamo::Vec3> {
    inline size_t operator()(const Dynamo::Vec3 &point) const {
        Dynamo::i64 tx = point.x * 73856093;
        Dynamo::i64 ty = point.y * 19349663;
        Dynamo::i64 tz = point.z * 83492791;
        return tx ^ ty ^ tz;
    }
};