#pragma once

#include <cmath>

#include "../Math/Vec2.hpp"
#include "../Math/Vec3.hpp"
#include "../Types.hpp"

namespace Dynamo {
    /**
     * @brief Convert an angle in radians to degrees.
     *
     * @param rad
     * @return f32
     */
    inline f32 to_degrees(f32 rad) {
        static const f32 factor = 180.0 / M_PI;
        return rad * factor;
    }

    /**
     * @brief Convert an angle in degrees to radians.
     *
     * @param deg
     * @return f32
     */
    inline f32 to_radians(f32 deg) {
        static const f32 factor = M_PI / 180.0;
        return deg * factor;
    }

    /**
     * @brief Linearly interpolate between a and b.
     *
     * @param a Start value.
     * @param b End value.
     * @param t Interpolation factor.
     * @return f32
     */
    inline f32 lerp(f32 a, f32 b, f32 t) { return a + (b - a) * t; }
} // namespace Dynamo