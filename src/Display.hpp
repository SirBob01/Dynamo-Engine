#pragma once
#define GLFW_INCLUDE_VULKAN

#include <memory>
#include <string>

#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "./Input.hpp"
#include "./Math/Vec2.hpp"

namespace Dynamo {
    /**
     * @brief Display window.
     *
     */
    class Display {
        GLFWmonitor *_monitor;
        GLFWwindow *_window;

        bool _vsync;

        std::unique_ptr<Input> _input;

      public:
        /**
         * @brief Initialize the display.
         *
         * @param title
         * @param window_width
         * @param window_height
         */
        Display(const std::string &title,
                unsigned window_width,
                unsigned window_height);
        ~Display();

        /**
         * @brief Get the Input subsystem.
         *
         * @return Input&
         */
        Input &get_input();

        /**
         * @brief Get the window size.
         *
         * @return Vec2
         */
        Vec2 get_window_size() const;

        /**
         * @brief Get the framebuffer size.
         *
         * @return Vec2
         */
        Vec2 get_framebuffer_size() const;

        /**
         * @brief Check if the display is open.
         *
         * @return true
         * @return false
         */
        bool is_open() const;

        /**
         * @brief Check if fullscreen mode is enabled.
         *
         * @return true
         * @return false
         */
        bool is_fullscreen() const;

        /**
         * @brief Check if vsync is enabled.
         *
         * @return true
         * @return false
         */
        bool is_vsync() const;

        /**
         * @brief Set the window size.
         *
         * @param size
         */
        void set_window_size(Vec2 size);

        /**
         * @brief Set fullscreen mode.
         *
         * @param flag
         */
        void set_fullscreen(bool flag);

        /**
         * @brief Set vsync.
         *
         * @param flag
         */
        void set_vsync(bool flag);

        /**
         * @brief Set the title of the window.
         *
         * @param title
         */
        void set_title(const std::string &title);

        /**
         * @brief Set the icon of the window.
         *
         * @param filepath
         */
        void set_icon(const std::string &filepath);
    };

    /**
     * @brief Display reference.
     *
     */
    using DisplayRef = std::reference_wrapper<Display>;
} // namespace Dynamo