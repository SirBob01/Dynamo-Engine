#pragma once

#include <chrono>

#include "../Types.hpp"

namespace Dynamo {
    /**
     * @brief Time duration in seconds.
     *
     */
    using Seconds = std::chrono::duration<f64>;

    /**
     * @brief Internal clock for querying per-frame time information.
     *
     */
    class Clock {
        Seconds _epoch;
        Seconds _delta;
        Seconds _prev;

        i64 _frames;

      public:
        /**
         * @brief Construct a new Clock object.
         *
         */
        Clock();

        /**
         * @brief Get the total time in seconds since initialization.
         *
         * @return Seconds
         */
        Seconds time();

        /**
         * @brief Get the delta time in seconds since the previous frame.
         *
         * @return Seconds
         */
        Seconds delta();

        /**
         * @brief Get the number of frames since initialization.
         *
         * @return i64
         */
        i64 frames();

        /**
         * @brief Update the clock.
         *
         */
        void tick();
    };
} // namespace Dynamo