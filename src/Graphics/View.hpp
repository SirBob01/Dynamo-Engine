#pragma once

#include "../Math/Box2.hpp"
#include "../Math/Color.hpp"
#include "../Math/Matrix.hpp"
#include "../Math/Vec2.hpp"
#include "../Types.hpp"
#include "./GeometryInstance.hpp"
#include "./Material.hpp"

namespace Dynamo::Graphics {
    /**
     * @brief Defines the extent of the rendering area.
     *
     */
    struct Viewport : public Box2 {
        f32 min_depth;
        f32 max_depth;
    };

    /**
     * @brief An area of the rendering target to draw to.
     *
     */
    class View {
      public:
        /**
         * @brief Destroy the View object.
         *
         */
        virtual ~View() = 0;

        /**
         * @brief Get a reference to the viewport.
         *
         * @return Viewport&
         */
        virtual Viewport &get_viewport() = 0;

        /**
         * @brief Get a reference to the scissor clipping boundary.
         *
         * @return Box2&
         */
        virtual Box2 &get_scissor() = 0;

        /**
         * @brief Draw a model.
         *
         * @param geometry   Model geometry
         * @param material   Model material
         * @param transforms Array of transforms for instanced rendering
         */
        virtual void draw(GeometryInstance &geometry,
                          Material &material,
                          std::vector<Matrix<4>> &transforms) = 0;

        /**
         * @brief Set the clear color of the view.
         *
         * @param color
         */
        virtual void clear(Color color) = 0;
    };
} // namespace Dynamo::Graphics