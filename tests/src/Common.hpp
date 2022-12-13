#pragma once

#include <catch2/matchers/catch_matchers_floating_point.hpp>

/**
 * @brief Approximator for comparing doubles
 *
 * @param x
 * @return Catch::Matchers::WithinAbsMatcher
 */
Catch::Matchers::WithinAbsMatcher Approx(double x) {
    return Catch::Matchers::WithinAbs(x, 1e-7);
}
