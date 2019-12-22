#ifndef DYNAMO_CLOCK_H_
#define DYNAMO_CLOCK_H_

#include <SDL2/SDL.h>

namespace Dynamo {
    class Clock {
        unsigned frames_;
        unsigned previous_;
        unsigned current_;

        unsigned delta_;
        unsigned delta_cap_;

    public:
        Clock();

        // Get the current number of frames
        unsigned get_frames();

        // Get milliseconds since SDL init
        unsigned get_time();

        // Get the delta time
        unsigned get_delta();

        // Get the average fps
        float get_fps();

        // Set the framerate cap
        void set_fps(unsigned fps);
        
        // Update the clock
        void tick();

    };
}

#endif