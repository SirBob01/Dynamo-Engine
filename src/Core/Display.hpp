#pragma once

#include <memory>

#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "../Math/Vec2.hpp"
#include "../Messenger/Messenger.hpp"

namespace Dynamo {
    /**
     * @brief Display module exposes an interface for manipulating the
     * application window
     *
     */
    class Display {
        GLFWmonitor *_monitor;
        GLFWwindow *_window;

        Vec2 _size;
        bool _vsync;

      public:
        /**
         * @brief Construct a new Display object
         *
         * @param width      Initial width of the window
         * @param height     Initial height of the window
         * @param title      Title of the window
         * @param fullscreen Toggle fullscreen mode
         * @param vsync      Toggle vsync
         */
        Display(int width,
                int height,
                std::string title,
                bool fullscreen,
                bool vsync);

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
        GLFWwindow *get_window();

        /**
         * @brief Get the size of the display in screen coordinates
         *
         * @return Vec2
         */
        Vec2 get_window_size();

        /**
         * @brief Get the logical size of the display in pixels
         *
         * @return Vec2
         */
        Vec2 get_size();

        /**
         * @brief Is the display closed?
         *
         * @return true
         * @return false
         */
        bool is_closed();

        /**
         * @brief Is the display windowed or in fullscreen?
         *
         * @return true
         * @return false
         */
        bool is_fullscreen();

        /**
         * @brief Is vsync enabled?
         *
         * @return true
         * @return false
         */
        bool is_vsync();

        /**
         * @brief Set the logical size of the display
         *
         * @param size
         */
        void set_size(Vec2 size);

        /**
         * @brief Set the title of the display
         *
         * @param title
         */
        void set_title(std::string title);

        /**
         * @brief Toggle fullscreen or windowed
         *
         * @param fullscreen
         */
        void set_fullscreen(bool fullscreen);

        /**
         * @brief Toggle vsync
         *
         * @param vsync
         */
        void set_vsync(bool vsync);

        /**
         * @brief Set the icon of the window
         *
         * @param filename Path to the image file
         */
        void set_icon(std::string filename);
    };
} // namespace Dynamo