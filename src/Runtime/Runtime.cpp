#include "Runtime.hpp"

namespace Dynamo {
    Runtime::Runtime(std::string title,
                     int width,
                     int height,
                     RuntimeFlag flags) {
        // Initialize GLFW
        if (!glfwInit()) {
            Log::error("Failed to initialize GLFW.");
        }

        // Core submodules
        _display = std::make_unique<Display>(width,
                                             height,
                                             title,
                                             flags & RuntimeFlag::FullScreen,
                                             flags & RuntimeFlag::VSync);
        _input = std::make_unique<Input>(*_display);
        _clock = std::make_unique<Clock>();

        // Graphics and sound submodules
        _renderer = std::make_unique<Graphics::VkRenderer>(*_display);
        _jukebox = std::make_unique<Sound::Jukebox>();

        // Seed the random number generator
        int seed = std::chrono::system_clock::now().time_since_epoch().count();
        Random::seed(seed);
    }

    Runtime::~Runtime() { glfwTerminate(); }

    Core Runtime::get_core() { return {*_display, *_input, *_clock}; }

    Graphics::Renderer &Runtime::get_renderer() { return *_renderer; }

    Sound::Jukebox &Runtime::get_jukebox() { return *_jukebox; }

    bool Runtime::is_running() { return !_display->is_closed(); }

    void Runtime::run() {
        // TODO: Use semi-fixed timestep to update game logic
        // (https://gafferongames.com/post/fix_your_timestep/)
        _input->poll();
        _clock->tick();
        _jukebox->update();
    }
} // namespace Dynamo