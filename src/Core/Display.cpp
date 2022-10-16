#define STB_IMAGE_IMPLEMENTATION
#include "Display.hpp"

namespace Dynamo {
    Display::Display(int width,
                     int height,
                     std::string title,
                     bool fullscreen,
                     bool vsync) {
        _monitor = glfwGetPrimaryMonitor();
        if (!_monitor) {
            Messenger::error("GLFW could not find primary monitor device.");
        }
        const GLFWvidmode *mode = glfwGetVideoMode(_monitor);
        if (width == 0) width = mode->width;
        if (height == 0) height = mode->height;

        _window =
            glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        _size = Vec2(width, height);
        _vsync = vsync;

        if (!_window) {
            Messenger::error("Failed to create a GLFW window");
        }
        glfwMakeContextCurrent(_window);

        set_fullscreen(fullscreen);
        set_vsync(vsync);
    }

    Display::~Display() { glfwDestroyWindow(_window); }

    const GLFWwindow &Display::get_window() { return *_window; }

    Vec2 Display::get_window_size() {
        int width, height;
        glfwGetWindowSize(_window, &width, &height);
        return Vec2(width, height);
    }

    const Vec2 &Display::get_size() { return _size; }

    bool Display::get_closed() { return glfwWindowShouldClose(_window); }

    bool Display::get_fullscreen() {
        return glfwGetWindowMonitor(_window) != nullptr;
    }

    bool Display::get_vsync() { return _vsync; }

    void Display::set_size(const Vec2 &size) {
        _size.x = size.x;
        _size.y = size.y;
    }

    void Display::set_title(std::string title) {
        glfwSetWindowTitle(_window, title.c_str());
    }

    void Display::set_fullscreen(bool fullscreen) {
        const GLFWvidmode *mode = glfwGetVideoMode(_monitor);
        if (fullscreen) {
            glfwSetWindowMonitor(_window,
                                 _monitor,
                                 mode->width / 2,
                                 mode->height / 2,
                                 mode->width,
                                 mode->height,
                                 mode->refreshRate);
        } else {
            glfwSetWindowMonitor(_window,
                                 nullptr,
                                 (mode->width - _size.x) / 2,
                                 (mode->height - _size.y) / 2,
                                 _size.x,
                                 _size.y,
                                 0);
        }
    }

    void Display::set_vsync(bool vsync) {
        _vsync = vsync;
        if (vsync) {
            glfwSwapInterval(1);
        } else {
            glfwSwapInterval(0);
        }
    }

    void Display::set_icon(std::string filename) {
        int width, height, channels;
        stbi_uc *pixels = stbi_load(filename.c_str(),
                                    &width,
                                    &height,
                                    &channels,
                                    STBI_rgb_alpha);
        if (!pixels) {
            Messenger::error("Could not load window icon file `" + filename +
                                 "`",
                             false);
            return;
        }
        GLFWimage image = {
            width,
            height,
            pixels,
        };
        glfwSetWindowIcon(_window, 1, &image);
    }

    void Display::refresh() { glfwPollEvents(); }
} // namespace Dynamo