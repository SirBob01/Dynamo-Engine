#pragma once

#include "Display.hpp"

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
    };
} // namespace Dynamo