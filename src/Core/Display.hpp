#pragma once

#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "../Log/Log.hpp"
#include "../Math/Vec2.hpp"
#include "../Types.hpp"

namespace Dynamo {
    /**
     * @brief Display module exposes an interface for manipulating the
     * application window.
     *
     */
    class Display {
        GLFWmonitor *_monitor;
        GLFWwindow *_window;

        Vec2 _size;
        b8 _vsync;

        std::string _title;

      public:
        /**
         * @brief Construct a new Display object.
         *
         * @param width      Initial width of the window.
         * @param height     Initial height of the window.
         * @param title      Title of the window.
         * @param fullscreen Toggle fullscreen mode.
         * @param vsync      Toggle vsync.
         */
        Display(i32 width,
                i32 height,
                std::string title,
                b8 fullscreen,
                b8 vsync);

        /**
         * @brief Destroy the Display object.
         *
         */
        ~Display();

        /**
         * @brief Get the underlying GLFW window.
         *
         * @return GLFWwindow&
         */
        GLFWwindow *get_window();

        /**
         * @brief Get the size of the display in screen coordinates.
         *
         * @return Vec2
         */
        Vec2 get_window_size() const;

        /**
         * @brief Get the size of the framebuffer in pixels.
         *
         * @return Vec2
         */
        Vec2 get_framebuffer_size() const;

        /**
         * @brief Get the logical size of the display.
         *
         * @return Vec2
         */
        Vec2 get_size() const;

        /**
         * @brief Get the title of the window.
         *
         * @return std::string
         */
        std::string get_title() const;

        /**
         * @brief Is the display closed?
         *
         * @return true
         * @return false
         */
        b8 is_closed() const;

        /**
         * @brief Is the display windowed or in fullscreen?
         *
         * @return true
         * @return false
         */
        b8 is_fullscreen() const;

        /**
         * @brief Is vsync enabled?
         *
         * @return true
         * @return false
         */
        b8 is_vsync() const;

        /**
         * @brief Set the logical size of the display.
         *
         * @param size
         */
        void set_size(Vec2 size);

        /**
         * @brief Set the title of the display.
         *
         * @param title
         */
        void set_title(std::string title);

        /**
         * @brief Toggle fullscreen or windowed.
         *
         * @param fullscreen
         */
        void set_fullscreen(b8 fullscreen);

        /**
         * @brief Toggle vsync.
         *
         * @param vsync
         */
        void set_vsync(b8 vsync);

        /**
         * @brief Set the icon of the window.
         *
         * @param filename Path to the image file.
         */
        void set_icon(std::string filename);
    };
} // namespace Dynamo