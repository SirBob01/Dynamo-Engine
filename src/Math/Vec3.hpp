#pragma once

#include <cmath>

#include "Mat4.hpp"

namespace Dynamo {
    /**
     * @brief Vector in 3-dimensions
     *
     */
    struct Vec3 {
        float x, y, z;

        /**
         * @brief Construct a new Vec3 object
         *
         * @param x
         * @param y
         * @param z
         */
        Vec3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

        /**
         * @brief Clone this vector
         *
         * @return Vec3
         */
        inline Vec3 clone() { return Vec3(x, y, z); }

        /**
         * @brief Calculate the squared length
         *
         * @return float
         */
        inline float length_squared() const { return x * x + y * y + z * z; }

        /**
         * @brief Calculate the length
         *
         * @return float
         */
        inline float length() const { return sqrt(length_squared()); }

        /**
         * @brief Add another vector
         *
         * @param rhs
         * @return Vec3
         */
        inline Vec3 operator+(const Vec3 &rhs) const {
            return Vec3(x + rhs.x, y + rhs.y, z + rhs.z);
        }

        /**
         * @brief Subtract another vector
         *
         * @param rhs
         * @return Vec3
         */
        inline Vec3 operator-(const Vec3 &rhs) const {
            return Vec3(x - rhs.x, y - rhs.y, z - rhs.z);
        }

        /**
         * @brief Multiply by a scalar
         *
         * @param scalar
         * @return Vec3
         */
        inline Vec3 operator*(float scalar) const {
            return Vec3(x * scalar, y * scalar, z * scalar);
        }

        /**
         * @brief Divide by a scalar
         *
         * @param scalar
         * @return Vec3
         */
        inline Vec3 operator/(float scalar) const {
            return Vec3(x / scalar, y / scalar, z / scalar);
        }

        /**
         * @brief Negation
         *
         * @return Vec3
         */
        inline Vec3 operator-() const { return Vec3(-x, -y, -z); }

        /**
         * @brief Add another vector in-place
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
         * @brief Subtract another vector in-place
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
         * @brief Multiply by a scalar in-place
         *
         * @param scalar
         * @return Vec3&
         */
        inline Vec3 &operator*=(float scalar) {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        /**
         * @brief Divide by a scalar in-place
         *
         * @param scalar
         * @return Vec3&
         */
        inline Vec3 &operator/=(float scalar) {
            x /= scalar;
            y /= scalar;
            z /= scalar;
            return *this;
        }

        /**
         * @brief Normalize this vector in-place
         *
         * @return Vec3&
         */
        inline Vec3 &normalize() {
            const float magnitude = length();
            x /= magnitude;
            y /= magnitude;
            z /= magnitude;
            return *this;
        }

        /**
         * @brief Apply a transform with a 3x3 matrix
         *
         * @param mat
         * @return Vec3&
         */
        inline Vec3 &transform(const Mat4 &mat) {
            float w = 1.0 / (mat.m * x + mat.n * y + mat.o * z + mat.p);
            x = (mat.a * x + mat.b * y + mat.c * z + mat.d) * w;
            y = (mat.e * x + mat.f * y + mat.g * z + mat.h) * w;
            y = (mat.i * x + mat.j * y + mat.k * z + mat.l) * w;
            return *this;
        }

        /**
         * @brief Dot product
         *
         * @param rhs
         * @return float
         */
        inline float operator*(const Vec3 &rhs) const {
            return x * rhs.x + y * rhs.y + z * rhs.z;
        }

        /**
         * @brief 3D cross product
         *
         * @param rhs
         * @return Vec3
         */
        inline Vec3 cross(const Vec3 &rhs) const {
            float x = y * rhs.z - z * rhs.y;
            float y = z * rhs.x - x * rhs.z;
            float z = x * rhs.y - y * rhs.x;
            return Vec3(x, y, z);
        }

        /**
         * @brief Equality operator
         *
         * @param rhs
         * @return true
         * @return false
         */
        inline bool operator==(const Vec3 &rhs) const {
            return x == rhs.x && y == rhs.y && z == rhs.z;
        }

        /**
         * @brief Inequality operator
         *
         * @param rhs
         * @return true
         * @return false
         */
        inline bool operator!=(const Vec3 &rhs) const {
            return !(*this == rhs);
        }
    };
} // namespace Dynamo