#pragma once

#include <string>

#include "../Math/Color.hpp"
#include "../Types.hpp"
#include "Graphics/Shader.hpp"
#include "Utils/IdTracker.hpp"

namespace Dynamo::Graphics {
    /**
     * @brief Enumerates the culling modes.
     *
     */
    enum class CullMode {
        /**
         * @brief Culling is disabled.
         *
         */
        None,

        /**
         * @brief Front face is culled.
         *
         */
        Front,

        /**
         * @brief Backface is culled.
         *
         */
        Back
    };

    /**
     * @brief Enumerates the polygon geometry render modes.
     *
     */
    enum class PolygonMode {
        /**
         * @brief Color fill.
         *
         */
        Fill,

        /**
         * @brief Wireframe mesh.
         *
         */
        Line,

        /**
         * @brief Point cloud.
         *
         */
        Point,
    };

    /**
     * @brief Describes the basic visual properties of a drawn model.
     *
     */
    struct Material {
        /**
         * @brief Enable writing to the color buffer.
         *
         */
        b8 color_write;

        /**
         * @brief Enable writing to the depth buffer.
         *
         */
        b8 depth_write;

        /**
         * @brief Enable writing to the stencil buffer.
         *
         */
        b8 stencil_write;

        /**
         * @brief Enable rendering geometry with colors embedded in the vertex
         * attributes.
         *
         */
        b8 vertex_colors;

        /**
         * @brief Transparency value [0 - 1].
         *
         */
        f32 opacity;

        /**
         * @brief Cull mode.
         *
         */
        CullMode cull_mode;

        /**
         * @brief Polygon mode.
         *
         */
        PolygonMode polygon_mode;

        /**
         * @brief Shader list.
         *
         */
        ShaderList shaders;

        /**
         * @brief Build the material.
         *
         */
        virtual void build() = 0;
    };
} // namespace Dynamo::Graphics