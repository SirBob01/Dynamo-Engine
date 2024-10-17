#pragma once

#include <Graphics/Shader.hpp>
#include <Utils/IdTracker.hpp>

namespace Dynamo::Graphics {
    /**
     * @brief Topology modes.
     *
     */
    enum class Topology {
        Point,
        Triangle,
        Line,
    };

    /**
     * @brief Fill modes.
     *
     */
    enum class Fill {
        Point,
        Line,
        Face,
    };

    /**
     * @brief Cull modes.
     *
     */
    enum class Cull {
        None,
        Back,
        Front,
    };

    /**
     * @brief Material descriptor.
     *
     */
    struct Material {
        /**
         * @brief Vertex shader handle.
         *
         */
        Shader vertex;

        /**
         * @brief Fragment shader handle.
         *
         */
        Shader fragment;

        /**
         * @brief Topology of the vertex input.
         *
         */
        Topology topology = Topology::Triangle;

        /**
         * @brief Fill mode on draw.
         *
         */
        Fill fill = Fill::Face;

        /**
         * @brief Cull mode.
         *
         */
        Cull cull = Cull::Back;
    };
} // namespace Dynamo::Graphics