#pragma once

#include <GLFW/glfw3.h>

#include "../Types.hpp"

namespace Dynamo {
    /**
     * @brief Internal timing system
     *
     */
    class Clock {
        i32 _frames;
        i32 _fps_period;

        f64 _fps_start;
        f64 _fps;

        f64 _delta;
        f64 _last_time;

      public:
        /**
         * @brief Construct a new Clock object
         *
         */
        Clock();

        /**
         * @brief Get the high precision time since the engine was initialized
         * in seconds
         *
         * @return f64
         */
        f64 get_time();

        /**
         * @brief Get the high precision delta time for the current frame in
         * seconds
         *
         * @return f64
         */
        f64 get_delta();

        /**
         * @brief Get the current FPS averaged over a fixed number of frames
         *
         * @return f64
         */
        f64 get_fps();

        /**
         * @brief Get the number of frames since the engine was initialized
         *
         * This is simply a counter that is incremented every tick
         *
         * @return i32
         */
        i32 get_frames();

        /**
         * @brief Update the clock
         *
         */
        void tick();
    };
} // namespace Dynamo