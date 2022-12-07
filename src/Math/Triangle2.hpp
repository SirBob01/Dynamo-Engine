#pragma once

#include "./Circle.hpp"
#include "./Vec2.hpp"
#include "./Vec3.hpp"

namespace Dynamo {
    /**
     * @brief 2D triangle
     *
     */
    struct Triangle2 {
        Vec2 a, b, c;

        /**
         * @brief Construct a new Triangle object
         *
         * @param a
         * @param b
         * @param c
         */
        Triangle2(Vec2 a, Vec2 b, Vec2 c) : a(a), b(b), c(c) {}

        /**
         * @brief Calculate the circle which minimally contains this triangle
         *
         * @return Circle
         */
        inline Circle circumcircle() const {
            Vec2 ab = b - a;
            Vec2 ac = c - a;

            float inv_d = 1.0 / (ab.cross(ac) * 2.0);
            float ab_sq = ab.length_squared();
            float ac_sq = ac.length_squared();

            float ux = inv_d * (ac.y * ab_sq - ab.y * ac_sq);
            float uy = inv_d * (ab.x * ac_sq - ac.x * ab_sq);
            Vec2 u(ux, uy);

            return Circle(a + u, u.length());
        }

        /**
         * @brief Calculate the barycentric coordinates of a point
         *
         * @param point
         * @return Vec3
         */
        inline Vec3 barycentric(const Vec2 &point) const {
            Vec2 ab = b - a;
            Vec2 ac = c - a;
            Vec2 ap = point - a;

            float d00 = ab * ab;
            float d01 = ab * ac;
            float d11 = ac * ac;
            float d20 = ap * ab;
            float d21 = ap * ac;
            float inv = 1.0 / (d00 * d11 - d01 * d01);

            float v = (d11 * d20 - d01 * d21) * inv;
            float w = (d00 * d21 - d01 * d20) * inv;
            float u = 1.0 - (v + w);
            return Vec3(u, v, w);
        }

        /**
         * @brief Calculate the winding order of the triangle
         *
         * If the result is < 0, the points are clockwise
         * If the result is = 0, the points are collinear
         * If the result is > 0, the points are anti-clockwise
         *
         * @return int
         */
        inline int winding() const {
            Vec2 ab = b - a;
            Vec2 ac = c - a;
            return ab.cross(ac);
        }

        /**
         * @brief Equality operator
         *
         * @param rhs
         * @return true
         * @return false
         */
        inline bool operator==(const Triangle2 &rhs) const {
            return a == rhs.a && b == rhs.b && c == rhs.c;
        }

        /**
         * @brief Inequality operator
         *
         * @param rhs
         * @return true
         * @return false
         */
        inline bool operator!=(const Triangle2 &rhs) const {
            return !(*this == rhs);
        }
    };
} // namespace Dynamo

/**
 * @brief Hash function implementation for Triangle2
 *
 * @tparam
 */
template <>
struct std::hash<Dynamo::Triangle2> {
    inline size_t operator()(const Dynamo::Triangle2 &triangle) const {
        size_t ta = std::hash<Dynamo::Vec2>{}(triangle.a);
        size_t tb = std::hash<Dynamo::Vec2>{}(triangle.b);
        size_t tc = std::hash<Dynamo::Vec2>{}(triangle.c);
        return ta ^ (tb << 1) ^ (tc << 2);
    }
};
