#include "Clock.hpp"

namespace Dynamo {
    Clock::Clock() {
        _epoch = std::chrono::steady_clock::now().time_since_epoch();
        _delta = Seconds{0};
        _prev = time();

        _frames = 0;
    }

    Seconds Clock::time() {
        Seconds now = std::chrono::steady_clock::now().time_since_epoch();
        return now - _epoch;
    }

    Seconds Clock::delta() { return _delta; }

    Seconds Clock::epoch() { return _epoch; }

    i64 Clock::frames() { return _frames; }

    void Clock::tick() {
        // Calculate delta time
        Seconds now = time();
        _delta = now - _prev;
        _prev = now;

        _frames++;
    }
} // namespace Dynamo