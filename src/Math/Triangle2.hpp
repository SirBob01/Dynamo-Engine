#pragma once

#include <functional>

#include "../Types.hpp"
#include "./Circle.hpp"
#include "./Vec2.hpp"
#include "./Vec3.hpp"

namespace Dynamo {
    /**
     * @brief 2D triangle.
     *
     */
    struct Triangle2 {
        Vec2 a, b, c;

        /**
         * @brief Construct a new Triangle object.
         *
         * @param a
         * @param b
         * @param c
         */
        constexpr Triangle2(Vec2 a = {}, Vec2 b = {}, Vec2 c = {}) : a(a), b(b), c(c) {}

        /**
         * @brief Calculate the circle which minimally contains this triangle.
         *
         * @return Circle
         */
        inline Circle circumcircle() const {
            Vec2 ab = b - a;
            Vec2 ac = c - a;

            f32 inv_d = 1.0 / (ab.cross(ac) * 2.0);
            f32 ab_sq = ab.length_squared();
            f32 ac_sq = ac.length_squared();

            f32 ux = inv_d * (ac.y * ab_sq - ab.y * ac_sq);
            f32 uy = inv_d * (ab.x * ac_sq - ac.x * ab_sq);
            Vec2 u(ux, uy);

            return Circle(a + u, u.length());
        }

        /**
         * @brief Calculate the barycentric coordinates of a point.
         *
         * @param point
         * @return Vec3
         */
        inline Vec3 barycentric(const Vec2 &point) const {
            Vec2 ab = b - a;
            Vec2 ac = c - a;
            Vec2 ap = point - a;

            f32 d00 = ab * ab;
            f32 d01 = ab * ac;
            f32 d11 = ac * ac;
            f32 d20 = ap * ab;
            f32 d21 = ap * ac;
            f32 inv = 1.0 / (d00 * d11 - d01 * d01);

            f32 v = (d11 * d20 - d01 * d21) * inv;
            f32 w = (d00 * d21 - d01 * d20) * inv;
            f32 u = 1.0 - (v + w);
            return Vec3(u, v, w);
        }

        /**
         * @brief Calculate the winding order of the triangle.
         *
         * If the result is < 0, the points are clockwise.
         * If the result is = 0, the points are collinear.
         * If the result is > 0, the points are anti-clockwise.
         *
         * @return i32
         */
        inline i32 winding() const {
            Vec2 ab = b - a;
            Vec2 ac = c - a;
            return ab.cross(ac);
        }

        /**
         * @brief Count the number of shared vertices with another triangle.
         *
         * @param rhs
         * @return i32
         */
        inline i32 shared_vertices(const Triangle2 &rhs) const {
            return (a == rhs.a) + (a == rhs.b) + (a == rhs.c) + (b == rhs.a) +
                   (b == rhs.b) + (b == rhs.c) + (c == rhs.a) + (c == rhs.b) +
                   (c == rhs.c);
        }

        /**
         * @brief Test if a triangle shares an edge with another triangle.
         *
         * @param rhs
         * @return true
         * @return false
         */
        inline b8 adjacent(const Triangle2 &rhs) const {
            return shared_vertices(rhs) > 1;
        }

        /**
         * @brief Equality operator.
         *
         * @param rhs
         * @return true
         * @return false
         */
        inline b8 operator==(const Triangle2 &rhs) const {
            return a == rhs.a && b == rhs.b && c == rhs.c;
        }

        /**
         * @brief Inequality operator.
         *
         * @param rhs
         * @return true
         * @return false
         */
        inline b8 operator!=(const Triangle2 &rhs) const {
            return !(*this == rhs);
        }
    };
} // namespace Dynamo

/**
 * @brief Hash function implementation for Triangle2.
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
