#pragma once

#include "../Core/Display.hpp"

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
        Display &_display;

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
    };
    inline Renderer::~Renderer() = default;
} // namespace Dynamo::Graphics