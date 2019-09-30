#ifndef DYNAMO_CLOCK_H_
#define DYNAMO_CLOCK_H_

#include <SDL2/SDL.h>

namespace Dynamo {
    class Clock {
        int frames_;
        int previous_;
        int current_;

        float delta_;

    public:
        Clock();

        // Get the current number of frames
        int get_frames();

        // Get the delta time
        float get_delta();

        // Update the clock
        void tick();

        // Set the framerate cap
        void set_fps(int fps);
    };
}

#endif