#pragma once

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
    };
} // namespace Dynamo