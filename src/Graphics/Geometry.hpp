#pragma once

#include <vector>

#include "../Types.hpp"
#include "./Vertex.hpp"

namespace Dynamo::Graphics {
    /**
     * @brief 3D Geometry structure
     *
     */
    struct Geometry {
        /**
         * @brief Ordered array of vertices
         *
         */
        std::vector<Vertex> vertices;

        /**
         * @brief Ordered array of indices to the vertex array
         *
         */
        std::vector<u32> indices;
    };
} // namespace Dynamo::Graphics