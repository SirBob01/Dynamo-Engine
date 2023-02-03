#pragma once

#include <functional>

#include "../Types.hpp"
#include "./Vec2.hpp"

namespace Dynamo {
    /**
     * @brief 2D axis-aligned bounding volume.
     *
     */
    struct Box2 {
        Vec2 min;
        Vec2 max;

        /**
         * @brief Construct a new Box2 object.
         *
         * @param min
         * @param max
         */
        constexpr Box2(Vec2 min = {}, Vec2 max = {}) : min(min), max(max) {}

        /**
         * @brief Construct a new Box2 object given its center and dimensions.
         *
         * @param center
         * @param width
         * @param height
         */
        Box2(Vec2 center, f32 width = 0, f32 height = 0) {
            Vec2 halfdim(width * 0.5, height * 0.5);
            min = center - halfdim;
            max = center + halfdim;
        }

        /**
         * @brief Get the center of this box.
         *
         * @return Vec2
         */
        inline Vec2 center() const { return (min + max) * 0.5; }

        /**
         * @brief Get the width of this box.
         *
         * @return f32
         */
        inline f32 width() const { return max.x - min.x; }

        /**
         * @brief Get the height of this box.
         *
         * @return f32
         */
        inline f32 height() const { return max.y - min.y; }

        /**
         * @brief Get the area of this box.
         *
         * @return f32
         */
        inline f32 area() const { return width() * height(); }

        /**
         * @brief Translate this box by an offset vector.
         *
         * @param offset
         * @return Box2D&
         */
        inline Box2 &translate(const Vec2 &offset) {
            min += offset;
            max += offset;
            return *this;
        }

        /**
         * @brief Clamp a point to be contained within the box.
         *
         * @param point
         * @return Vec2
         */
        inline Vec2 clamp(const Vec2 &point) const {
            return Vec2(std::clamp(point.x, min.x, max.x),
                        std::clamp(point.y, min.y, max.y));
        }

        /**
         * @brief Test if this box overlaps with another box.
         *
         * @param box
         * @return true
         * @return false
         */
        inline b8 intersects(const Box2 &box) const {
            b8 h = max.x < box.min.x || min.x > box.max.x;
            b8 v = max.y < box.min.y || min.y > box.max.y;
            return !(h || v);
        }

        /**
         * @brief Test if a point is contained within this box.
         *
         * @param point
         * @return true
         * @return false
         */
        inline b8 contains(const Vec2 &point) const {
            b8 h = point.x >= min.x && point.x <= max.x;
            b8 v = point.y >= min.y && point.y <= max.y;
            return h && v;
        }

        /**
         * @brief Test if a box is contained within this box.
         *
         * @param point
         * @return true
         * @return false
         */
        inline b8 contains(const Box2 &box) const {
            b8 h = box.min.x >= min.x && box.max.x <= max.x;
            b8 v = box.min.y >= min.y && box.max.y <= max.y;
            return h && v;
        }

        /**
         * @brief Equality operator.
         *
         * @param rhs
         * @return true
         * @return false
         */
        inline b8 operator==(const Box2 &rhs) const {
            return min == rhs.min && max == rhs.max;
        }

        /**
         * @brief Inequality operator.
         *
         * @param rhs
         * @return true
         * @return false
         */
        inline b8 operator!=(const Box2 &rhs) const { return !(*this == rhs); }

        /**
         * @brief Test if the bounding volume is valid (min is less than max).
         *
         * @return true
         * @return false
         */
        inline b8 valid() const { return width() >= 0 && height() >= 0; }
    };
} // namespace Dynamo

/**
 * @brief Hash function implementation for Box2.
 *
 * @tparam
 */
template <>
struct std::hash<Dynamo::Box2> {
    inline size_t operator()(const Dynamo::Box2 &box) const {
        size_t tmin = std::hash<Dynamo::Vec2>{}(box.min);
        size_t tmax = std::hash<Dynamo::Vec2>{}(box.max);
        return tmin ^ (tmax << 1);
    }
};
