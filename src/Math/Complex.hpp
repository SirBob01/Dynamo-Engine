#pragma once

#include <cmath>
#include <functional>

#include "../Types.hpp"

namespace Dynamo {
    /**
     * @brief Complex number with real and imaginary components
     *
     */
    struct Complex {
        f32 re;
        f32 im;

        /**
         * @brief Construct a new Complex object
         *
         * @param re Real part
         * @param im Imaginary part
         */
        constexpr Complex(f32 re = 0, f32 im = 0) : re(re), im(im) {}

        /**
         * @brief Calculate the square length
         *
         * @return f32
         */
        inline f32 length_squared() const { return re * re + im * im; }

        /**
         * @brief Calculate the length
         *
         * @return f32
         */
        inline f32 length() const { return std::sqrt(length_squared()); }

        /**
         * @brief Calculate the reciprocal of the complex number for use in
         * divisions
         *
         * @return Complex
         */
        inline Complex reciprocal() const {
            f32 scale = 1.0 / length_squared();
            return Complex(re * scale, -im * scale);
        }

        /**
         * @brief Calculate the conjugate of the complex number
         *
         * @return Complex
         */
        inline Complex conjugate() const { return Complex(re, -im); }

        /**
         * @brief Apply Euler's formula
         *
         * e^(x + y i)
         * = e^x * e^yi
         * = e^x * (cos(y) + sin(y) i)
         * = e^x * cos(y) + e^x * sin(y) i
         *
         * @return Complex
         */
        inline Complex exp() const {
            f32 scale = std::exp(re);
            return Complex(scale * std::cos(im), scale * std::sin(im));
        }

        /**
         * @brief Negation
         *
         * @return Complex
         */
        inline Complex operator-() const { return Complex(-re, -im); }

        /**
         * @brief Add another complex number
         *
         * @param rhs
         * @return Complex
         */
        inline Complex operator+(const Complex &rhs) const {
            return Complex(re + rhs.re, im + rhs.im);
        }

        /**
         * @brief Subtract another complex number
         *
         * @param rhs
         * @return Complex
         */
        inline Complex operator-(const Complex &rhs) const {
            return Complex(re - rhs.re, im - rhs.im);
        }

        /**
         * @brief Multiply by another complex number
         *
         * @param rhs
         * @return Complex
         */
        inline Complex operator*(const Complex &rhs) const {
            f32 n_re = re * rhs.re - im * rhs.im;
            f32 n_im = re * rhs.im + im * rhs.re;
            return Complex(n_re, n_im);
        }

        /**
         * @brief Multiply by a scalar
         *
         * @param scalar
         * @return Complex
         */
        inline Complex operator*(f32 scalar) const {
            return Complex(scalar * re, scalar * im);
        }

        /**
         * @brief Divide by another complex number
         *
         * @param rhs
         * @return Complex
         */
        inline Complex operator/(const Complex &rhs) const {
            return *this * rhs.reciprocal();
        }

        /**
         * @brief Divide by a scalar
         *
         * @param scalar
         * @return Complex
         */
        inline Complex operator/(f32 scalar) const {
            f32 inv = 1.0 / scalar;
            return *this * inv;
        }

        /**
         * @brief Add another complex number in-place
         *
         * @param rhs
         * @return Complex&
         */
        inline Complex &operator+=(const Complex &rhs) {
            re += rhs.re;
            im += rhs.im;
            return *this;
        }

        /**
         * @brief Subtract another complex number in-place
         *
         * @param rhs
         * @return Complex&
         */
        inline Complex &operator-=(const Complex &rhs) {
            re -= rhs.re;
            im -= rhs.im;
            return *this;
        }

        /**
         * @brief Multiply by another complex number in-place
         *
         * @param rhs
         * @return Complex&
         */
        inline Complex &operator*=(const Complex &rhs) {
            f32 n_re = re * rhs.re - im * rhs.im;
            f32 n_im = re * rhs.im + im * rhs.re;
            re = n_re;
            im = n_im;
            return *this;
        }

        /**
         * @brief Multiply by a scalar in-place
         *
         * @param scalar
         * @return Complex&
         */
        inline Complex &operator*=(f32 scalar) {
            re *= scalar;
            im *= scalar;
            return *this;
        }

        /**
         * @brief Divide by another complex number in-place
         *
         * @param rhs
         * @return Complex&
         */
        inline Complex &operator/=(const Complex &rhs) {
            Complex inv = rhs.reciprocal();
            return *this *= inv;
        }

        /**
         * @brief Divide by a scalar in-place
         *
         * @param scalar
         * @return Complex&
         */
        inline Complex &operator/=(f32 scalar) {
            f32 inv = 1.0 / scalar;
            return *this *= inv;
        }

        /**
         * @brief Equality operator
         *
         * @param rhs
         * @return true
         * @return false
         */
        inline b8 operator==(const Complex &rhs) const {
            return rhs.re == re && rhs.im == im;
        }

        /**
         * @brief Inequality operator
         *
         * @param rhs
         * @return true
         * @return false
         */
        inline b8 operator!=(const Complex &rhs) const {
            return !(*this == rhs);
        }
    };
} // namespace Dynamo

/**
 * @brief Hash function implementation for Complex
 *
 * @tparam
 */
template <>
struct std::hash<Dynamo::Complex> {
    inline size_t operator()(const Dynamo::Complex &complex) const {
        Dynamo::i64 tre = complex.re * 73856093;
        Dynamo::i64 tim = complex.im * 19349663;
        return tre ^ tim;
    }
};
