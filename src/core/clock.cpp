#include "clock.h"

namespace Dynamo {
    Clock::Clock() {
        frames_ = 0;
        previous_ = 0;
        current_ = 0;

        delta_ = 0;
        delta_cap_ = 100;

        fps_ = 0.0f;
        fps_start_ = 0;
        fps_period_ = 30;
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

    float Clock::get_fps() {
        return fps_;
    }

    void Clock::set_fps(unsigned fps) {
        // Delay each loop iteration to maintain framerate
        // FPS cap 0 allows machine to run app as fast as possible
        if(fps) {
            float cap_time = 1000.0/fps;
            if(delta_ < cap_time) {
                SDL_Delay(cap_time - delta_);
            }
        }
        previous_ = current_;
    }

    void Clock::tick() {
        // Call once only at the start of the game loop
        frames_++;
        current_ = SDL_GetTicks();
        delta_ = current_ - previous_;

        // Count fps at a certain frequency
        if(frames_%fps_period_ == 0) {
            fps_ = fps_period_ / ((current_ - fps_start_) / 1000.0);
            fps_start_ = current_;
        }
        
        if(delta_ > delta_cap_) { // Protect integration from breaking
            delta_ = delta_cap_;
        }
    }
}