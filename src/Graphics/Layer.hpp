#pragma once

#include <vector>

#include "../Math/Box2.hpp"
#include "../Types.hpp"
#include "./LayerView.hpp"

namespace Dynamo::Graphics {
    /**
     * @brief Enumerates the different types of rendering layers.
     *
     */
    enum class LayerType {
        /**
         * @brief Standard 3D scene layer.
         *
         */
        L3D,

        /**
         * @brief Layer for 2D geometry with no depth testing (draw order matters).
         *
         */
        L2D,
    };

    /**
     * @brief Layer configuration settings.
     *
     */
    struct LayerConfiguration {
        /**
         * @brief Type of the layer.
         *
         */
        LayerType type;

        /**
         * @brief Can the layer be GPU picked?
         *
         * If a pickable layer is on top of another pickable layer,
         * the former will take precedence.
         *
         */
        b8 gpu_pickable;
    };

    /**
     * @brief A layer is a distinct target on which objects are drawn.
     *
     */
    class Layer {
      public:
        /**
         * @brief Destroy the Layer object.
         *
         */
        virtual ~Layer() = default;

        /**
         * @brief Create a layer view.
         *
         * @param extent  Extent of the view.
         * @param scissor Scissor of the view.
         * @return LayerView&
         */
        virtual LayerView &create_view(Box2 extent, Box2 scissor) = 0;
    };
} // namespace Dynamo::Graphics