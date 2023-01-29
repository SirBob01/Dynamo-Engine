#pragma once

#include <array>

#include "../Types.hpp"

namespace Dynamo {
    /**
     * @brief A square matrix
     *
     * @tparam N Number of rows or columns
     */
    template <u32 N>
    struct Matrix {
        static constexpr u32 N2 = N * N;
        std::array<f32, N2> values;

        /**
         * @brief Construct a new Matrix object
         *
         */
        constexpr Matrix() {
            for (u32 i = 0; i < N; i++) {
                for (u32 j = 0; j < N; j++) {
                    values[i * N + j] = (i == j);
                }
            }
        }

        /**
         * @brief Construct a new Matrix object
         *
         */
        constexpr Matrix(f32 v) {
            for (u32 i = 0; i < N2; i++) {
                values[i] = v;
            }
        }

        /**
         * @brief Negation
         *
         * @return Matrix<N>
         */
        inline Matrix<N> operator-() {
            Matrix<N> result;
            for (u32 i = 0; i < N2; i++) {
                result.values[i] = -values[i];
            }
            return result;
        }

        /**
         * @brief Add another matrix
         *
         * @param rhs
         * @return Matrix<N>
         */
        inline Matrix<N> operator+(const Matrix<N> &rhs) {
            Matrix<N> result;
            for (u32 i = 0; i < N2; i++) {
                result.values[i] = values[i] + rhs.values[i];
            }
            return result;
        }

        /**
         * @brief Subtract another matrix
         *
         * @param rhs
         * @return Matrix<N>
         */
        inline Matrix<N> operator-(const Matrix<N> &rhs) {
            Matrix<N> result;
            for (u32 i = 0; i < N2; i++) {
                result.values[i] = values[i] - rhs.values[i];
            }
            return result;
        }

        /**
         * @brief Multiply with another matrix
         *
         * @param rhs
         * @return Matrix<N>
         */
        inline Matrix<N> operator*(const Matrix<N> &rhs) {
            Matrix<N> result;
            for (u32 i = 0; i < N; i++) {
                for (u32 j = 0; j < N; j++) {
                    f32 dot = 0;
                    for (u32 c = 0; c < N; c++) {
                        dot += values[i * N + c] * rhs.values[c * N + j];
                    }
                    result.values[i * N + j] = dot;
                }
            }
            return result;
        }

        /**
         * @brief Multiply by a scalar
         *
         * @param rhs
         * @return Matrix<N>
         */
        inline Matrix<N> operator*(f32 rhs) {
            Matrix<N> result;
            for (u32 i = 0; i < N2; i++) {
                result.values[i] = values[i] * rhs;
            }
            return result;
        }

        /**
         * @brief Divide by a scalar
         *
         * @param rhs
         * @return Matrix<N>
         */
        inline Matrix<N> operator/(f32 rhs) {
            f32 inv = 1.0 / rhs;
            return (*this) * inv;
        }

        /**
         * @brief Add another matrix in-place
         *
         * @param rhs
         * @return Matrix<N>
         */
        inline Matrix<N> &operator+=(const Matrix<N> &rhs) {
            for (u32 i = 0; i < N2; i++) {
                values[i] += rhs.values[i];
            }
            return *this;
        }

        /**
         * @brief Subtract another matrix in-place
         *
         * @param rhs
         * @return Matrix<N>
         */
        inline Matrix<N> &operator-=(const Matrix<N> &rhs) {
            for (u32 i = 0; i < N2; i++) {
                values[i] -= rhs.values[i];
            }
            return *this;
        }

        /**
         * @brief Multiply with another matrix in-place
         *
         * @param rhs
         * @return Matrix<N>
         */
        inline Matrix<N> &operator*=(const Matrix<N> &rhs) {
            Matrix<N> result;
            for (u32 i = 0; i < N; i++) {
                for (u32 j = 0; j < N; j++) {
                    f32 dot = 0;
                    for (u32 c = 0; c < N; c++) {
                        dot += values[i * N + c] * rhs.values[c * N + j];
                    }
                    result.values[i * N + j] = dot;
                }
            }
            values = result.values;
            return *this;
        }

        /**
         * @brief Multiply by a scalar in-place
         *
         * @param rhs
         * @return Matrix<N>
         */
        inline Matrix<N> operator*=(f32 rhs) {
            for (u32 i = 0; i < N2; i++) {
                values[i] *= rhs;
            }
            return *this;
        }

        /**
         * @brief Divide by a scalar in-place
         *
         * @param rhs
         * @return Matrix<N>
         */
        inline Matrix<N> operator/=(f32 rhs) {
            f32 inv = 1.0 / rhs;
            return (*this) *= inv;
        }

        /**
         * @brief Swap rows and columns
         *
         * @return Matrix<N>&
         */
        inline Matrix<N> &transpose() {
            for (i32 i = 0; i < N; i++) {
                for (i32 j = 0; j < N; j++) {
                    std::swap(values[i + j * N], values[j + i * N]);
                }
            }
            return *this;
        }

        /**
         * @brief Equality operator
         *
         * @param rhs
         * @return true
         * @return false
         */
        inline b8 operator==(const Matrix<N> &rhs) const {
            for (u32 i = 0; i < N2; i++) {
                if (values[i] != rhs.values[i]) {
                    return false;
                }
            }
            return true;
        }

        /**
         * @brief Inequality operator
         *
         * @param rhs
         * @return true
         * @return false
         */
        inline b8 operator!=(const Matrix<N> &rhs) const {
            return !(*this == rhs);
        }
    };

    /**
     * @brief 3x3 matrix
     *
     */
    using Mat3 = Matrix<3>;

    /**
     * @brief 4x4 matrix
     *
     */
    using Mat4 = Matrix<4>;
} // namespace Dynamo