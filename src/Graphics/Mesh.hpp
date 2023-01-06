#pragma once

#include <string>
#include <vector>

#include <tiny_obj_loader.h>

#include "../Log/Log.hpp"
#include "./Vertex.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Enumeration of the different 3D file types
     *
     */
    enum class File3D : unsigned {
        /**
         * @brief Wavefront Obj file
         *
         */
        Obj
    };

    /**
     * @brief 3D mesh
     *
     */
    struct Mesh {
        std::vector<Vertex> vertices;
        std::vector<unsigned> indices;

        /**
         * @brief Construct a new Mesh object
         *
         * @param filename Path to the 3D file
         * @param filetype Type of the 3D file
         */
        Mesh(const std::string filename, const File3D filetype);

      private:
        /**
         * @brief Load an obj file
         *
         * @param filename Path to the 3D file
         */
        void load_obj(const std::string filename);
    };
} // namespace Dynamo::Graphics::Vulkan