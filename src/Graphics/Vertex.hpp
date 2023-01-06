#pragma once

#include "../Math/Vec2.hpp"
#include "../Math/Vec3.hpp"

namespace Dynamo::Graphics {
    /**
     * @brief Vertex object
     *
     */
    struct Vertex {
        Vec3 position;
        Vec3 normal;
        Vec2 texture;

        /**
         * @brief Equality operator
         *
         * @param rhs
         * @return true
         * @return false
         */
        inline bool operator==(const Vertex &rhs) const {
            return position == rhs.position && normal == rhs.normal &&
                   texture == rhs.texture;
        }
    };
} // namespace Dynamo::Graphics

/**
 * @brief Hash function implementation for Vertex
 *
 * @tparam
 */
template <>
struct std::hash<Dynamo::Graphics::Vertex> {
    inline size_t operator()(const Dynamo::Graphics::Vertex &vertex) const {
        size_t tp = std::hash<Dynamo::Vec3>{}(vertex.position);
        size_t tn = std::hash<Dynamo::Vec3>{}(vertex.normal);
        size_t tt = std::hash<Dynamo::Vec2>{}(vertex.texture);
        return (((tp ^ (tn << 1)) >> 1) ^ (tt << 2)) >> 2;
    }
};
