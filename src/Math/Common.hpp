#pragma once

constexpr double PI = 3.14159265358979323846264338327950288;

namespace Dynamo {
    /**
     * @brief Convert an angle in radians to degrees.
     *
     * @param rad
     * @return float
     */
    inline float to_degrees(float rad) {
        static const float factor = 180.0 / PI;
        return rad * factor;
    }

    /**
     * @brief Convert an angle in degrees to radians.
     *
     * @param deg
     * @return float
     */
    inline float to_radians(float deg) {
        static const float factor = PI / 180.0;
        return deg * factor;
    }

    /**
     * @brief Linearly interpolate between a and b.
     *
     * @param a Start value.
     * @param b End value.
     * @param t Interpolation factor.
     * @return float
     */
    inline float lerp(float a, float b, float t) { return a + (b - a) * t; }
} // namespace Dynamo