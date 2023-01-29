#include "Clock.hpp"

namespace Dynamo {
    Clock::Clock() {
        _frames = 0;
        _fps_period = 30; // Calculate the FPS over 30 frames

        _fps_start = get_time();
        _fps = 0;

        _delta = 0;
        _last_time = get_time();
    }

    f64 Clock::get_time() { return glfwGetTime(); }

    f64 Clock::get_delta() { return _delta; }

    f64 Clock::get_fps() { return _fps; }

    i32 Clock::get_frames() { return _frames; }

    void Clock::tick() {
        _frames++;

        // Calculate delta time
        f64 now = glfwGetTime();
        _delta = now - _last_time;
        _last_time = now;

        // Calculate FPS
        if (_frames % _fps_period == 0) {
            _fps = _fps_period / (now - _fps_start);
            _fps_start = now;
        }
    }
} // namespace Dynamo