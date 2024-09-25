#pragma once

#include <cmath>
#include <functional>

#include <Math/Vec3.hpp>

namespace Dynamo {
    /**
     * @brief Quaternion expression for 3D orientations.
     *
     */
    struct Quaternion {
        float x, y, z, w;

        /**
         * @brief Construct a new Quaternion object.
         *
         * @param x Coefficient of i component.
         * @param y Coefficient of j component.
         * @param z Coefficient of k component.
         * @param w Scalar component.
         */
        constexpr Quaternion(float x = 0,
                             float y = 0,
                             float z = 0,
                             float w = 1) :
            x(x),
            y(y), z(z), w(w) {}

        /**
         * @brief Construct a new Quaternion object from an angle-axis pair.
         *
         * @param axis  Rotation axis.
         * @param angle Rotation angle in radians.
         */
        Quaternion(Vec3 axis, float angle) {
            float half_angle = 0.5 * angle;
            float sine = std::sin(half_angle);
            x = axis.x * sine;
            y = axis.y * sine;
            z = axis.z * sine;
            w = std::cos(half_angle);
        }

        /**
         * @brief Calculate the square length.
         *
         * @return float
         */
        inline float length_squared() const {
            return x * x + y * y + z * z + w * w;
        }

        /**
         * @brief Calculate the length.
         *
         * @return float
         */
        inline float length() const { return std::sqrt(length_squared()); }

        /**
         * @brief Calculate the reciprocal of the quaternion for use in
         * divisions.
         *
         * @return Quaternion
         */
        inline Quaternion reciprocal() const {
            float scale = 1.0 / length_squared();
            return Quaternion(-x * scale, -y * scale, -z * scale, w * scale);
        }

        /**
         * @brief Calculate the conjugate of the quaternion.
         *
         * @return Quaternion
         */
        inline Quaternion conjugate() const {
            return Quaternion(-x, -y, -z, w);
        }

        /**
         * @brief Negation.
         *
         * @return Quaternion
         */
        inline Quaternion operator-() const {
            return Quaternion(-x, -y, -z, -w);
        }

        /**
         * @brief Add another quaternion.
         *
         * @param rhs
         * @return Quaternion
         */
        inline Quaternion operator+(const Quaternion &rhs) const {
            return Quaternion(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
        }

        /**
         * @brief Subtract another quaternion.
         *
         * @param rhs
         * @return Quaternion
         */
        inline Quaternion operator-(const Quaternion &rhs) const {
            return Quaternion(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
        }

        /**
         * @brief Multiply by another quaternion.
         *
         * This operation composes rotations and is non-commutative.
         *
         * @param rhs
         * @return Quaternion
         */
        inline Quaternion operator*(const Quaternion &rhs) const {
            float n_x = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
            float n_y = w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x;
            float n_z = w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w;
            float n_w = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
            return Quaternion(n_x, n_y, n_z, n_w);
        }

        /**
         * @brief Multiply by a scalar.
         *
         * @param scalar
         * @return Quaternion
         */
        inline Quaternion operator*(float scalar) const {
            return Quaternion(scalar * x, scalar * y, scalar * z, scalar * w);
        }

        /**
         * @brief Divide by a scalar.
         *
         * @param scalar
         * @return Quaternion
         */
        inline Quaternion operator/(float scalar) const {
            float inv = 1.0 / scalar;
            return Quaternion(inv * x, inv * y, inv * z, inv * w);
        }

        /**
         * @brief Add another quaternion in-place.
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
         * @brief Subtract another quaternion in-place.
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
         * @brief Multiply by another quaternion in-place.
         *
         * This operation composes rotations and is non-commutative.
         *
         * @param rhs
         * @return Quaternion
         */
        inline Quaternion &operator*=(const Quaternion &rhs) {
            float n_x = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
            float n_y = w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x;
            float n_z = w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w;
            float n_w = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
            x = n_x;
            y = n_y;
            z = n_z;
            w = n_w;
            return *this;
        }

        /**
         * @brief Multiply by a scalar in-place.
         *
         * @param scalar
         * @return Quaternion
         */
        inline Quaternion &operator*=(float scalar) {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            w *= scalar;
            return *this;
        }

        /**
         * @brief Divide by a scalar in-place.
         *
         * @param scalar
         * @return Quaternion
         */
        inline Quaternion &operator/=(float scalar) {
            float inv = 1.0 / scalar;
            return *this *= inv;
        }

        /**
         * @brief Normalize this quaternion in-place.
         *
         * @return Quaternion&
         */
        inline Quaternion &normalize() { return *this /= length(); }

        /**
         * @brief Apply rotation to a Vec3.
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
         * @brief Extract the forward vector of the rotation basis.
         *
         * @return Vec3
         */
        inline Vec3 forward() const {
            float n_x = 2 * (x * z + w * y);
            float n_y = 2 * (y * z - w * x);
            float n_z = 1 - 2 * (x * x + y * y);
            return Vec3(n_x, n_y, n_z);
        }

        /**
         * @brief Extract the up vector of the rotation basis.
         *
         * @return Vec3
         */
        inline Vec3 up() const {
            float n_x = 2 * (x * y - w * z);
            float n_y = 1 - 2 * (x * x + z * z);
            float n_z = 2 * (y * z + w * x);
            return Vec3(n_x, n_y, n_z);
        }

        /**
         * @brief Extract the right vector of the rotation basis.
         *
         * @return Vec3
         */
        inline Vec3 right() const {
            float n_x = 1 - 2 * (y * y + z * z);
            float n_y = 2 * (x * y + w * z);
            float n_z = 2 * (x * z - w * y);
            return Vec3(n_x, n_y, n_z);
        }

        /**
         * @brief Equality operator.
         *
         * @param rhs
         * @return true
         * @return false
         */
        inline bool operator==(const Quaternion &rhs) const {
            return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
        }

        /**
         * @brief Inequality operator.
         *
         * @param rhs
         * @return true
         * @return false
         */
        inline bool operator!=(const Quaternion &rhs) const {
            return !(*this == rhs);
        }
    };
} // namespace Dynamo

/**
 * @brief Hash function implementation for Quaternion.
 *
 * @tparam
 */
template <>
struct std::hash<Dynamo::Quaternion> {
    inline size_t operator()(const Dynamo::Quaternion &quaternion) const {
        long long tx = quaternion.x * 73856093;
        long long ty = quaternion.y * 19349663;
        long long tz = quaternion.z * 83492791;
        long long tw = quaternion.w * 52477425;
        return tx ^ ty ^ tz ^ tw;
    }
};
