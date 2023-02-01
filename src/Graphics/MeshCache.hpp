#pragma once

#include <tiny_obj_loader.h>

#include "../Asset/AssetCache.hpp"
#include "./Mesh.hpp"

namespace Dynamo::Graphics {
    class MeshCache : public AssetCache<Mesh> {
        /**
         * @brief Load an obj file.
         *
         * @param filename
         */
        Mesh *load_obj(const std::string filename);

      protected:
        /**
         * @brief Load a mesh from a file.
         *
         * @param filename Path to the mesh file.
         * @return Mesh*
         */
        Mesh *load(const std::string filename) override;

      public:
        /**
         * @brief Construct a new MeshCache object.
         *
         * @param asset_directory Root asset directory.
         */
        MeshCache(const std::string asset_directory);

        /**
         * @brief Destroy the MeshCache object.
         *
         */
        virtual ~MeshCache() = default;
    };
} // namespace Dynamo::Graphics