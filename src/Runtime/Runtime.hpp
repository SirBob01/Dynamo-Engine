#pragma once

#include "RuntimeFlag.hpp"

#include "../Core/Clock.hpp"
#include "../Core/Core.hpp"
#include "../Core/Display.hpp"
#include "../Core/Input.hpp"

#include "../Graphics/Renderer.hpp"
#include "../Graphics/Vulkan/VkRenderer.hpp"

#include "../Sound/Jukebox.hpp"

#include "../Log/Log.hpp"
#include "../Utils/Random.hpp"

namespace Dynamo {
    /**
     * @brief Main runtime synchronizes the game loop, rendering, sound, and
     * scenes
     *
     */
    class Runtime {
        std::unique_ptr<Display> _display;
        std::unique_ptr<Input> _input;
        std::unique_ptr<Clock> _clock;

        std::unique_ptr<Graphics::Renderer> _renderer;
        std::unique_ptr<Sound::Jukebox> _jukebox;

      public:
        /**
         * @brief Construct a new Runtime object
         *
         * @param title  Title of the window
         * @param width  Initial width of the display (0 if width of screen)
         * @param height Initial height of the display (0 if height of screen)
         * @param flags  Initial configuration settings
         */
        Runtime(std::string title,
                int width = 0,
                int height = 0,
                RuntimeFlag flags = RuntimeFlag::None);

        /**
         * @brief Destroy the Runtime object
         *
         * This will clean up the scenes and the GLFW instance
         *
         */
        ~Runtime();

        /**
         * @brief Get the core modules
         *
         * @return const Core
         */
        Core get_core();

        /**
         * @brief Get a reference to the audio engine
         *
         * @return Sound::Jukebox&
         */
        Sound::Jukebox &get_jukebox();

        /**
         * @brief Get a reference to the rendering engine
         *
         * @return Graphics::Renderer&
         */
        Graphics::Renderer &get_renderer();

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