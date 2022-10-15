#pragma once

#include <memory>

#include <GLFW/glfw3.h>

#include "../Messenger/Messenger.hpp"

namespace Dynamo {
    /**
     * @brief Display module exposes an interface for manipulating the
     * application window
     *
     */
    class Display {
        GLFWwindow *_window;

      public:
        /**
         * @brief Construct a new Display object
         *
         * @param width      Initial width of the window
         * @param height     Initial height of the window
         * @param title      Title of the window
         * @param fullscreen Toggle fullscreen mode
         */
        Display(int width, int height, std::string title, bool fullscreen);

        /**
         * @brief Destroy the Display object
         *
         * This will clean up the internal GLFW window
         *
         */
        ~Display();

        /**
         * @brief Get the underlying GLFW window
         *
         * @return GLFWwindow&
         */
        GLFWwindow &get_window();

        /**
         * @brief Set the title of the display
         *
         * @param title
         */
        void set_title(std::string title);
    };
} // namespace Dynamo