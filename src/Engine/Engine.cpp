#include "Engine.hpp"

namespace Dynamo {
    Engine::Engine(std::string title, int width, int height, EngineFlag flags) {
        // Initialize GLFW
        if (!glfwInit()) {
            Messenger::error("Failed to initialize GLFW.");
        }

        _display = std::make_unique<Display>(width,
                                             height,
                                             title,
                                             flags & EngineFlag::FullScreen);
    }

    Engine::~Engine() { glfwTerminate(); }

    void Engine::run() {
        while (!glfwWindowShouldClose(&_display->get_window())) {
            glfwPollEvents();
        }
    }
} // namespace Dynamo