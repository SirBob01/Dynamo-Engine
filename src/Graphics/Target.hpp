#pragma once

#include "../Math/Box2.hpp"
#include "./View.hpp"

namespace Dynamo::Graphics {
    /**
     * @brief Target for rendering.
     *
     */
    class Target {
      public:
        /**
         * @brief Destroy the Target object.
         *
         */
        virtual ~Target() = 0;

        /**
         * @brief Get the width of the render target.
         *
         * @return u32
         */
        virtual u32 get_width() const = 0;

        /**
         * @brief Get the height of the render target.
         *
         * @return u32
         */
        virtual u32 get_height() const = 0;

        /**
         * @brief Get the list of all views in this render target.
         *
         * @return const std::vector<std::unique_ptr<View>>&
         */
        virtual const std::vector<std::unique_ptr<View>> &get_views() = 0;

        /**
         * @brief Add a view to the render target.
         *
         * @return std::unique_ptr<View>
         */
        virtual std::unique_ptr<View> add_view() = 0;
    };
} // namespace Dynamo::Graphics