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
    inline double to_radians(float deg) {
        static const float factor = M_PI / 180.0;
        return deg * factor;
    }

    /**
     * @brief Calculate the barycentric coordinates of a point with respect to a
     * triangle
     *
     * @param point Query point
     * @param a     First triangle vertex
     * @param b     Second triangle vertex
     * @param c     Third triangle vertex
     * @return Vec3
     */
    inline Vec3 barycentric(const Vec2 &point,
                            const Vec2 &a,
                            const Vec2 &b,
                            const Vec2 &c) {
        Vec2 v0 = b - a;
        Vec2 v1 = c - a;
        Vec2 v2 = point - a;

        float d00 = v0 * v0;
        float d01 = v0 * v1;
        float d11 = v1 * v1;
        float d20 = v2 * v0;
        float d21 = v2 * v1;
        float inv = 1.0 / (d00 * d11 - d01 * d01);

        float v = (d11 * d20 - d01 * d21) * inv;
        float w = (d00 * d21 - d01 * d20) * inv;
        float u = 1.0f - v - w;
        return Vec3(u, v, w);
    }
} // namespace Dynamo