#pragma once

#include "./Vec2.hpp"

namespace Dynamo {
    /**
     * @brief 2D bidirectional line segment
     *
     */
    struct Segment2 {
        Vec2 a, b;

        /**
         * @brief Construct a new Segment2 object
         *
         * @param a
         * @param b
         */
        Segment2(Vec2 a, Vec2 b) : a(a), b(b) {}

        /**
         * @brief Calculate the squared length
         *
         * @return float
         */
        inline float length_squared() const { return (a - b).length_squared(); }

        /**
         * @brief Calculate the length
         *
         * @return float
         */
        inline float length() const { return (a - b).length(); }

        /**
         * @brief Equality operator
         *
         * @param rhs
         * @return true
         * @return false
         */
        inline bool operator==(const Segment2 &rhs) const {
            return (a == rhs.a && b == rhs.b) || (b == rhs.a && a == rhs.b);
        }

        /**
         * @brief Inequality operator
         *
         * @param rhs
         * @return true
         * @return false
         */
        inline bool operator!=(const Segment2 &rhs) const {
            return !(*this == rhs);
        }
    };
} // namespace Dynamo

/**
 * @brief Hash function implementation for Segment2
 *
 * @tparam
 */
template <>
struct std::hash<Dynamo::Segment2> {
    inline size_t operator()(const Dynamo::Segment2 &segment) const {
        size_t ta = std::hash<Dynamo::Vec2>{}(segment.a);
        size_t tb = std::hash<Dynamo::Vec2>{}(segment.b);
        return (ta ^ (tb << 1)) + ((ta << 1) ^ tb);
    }
};
