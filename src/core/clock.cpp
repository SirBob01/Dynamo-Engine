#include "clock.h"

namespace Dynamo {
    Clock::Clock() {
        frames_ = 0;
        previous_ = 0;
        current_ = 0;

        delta_ = 0;
        delta_cap_ = 30;
    }

    unsigned Clock::get_frames() {
        return frames_;
    }

    unsigned Clock::get_time() {
        return current_;
    }

    unsigned Clock::get_delta() {
        return delta_;
    }

    void Clock::tick() {
        // Call once only at the start of the game loop
        frames_++;
        current_ = SDL_GetTicks();
        delta_ = current_ - previous_;
        if(delta_ > delta_cap_) { // Protect integration from breaking
            delta_ = delta_cap_;
        }
    }

    void Clock::set_fps(unsigned fps) {
        // Delay each loop iteration to maintain framerate
        float cap_time = 1000.0/fps;
        if(delta_ < cap_time) {
            SDL_Delay(cap_time - delta_);
        }
        
        previous_ = current_;
    }
}