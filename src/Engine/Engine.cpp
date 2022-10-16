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
                                             flags & EngineFlag::FullScreen,
                                             flags & EngineFlag::VSync);
    }

    Engine::~Engine() { glfwTerminate(); }

    Core Engine::get_core() { return {*_display}; }

    bool Engine::is_running() { return !_display->is_closed(); }

    void Engine::run() { _display->refresh(); }
} // namespace Dynamo