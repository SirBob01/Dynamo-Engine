#pragma once

#include <catch2/matchers/catch_matchers_floating_point.hpp>

/**
 * @brief Approximator for comparing doubles
 *
 * @param x
 * @param eps
 * @return Catch::Matchers::WithinAbsMatcher
 */
inline Catch::Matchers::WithinAbsMatcher Approx(double x, double eps = 1e-6) {
    return Catch::Matchers::WithinAbs(x, eps);
}
