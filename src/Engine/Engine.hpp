#pragma once

#include <GLFW/glfw3.h>

#include "../Core/Display.hpp"
#include "../Messenger/Messenger.hpp"
#include "EngineFlag.hpp"

namespace Dynamo {
    /**
     * @brief The main engine
     *
     * This handles the runtime by updating the game loop, rendering, and
     * managing scenes
     *
     */
    class Engine {
        std::unique_ptr<Display> _display;

      public:
        /**
         * @brief Construct a new Engine object
         *
         * @param title  Title of the window
         * @param width  Initial width of the display
         * @param height Initial height of the display
         * @param flags  Initial configuration settings
         */
        Engine(std::string title,
               int width = 0,
               int height = 0,
               EngineFlag flags = EngineFlag::None);

        /**
         * @brief Destroy the Engine object
         *
         * This will clean up the scenes and the GLFW instance
         *
         */
        ~Engine();

        /**
         * @brief Perform a single frame update
         *
         */
        void run();
    };
} // namespace Dynamo