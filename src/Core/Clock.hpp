#pragma once

#include <GLFW/glfw3.h>

namespace Dynamo {
    /**
     * @brief Internal timing system
     *
     */
    class Clock {
        int _frames;
        int _fps_period;

        double _fps_start;
        double _fps;

        double _delta;
        double _last_time;

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
         * @return double
         */
        double get_time();

        /**
         * @brief Get the high precision delta time for the current frame in
         * seconds
         *
         * @return double
         */
        double get_delta();

        /**
         * @brief Get the current FPS averaged over a fixed number of frames
         *
         * @return double
         */
        double get_fps();

        /**
         * @brief Get the number of frames since the engine was initialized
         *
         * This is simply a counter that is incremented every tick
         *
         * @return int
         */
        int get_frames();

        /**
         * @brief Update the clock
         *
         */
        void tick();
    };
} // namespace Dynamo