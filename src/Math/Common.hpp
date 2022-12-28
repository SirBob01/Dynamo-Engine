#pragma once

#include <cmath>

#include "../Math/Vec2.hpp"
#include "../Math/Vec3.hpp"

namespace Dynamo {
    /**
     * @brief Convert an angle in radians to degrees
     *
     * @param rad
     * @return float
     */
    inline float to_degrees(float rad) {
        static const float factor = 180.0 / M_PI;
        return rad * factor;
    }

    /**
     * @brief Convert an angle in degrees to radians
     *
     * @param deg
     * @return float
     */
    inline float to_radians(float deg) {
        static const float factor = M_PI / 180.0;
        return deg * factor;
    }
} // namespace Dynamo