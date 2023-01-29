#pragma once

#include <cmath>
#include <functional>

#include "../Types.hpp"
#include "./Vec3.hpp"

namespace Dynamo {
    /**
     * @brief Quaternion expression
     *
     * xi + yj + zk + w, such that i^2 = j^2 = k^2 = ijk = -1
     *
     */
    struct Quaternion {
        f32 x, y, z, w;

        /**
         * @brief Construct a new Quaternion object
         *
         * @param x Coefficient of i component
         * @param y Coefficient of j component
         * @param z Coefficient of k component
         * @param w Scalar component
         */
        constexpr Quaternion(f32 x = 0, f32 y = 0, f32 z = 0, f32 w = 1) :
            x(x), y(y), z(z), w(w) {}

        /**
         * @brief Construct a new Quaternion object from an angle-axis pair
         *
         * @param axis  Rotation axis
         * @param angle Rotation angle in radians
         */
        Quaternion(Vec3 axis, f32 angle) {
            f32 half_angle = 0.5 * angle;
            f32 sine = std::sin(half_angle);
            x = axis.x * sine;
            y = axis.y * sine;
            z = axis.z * sine;
            w = std::cos(half_angle);
        }

        /**
         * @brief Calculate the square length
         *
         * @return f32
         */
        inline f32 length_squared() const {
            return x * x + y * y + z * z + w * w;
        }

        /**
         * @brief Calculate the length
         *
         * @return f32
         */
        inline f32 length() const { return std::sqrt(length_squared()); }

        /**
         * @brief Calculate the reciprocal of the quaternion for use in
         * divisions
         *
         * @return Quaternion
         */
        inline Quaternion reciprocal() const {
            f32 scale = 1.0 / length_squared();
            return Quaternion(-x * scale, -y * scale, -z * scale, w * scale);
        }

        /**
         * @brief Calculate the conjugate of the quaternion
         *
         * @return Quaternion
         */
        inline Quaternion conjugate() const {
            return Quaternion(-x, -y, -z, w);
        }

        /**
         * @brief Negation
         *
         * @return Quaternion
         */
        inline Quaternion operator-() const {
            return Quaternion(-x, -y, -z, -w);
        }

        /**
         * @brief Add another quaternion
         *
         * @param rhs
         * @return Quaternion
         */
        inline Quaternion operator+(const Quaternion &rhs) const {
            return Quaternion(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
        }

        /**
         * @brief Subtract another quaternion
         *
         * @param rhs
         * @return Quaternion
         */
        inline Quaternion operator-(const Quaternion &rhs) const {
            return Quaternion(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
        }

        /**
         * @brief Multiply by another quaternion
         *
         * This operation composes rotations and is non-commutative
         *
         * @param rhs
         * @return Quaternion
         */
        inline Quaternion operator*(const Quaternion &rhs) const {
            f32 n_x = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
            f32 n_y = w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x;
            f32 n_z = w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w;
            f32 n_w = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
            return Quaternion(n_x, n_y, n_z, n_w);
        }

        /**
         * @brief Multiply by a scalar
         *
         * @param scalar
         * @return Quaternion
         */
        inline Quaternion operator*(f32 scalar) const {
            return Quaternion(scalar * x, scalar * y, scalar * z, scalar * w);
        }

        /**
         * @brief Divide by a scalar
         *
         * @param scalar
         * @return Quaternion
         */
        inline Quaternion operator/(f32 scalar) const {
            f32 inv = 1.0 / scalar;
            return Quaternion(inv * x, inv * y, inv * z, inv * w);
        }

        /**
         * @brief Add another quaternion in-place
         *
         * @param rhs
         * @return Quaternion
         */
        inline Quaternion &operator+=(const Quaternion &rhs) {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            w += rhs.w;
            return *this;
        }

        /**
         * @brief Subtract another quaternion in-place
         *
         * @param rhs
         * @return Quaternion
         */
        inline Quaternion &operator-=(const Quaternion &rhs) {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            w -= rhs.w;
            return *this;
        }

        /**
         * @brief Multiply by another quaternion in-place
         *
         * This operation composes rotations and is non-commutative
         *
         * @param rhs
         * @return Quaternion
         */
        inline Quaternion &operator*=(const Quaternion &rhs) {
            f32 n_x = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
            f32 n_y = w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x;
            f32 n_z = w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w;
            f32 n_w = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
            x = n_x;
            y = n_y;
            z = n_z;
            w = n_w;
            return *this;
        }

        /**
         * @brief Multiply by a scalar in-place
         *
         * @param scalar
         * @return Quaternion
         */
        inline Quaternion &operator*=(f32 scalar) {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            w *= scalar;
            return *this;
        }

        /**
         * @brief Divide by a scalar in-place
         *
         * @param scalar
         * @return Quaternion
         */
        inline Quaternion &operator/=(f32 scalar) {
            f32 inv = 1.0 / scalar;
            return *this *= inv;
        }

        /**
         * @brief Normalize this quaternion in-place
         *
         * @return Quaternion&
         */
        inline Quaternion &normalize() { return *this /= length(); }

        /**
         * @brief Apply rotation to a Vec3
         *
         * @param v
         * @return Vec3
         */
        inline Vec3 rotate(const Vec3 &v) const {
            Vec3 u(x, y, z);
            return (u * 2 * (u * v)) + (v * (w * w - (u * u))) +
                   (u.cross(v) * 2 * w);
        }

        /**
         * @brief Extract the forward vector of the rotation basis
         *
         * @return Vec3
         */
        inline Vec3 forward() const {
            f32 n_x = 2 * (x * z + w * y);
            f32 n_y = 2 * (y * z - w * x);
            f32 n_z = 1 - 2 * (x * x + y * y);
            return Vec3(n_x, n_y, n_z);
        }

        /**
         * @brief Extract the up vector of the rotation basis
         *
         * @return Vec3
         */
        inline Vec3 up() const {
            f32 n_x = 2 * (x * y - w * z);
            f32 n_y = 1 - 2 * (x * x + z * z);
            f32 n_z = 2 * (y * z + w * x);
            return Vec3(n_x, n_y, n_z);
        }

        /**
         * @brief Extract the right vector of the rotation basis
         *
         * @return Vec3
         */
        inline Vec3 right() const {
            f32 n_x = 1 - 2 * (y * y + z * z);
            f32 n_y = 2 * (x * y + w * z);
            f32 n_z = 2 * (x * z - w * y);
            return Vec3(n_x, n_y, n_z);
        }

        /**
         * @brief Equality operator
         *
         * @param rhs
         * @return true
         * @return false
         */
        inline b8 operator==(const Quaternion &rhs) const {
            return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
        }

        /**
         * @brief Inequality operator
         *
         * @param rhs
         * @return true
         * @return false
         */
        inline b8 operator!=(const Quaternion &rhs) const {
            return !(*this == rhs);
        }
    };
} // namespace Dynamo

/**
 * @brief Hash function implementation for Quaternion
 *
 * @tparam
 */
template <>
struct std::hash<Dynamo::Quaternion> {
    inline size_t operator()(const Dynamo::Quaternion &quaternion) const {
        Dynamo::i64 tx = quaternion.x * 73856093;
        Dynamo::i64 ty = quaternion.y * 19349663;
        Dynamo::i64 tz = quaternion.z * 83492791;
        Dynamo::i64 tw = quaternion.w * 52477425;
        return tx ^ ty ^ tz ^ tw;
    }
};
