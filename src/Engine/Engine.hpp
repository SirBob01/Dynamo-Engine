#pragma once

#include "../Core/Clock.hpp"
#include "../Core/Core.hpp"
#include "../Core/Display.hpp"
#include "../Core/Input.hpp"
#include "../Messenger/Messenger.hpp"
#include "../Utils/Random.hpp"
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
        std::unique_ptr<Input> _input;
        std::unique_ptr<Clock> _clock;

      public:
        /**
         * @brief Construct a new Engine object
         *
         * @param title  Title of the window
         * @param width  Initial width of the display (0 if width of screen)
         * @param height Initial height of the display (0 if height of screen)
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
         * @brief Get the core modules
         *
         * @return const Core
         */
        Core get_core();

        /**
         * @brief Is the application still running?
         *
         * @return true
         * @return false
         */
        bool is_running();

        /**
         * @brief Perform a single frame update
         *
         */
        void run();
    };
} // namespace Dynamo