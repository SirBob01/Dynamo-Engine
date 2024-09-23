#include "Clock.hpp"

namespace Dynamo {
    Clock::Clock() {
        _start = std::chrono::steady_clock::now().time_since_epoch();
        _delta = Seconds(0);
        _prev = _start;

        _frames = 0;
    }

    TimePoint Clock::time() { return std::chrono::system_clock::now(); }

    Seconds Clock::elapsed() const {
        Seconds now = std::chrono::steady_clock::now().time_since_epoch();
        return now - _start;
    }

    Seconds Clock::delta() const { return _delta; }

    unsigned long long Clock::frames() const { return _frames; }

    void Clock::tick() {
        Seconds now = std::chrono::steady_clock::now().time_since_epoch();
        _delta = now - _prev;
        _prev = now;

        _frames++;
    }
} // namespace Dynamo