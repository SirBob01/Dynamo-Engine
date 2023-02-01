#pragma once

#include <string>

#include "../Asset/Asset.hpp"
#include "./Material.hpp"
#include "./Shader.hpp"

namespace Dynamo::Graphics {
    /**
     * @brief Enumerates the polygon draw modes.
     *
     */
    enum class PolygonMode {
        /**
         * @brief Color fill.
         *
         */
        Fill,

        /**
         * @brief Point cloud.
         *
         */
        Point,

        /**
         * @brief Wireframe mesh.
         *
         */
        Line
    };

    /**
     * @brief Material system for building and setting materials.
     *
     */
    class MaterialSystem {
      public:
        /**
         * @brief Destroy the MaterialSystem object.
         *
         */
        virtual ~MaterialSystem() = default;

        /**
         * @brief Build a new material from a shader set.
         *
         * @param shaders      List of shader filenames used in the pipeline.
         * @param polygon_mode Polygon rendering mode.
         * @return std::unique_ptr<Material>
         */
        virtual std::unique_ptr<Material>
        build(ShaderList &shaders,
              PolygonMode polygon_mode = PolygonMode::Fill) = 0;
    };
} // namespace Dynamo::Graphics