#pragma once

#include "../Core/Clock.hpp"
#include "../Core/Core.hpp"
#include "../Core/Display.hpp"
#include "../Core/Input.hpp"
#include "../Graphics/Renderer.hpp"
#include "../Graphics/Vulkan/Renderer.hpp"
#include "../Log/Log.hpp"
#include "../Sound/Jukebox.hpp"
#include "../Types.hpp"
#include "../Utils/Random.hpp"
#include "../Utils/ThreadPool.hpp"
#include "ApplicationFlag.hpp"

namespace Dynamo {
    /**
     * @brief Application startup configuration options
     *
     */
    struct ApplicationConfiguration {
        /**
         * @brief Title of the display
         *
         */
        std::string title;

        /**
         * @brief Width of the display
         *
         */
        u32 width;

        /**
         * @brief Height of the display
         *
         */
        u32 height;

        /**
         * @brief Runtime settings
         *
         */
        ApplicationFlag flags = ApplicationFlag::None;

        /**
         * @brief Root asset directory
         *
         */
        std::string asset_directory;
    };

    /**
     * @brief Main runtime synchronizes the game loop, rendering, sound, and
     * scenes
     *
     */
    class Application {
        std::unique_ptr<Display> _display;
        std::unique_ptr<Input> _input;
        std::unique_ptr<Clock> _clock;

        std::unique_ptr<Graphics::Renderer> _renderer;
        std::unique_ptr<Sound::Jukebox> _jukebox;

        ThreadPool _threads;

      public:
        /**
         * @brief Construct a new Application object
         *
         * @param config Initialization options
         */
        Application(ApplicationConfiguration config);

        /**
         * @brief Destroy the Application object
         *
         * This will clean up the scenes and the GLFW instance
         *
         */
        ~Application();

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
        b8 is_running();

        /**
         * @brief Perform a single frame update
         *
         */
        void run();
    };
} // namespace Dynamo