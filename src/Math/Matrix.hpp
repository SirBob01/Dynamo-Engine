#pragma once

#include <array>

namespace Dynamo {
    /**
     * @brief A square matrix
     *
     * @tparam N Number of rows or columns
     */
    template <unsigned N>
    struct Matrix {
        static constexpr unsigned N2 = N * N;
        std::array<float, N2> values;

        /**
         * @brief Construct a new Matrix object
         *
         */
        constexpr Matrix() {
            for (unsigned i = 0; i < N; i++) {
                for (unsigned j = 0; j < N; j++) {
                    values[i * N + j] = (i == j);
                }
            }
        }

        /**
         * @brief Construct a new Matrix object
         *
         */
        constexpr Matrix(float v) {
            for (unsigned i = 0; i < N2; i++) {
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
            for (unsigned i = 0; i < N2; i++) {
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
            for (unsigned i = 0; i < N2; i++) {
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
            for (unsigned i = 0; i < N2; i++) {
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
            for (unsigned i = 0; i < N; i++) {
                for (unsigned j = 0; j < N; j++) {
                    float dot = 0;
                    for (unsigned c = 0; c < N; c++) {
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
        inline Matrix<N> operator*(float rhs) {
            Matrix<N> result;
            for (unsigned i = 0; i < N2; i++) {
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
        inline Matrix<N> operator/(float rhs) {
            float inv = 1.0 / rhs;
            return (*this) * inv;
        }

        /**
         * @brief Add another matrix in-place
         *
         * @param rhs
         * @return Matrix<N>
         */
        inline Matrix<N> &operator+=(const Matrix<N> &rhs) {
            for (unsigned i = 0; i < N2; i++) {
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
            for (unsigned i = 0; i < N2; i++) {
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
            for (unsigned i = 0; i < N; i++) {
                for (unsigned j = 0; j < N; j++) {
                    float dot = 0;
                    for (unsigned c = 0; c < N; c++) {
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
        inline Matrix<N> operator*=(float rhs) {
            for (unsigned i = 0; i < N2; i++) {
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
        inline Matrix<N> operator/=(float rhs) {
            float inv = 1.0 / rhs;
            return (*this) *= inv;
        }

        /**
         * @brief Swap rows and columns
         *
         * @return Matrix<N>&
         */
        inline Matrix<N> &transpose() {
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
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
        inline bool operator==(const Matrix<N> &rhs) const {
            for (unsigned i = 0; i < N2; i++) {
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
        inline bool operator!=(const Matrix<N> &rhs) const {
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