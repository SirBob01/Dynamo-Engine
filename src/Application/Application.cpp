#include "Application.hpp"

namespace Dynamo {
    Application::Application(ApplicationConfiguration config) {
        // Initialize GLFW
        if (!glfwInit()) {
            Log::error("Failed to initialize GLFW.");
        }

        // Core submodules
        _assets = std::make_unique<AssetCache>(config.asset_directory);
        _display = std::make_unique<Display>(
            config.width,
            config.height,
            config.title,
            config.flags & ApplicationFlag::FullScreen,
            config.flags & ApplicationFlag::VSync);
        _input = std::make_unique<Input>(*_display);
        _clock = std::make_unique<Clock>();

        // Graphics and sound submodules
        _renderer = std::make_unique<Graphics::Vulkan::Renderer>(*_display);
        _jukebox = std::make_unique<Sound::Jukebox>();

        // Seed the random number generator
        Random::seed(_clock->epoch().count());
    }

    Application::~Application() { glfwTerminate(); }

    AssetCache &Application::get_assets() { return *_assets; }

    Core Application::get_core() { return {*_display, *_input, *_clock}; }

    Graphics::Renderer &Application::get_renderer() { return *_renderer; }

    Sound::Jukebox &Application::get_jukebox() { return *_jukebox; }

    b8 Application::is_running() { return !_display->is_closed(); }

    void Application::run() {
        // TODO: Use semi-fixed timestep to update game logic
        // (https://gafferongames.com/post/fix_your_timestep/)
        _input->poll();
        _clock->tick();

        // Process audio and graphics in parallel
        _threads.submit([this]() { _jukebox->update(); });
        _threads.submit([this]() { _renderer->refresh(); });
        _threads.wait_all();
    }
} // namespace Dynamo