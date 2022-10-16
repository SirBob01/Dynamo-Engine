#pragma once

#include "Clock.hpp"
#include "Display.hpp"
#include "Input.hpp"

namespace Dynamo {
    /**
     * @brief Core modules used in every scene
     *
     */
    struct Core {
        /**
         * @brief Display instance
         *
         */
        Display &display;

        /**
         * @brief Input handler
         *
         */
        Input &input;

        /**
         * @brief Time manager
         *
         */
        Clock &clock;
    };
} // namespace Dynamo