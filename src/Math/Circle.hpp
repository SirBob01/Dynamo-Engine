#pragma once

#include "./Vec2.hpp"

namespace Dynamo {
    /**
     * @brief 2D circle
     *
     */
    struct Circle {
        Vec2 center;
        float radius;

        /**
         * @brief Construct a new Circle object
         *
         * @param center
         * @param radius
         */
        Circle(Vec2 center, float radius = 1) :
            center(center), radius(radius) {}

        /**
         * @brief Get the circumference of this circle
         *
         * @return float
         */
        inline float circumference() const { return 2 * M_PI * radius; }

        /**
         * @brief Get the diameter of this circle
         *
         * @return float
         */
        inline float diameter() const { return 2 * radius; }

        /**
         * @brief Get the area of this circle
         *
         * @return float
         */
        inline float area() const { return M_PI * radius * radius; }

        /**
         * @brief Test if this circle intersects with another circle
         *
         * @param circle
         * @return true
         * @return false
         */
        inline bool intersects(const Circle &circle) const {
            float r_sum = circle.radius + radius;
            float d_sq = (circle.center - center).length_squared();
            return d_sq <= r_sum * r_sum;
        }

        /**
         * @brief Test if a point is contained within this circle
         *
         * @param point
         * @return true
         * @return false
         */
        inline bool contains(const Vec2 &point) const {
            float d_sq = (point - center).length_squared();
            return d_sq <= radius * radius;
        }

        /**
         * @brief Test if a circle is contained within this circle
         *
         * @param circle
         * @return true
         * @return false
         */
        inline bool contains(const Circle &circle) const {
            float d_sq = (circle.center - center).length_squared();
            float r_diff = (circle.radius - radius);
            return d_sq <= r_diff * r_diff;
        }

        /**
         * @brief Equality operator
         *
         * @param rhs
         * @return true
         * @return false
         */
        inline bool operator==(const Circle &rhs) const {
            return radius == rhs.radius && center == rhs.center;
        }

        /**
         * @brief Inequality operator
         *
         * @param rhs
         * @return true
         * @return false
         */
        inline bool operator!=(const Circle &rhs) const {
            return !(*this == rhs);
        }

        /**
         * @brief Test if the circle is valid (radius is non-negative)
         *
         * @return true
         * @return false
         */
        inline bool valid() const { return radius >= 0; }
    };
} // namespace Dynamo