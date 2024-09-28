#include <Application.hpp>

namespace Dynamo {
    Application::Application(const ApplicationSettings &settings) {
        _display = std::make_unique<Display>(settings.title,
                                             settings.window_width,
                                             settings.window_height);
        _jukebox = std::make_unique<Sound::Jukebox>();

        // Run audio on a separate thread
        _audio_thread = std::thread([&]() {
            while (is_running()) {
                _jukebox->update();
            }
        });
    }

    Application::~Application() {
        if (_audio_thread.joinable()) {
            _audio_thread.join();
        }
    }

    bool Application::is_running() const { return _display->is_open(); }

    Display &Application::display() { return *_display; }

    Input &Application::input() { return _display->get_input(); }

    Clock &Application::clock() { return _clock; }

    Sound::Jukebox &Application::jukebox() { return *_jukebox; }

    void Application::update() {
        // Poll for input
        _display->get_input().poll();

        // Tick
        _clock.tick();
    }
} // namespace Dynamo