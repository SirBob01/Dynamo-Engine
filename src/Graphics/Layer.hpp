#pragma once

namespace Dynamo::Graphics {
    /**
     * @brief Enumerates the different types of render layers
     *
     */
    enum class Layer {
        /**
         * @brief Standard 3D scene render layer
         *
         */
        L3D,

        /**
         * @brief Render layer for 2D geometry with no depth testing
         *
         */
        L2D,
    };
} // namespace Dynamo::Graphics