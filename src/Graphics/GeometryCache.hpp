#pragma once

#include <string>
#include <vector>

#include <tiny_obj_loader.h>

#include "../Asset/AssetCache.hpp"
#include "../Log/Log.hpp"
#include "../Types.hpp"
#include "./Geometry.hpp"

namespace Dynamo::Graphics {
    /**
     * @brief Geometry asset cache
     *
     */
    class GeometryCache : public AssetCache<Geometry> {
      private:
        /**
         * @brief Load geometry from an .obj file
         *
         * @param filepath
         * @return Geometry*
         */
        Geometry *load_obj(const std::string filepath);

      protected:
        /**
         * @brief Load geometry from a file
         *
         * @param filepath
         * @return Geometry*
         */
        Geometry *load(const std::string filepath) override;

      public:
        /**
         * @brief Construct a new GeometryCache object
         *
         * @param asset_directory Root asset directory
         */
        GeometryCache(const std::string asset_directory);
    };
} // namespace Dynamo::Graphics