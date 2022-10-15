#include "Display.hpp"

namespace Dynamo {
    Display::Display(int width,
                     int height,
                     std::string title,
                     bool fullscreen) {
        GLFWmonitor *monitor = nullptr;
        if (fullscreen) {
            monitor = glfwGetPrimaryMonitor();
        }
        _window =
            glfwCreateWindow(width, height, title.c_str(), monitor, nullptr);
        if (!_window) {
            Messenger::error("Failed to create a GLFW window");
        }
    }

    Display::~Display() { glfwDestroyWindow(_window); }

    GLFWwindow &Display::get_window() { return *_window; }

    void Display::set_title(std::string title) {
        glfwSetWindowTitle(_window, title.c_str());
    }
} // namespace Dynamo