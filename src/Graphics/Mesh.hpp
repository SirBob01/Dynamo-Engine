#pragma once

#include <vector>

#include "../Types.hpp"
#include "./Vertex.hpp"

namespace Dynamo::Graphics {
    /**
     * @brief 3D mesh data
     *
     */
    struct Mesh {
        /**
         * @brief Vertex array
         *
         */
        std::vector<Vertex> vertices;

        /**
         * @brief Index array
         *
         */
        std::vector<u32> indices;

        /**
         * @brief Construct a new Mesh object
         *
         * @param vertices Array of vertices
         * @param indices  Array of indices
         */
        Mesh(std::vector<Vertex> &vertices, std::vector<u32> &indices);
    };
} // namespace Dynamo::Graphics