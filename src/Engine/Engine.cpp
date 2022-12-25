#include "Engine.hpp"

namespace Dynamo {
    Engine::Engine(std::string title, int width, int height, EngineFlag flags) {
        // Initialize GLFW
        if (!glfwInit()) {
            Log::error("Failed to initialize GLFW.");
        }

        // Core submodules
        _display = std::make_unique<Display>(width,
                                             height,
                                             title,
                                             flags & EngineFlag::FullScreen,
                                             flags & EngineFlag::VSync);
        _input = std::make_unique<Input>(*_display);
        _clock = std::make_unique<Clock>();

        // Rendering submodules
        _jukebox = std::make_unique<Jukebox>();

        // Seed the random number generator
        int seed = std::chrono::system_clock::now().time_since_epoch().count();
        Random::seed(seed);
    }

    Engine::~Engine() { glfwTerminate(); }

    Core Engine::get_core() { return {*_display, *_input, *_clock}; }

    Jukebox &Engine::get_jukebox() { return *_jukebox; }

    bool Engine::is_running() { return !_display->is_closed(); }

    void Engine::run() {
        // TODO: Use semi-fixed timestep to update game logic
        // (https://gafferongames.com/post/fix_your_timestep/)
        _input->poll();
        _clock->tick();
        _jukebox->update();
    }
} // namespace Dynamo