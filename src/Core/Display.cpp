#define STB_IMAGE_IMPLEMENTATION
#include "Display.hpp"

namespace Dynamo {
    Display::Display(i32 width,
                     i32 height,
                     std::string title,
                     b8 fullscreen,
                     b8 vsync) {
        _monitor = glfwGetPrimaryMonitor();
        if (!_monitor) {
            Log::error("GLFW could not find primary monitor device.");
        }
        const GLFWvidmode *mode = glfwGetVideoMode(_monitor);
        if (width == 0) width = mode->width;
        if (height == 0) height = mode->height;

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        _window =
            glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        _size = Vec2(width, height);
        _vsync = vsync;
        _title = title;

        if (!_window) {
            Log::error("Failed to create a GLFW window");
        }
        glfwMakeContextCurrent(_window);

        set_fullscreen(fullscreen);
        set_vsync(vsync);
    }

    Display::~Display() { glfwDestroyWindow(_window); }

    GLFWwindow *Display::get_window() { return _window; }

    std::vector<const i8 *> Display::get_vulkan_extensions() const {
        std::vector<const i8 *> extensions;
        u32 count;
        const i8 **arr = glfwGetRequiredInstanceExtensions(&count);
        for (u32 i = 0; i < count; i++) {
            extensions.push_back(arr[i]);
        }
        return extensions;
    }

    vk::SurfaceKHR Display::get_vulkan_surface(vk::Instance instance) const {
        VkSurfaceKHR surface;
        vk::Result result = static_cast<vk::Result>(
            glfwCreateWindowSurface(instance, _window, nullptr, &surface));
        if (result != vk::Result::eSuccess) {
            Log::error("Failed to create a Vulkan surface: {}",
                       vk::to_string(result));
        }
        return surface;
    }

    Vec2 Display::get_window_size() const {
        i32 width, height;
        glfwGetWindowSize(_window, &width, &height);
        return Vec2(width, height);
    }

    Vec2 Display::get_framebuffer_size() const {
        i32 width, height;
        glfwGetFramebufferSize(_window, &width, &height);
        return Vec2(width, height);
    }

    Vec2 Display::get_size() const { return _size; }

    std::string Display::get_title() const { return _title; }

    b8 Display::is_closed() const { return glfwWindowShouldClose(_window); }

    b8 Display::is_fullscreen() const {
        return glfwGetWindowMonitor(_window) != nullptr;
    }

    b8 Display::is_vsync() const { return _vsync; }

    void Display::set_size(Vec2 size) {
        _size.x = size.x;
        _size.y = size.y;
    }

    void Display::set_title(std::string title) {
        _title = title;
        glfwSetWindowTitle(_window, _title.c_str());
    }

    void Display::set_fullscreen(b8 fullscreen) {
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

    void Display::set_vsync(b8 vsync) {
        _vsync = vsync;
        if (vsync) {
            glfwSwapInterval(1);
        } else {
            glfwSwapInterval(0);
        }
    }

    void Display::set_icon(std::string filename) {
        i32 width, height, channels;
        stbi_uc *pixels = stbi_load(filename.c_str(),
                                    &width,
                                    &height,
                                    &channels,
                                    STBI_rgb_alpha);
        if (!pixels) {
            Log::warn("Could not load window icon file `{}`", filename);
            return;
        }
        GLFWimage image = {
            width,
            height,
            pixels,
        };
        glfwSetWindowIcon(_window, 1, &image);
    }
} // namespace Dynamo
