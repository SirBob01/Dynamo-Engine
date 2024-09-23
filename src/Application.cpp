#include "Application.hpp"

namespace Dynamo {
    Application::Application(const ApplicationSettings &settings) {
        _display = std::make_unique<Display>(settings.title,
                                             settings.window_width,
                                             settings.window_height);
        _jukebox = std::make_unique<Sound::Jukebox>();
    }

    bool Application::is_running() const { return _display->is_open(); }

    Display &Application::get_display() { return *_display; }

    Input &Application::get_input() { return _display->get_input(); }

    Clock &Application::get_clock() { return _clock; }

    Sound::Jukebox &Application::get_jukebox() { return *_jukebox; }

    void Application::update() {
        // Update subsystems
        _display->get_input().poll();

        // Tick
        _clock.tick();
    }
} // namespace Dynamo