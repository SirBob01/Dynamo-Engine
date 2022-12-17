#pragma once

#include <cmath>

namespace Dynamo {
    /**
     * @brief Complex number with real and imaginary components
     *
     */
    struct Complex {
        double re;
        double im;

        /**
         * @brief Construct a new Complex object
         *
         * @param re Real part
         * @param im Imaginary part
         */
        constexpr Complex(double re = 0, double im = 0) : re(re), im(im) {}

        /**
         * @brief Calculate the square length
         *
         * @return double
         */
        inline double length_squared() const { return re * re + im * im; }

        /**
         * @brief Calculate the length
         *
         * @return double
         */
        inline double length() const { return std::sqrt(length_squared()); }

        /**
         * @brief Calculate the reciprocal of the complex number for use in
         * divisions
         *
         * @return Complex
         */
        inline Complex reciprocal() const {
            double scale = 1.0 / length_squared();
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
            double scale = std::exp(re);
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
            double n_re = re * rhs.re - im * rhs.im;
            double n_im = re * rhs.im + im * rhs.re;
            return Complex(n_re, n_im);
        }

        /**
         * @brief Multiply by a scalar
         *
         * @param scalar
         * @return Complex
         */
        inline Complex operator*(double scalar) const {
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
        inline Complex operator/(double scalar) const {
            double inv = 1.0 / scalar;
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
            double n_re = re * rhs.re - im * rhs.im;
            double n_im = re * rhs.im + im * rhs.re;
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
        inline Complex &operator*=(double scalar) {
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
        inline Complex &operator/=(double scalar) {
            double inv = 1.0 / scalar;
            return *this *= inv;
        }

        /**
         * @brief Equality operator
         *
         * @param rhs
         * @return true
         * @return false
         */
        inline bool operator==(const Complex &rhs) const {
            return rhs.re == re && rhs.im == im;
        }

        /**
         * @brief Inequality operator
         *
         * @param rhs
         * @return true
         * @return false
         */
        inline bool operator!=(const Complex &rhs) const {
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
        long tre = complex.re * 73856093;
        long tim = complex.im * 19349663;
        return tre ^ tim;
    }
};
