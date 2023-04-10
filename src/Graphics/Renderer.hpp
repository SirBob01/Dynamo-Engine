#pragma once

#include <functional>

#include "../Core/Display.hpp"
#include "../Math/Color.hpp"

namespace Dynamo::Graphics {
    /**
     * @brief Abstract graphics rendering engine for drawing 2D and 3D objects
     *
     */
    class Renderer {
      protected:
        /**
         * @brief Reference to the video display
         *
         */
        std::reference_wrapper<Display> _display;

      public:
        /**
         * @brief Construct a new Renderer object
         *
         * @param display
         */
        Renderer(Display &display) : _display(display) {}

        /**
         * @brief Destroy the Renderer object
         *
         */
        virtual ~Renderer() = 0;

        /**
         * @brief Refresh the renderer to update the display
         *
         */
        virtual void refresh() = 0;

        /**
         * @brief Clear the display with a color
         *
         */
        virtual void clear(Color color) = 0;
    };
    inline Renderer::~Renderer() = default;
} // namespace Dynamo::Graphics