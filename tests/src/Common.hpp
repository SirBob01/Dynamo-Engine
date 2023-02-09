#pragma once

#include <catch2/matchers/catch_matchers_floating_point.hpp>

/**
 * @brief Approximator for comparing doubles
 *
 * @param x
 * @return Catch::Matchers::WithinAbsMatcher
 */
Catch::Matchers::WithinAbsMatcher Approx(double x) {
    return Catch::Matchers::WithinAbs(x, 1e-6);
}

/**
 * @brief Prevent optimizing a statement
 *
 * @tparam T
 * @param value
 */
template <class T>
inline void do_not_optimize(T &value) {
#if defined(__clang__)
    asm volatile("" : "+r,m"(value) : : "memory");
#else
    asm volatile("" : "+m,r"(value) : : "memory");
#endif
}