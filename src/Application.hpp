#pragma once

#include <functional>
#include <string>
#include <thread>

#include "./Sound/Jukebox.hpp"
#include "Clock.hpp"
#include "Display.hpp"

namespace Dynamo {
    /**
     * @brief Application setup options.
     *
     */
    struct ApplicationSettings {
        /**
         * @brief Application title.
         *
         */
        std::string title;

        /**
         * @brief Width of the display window.
         *
         */
        unsigned window_width;

        /**
         * @brief Height of the display window.
         *
         */
        unsigned window_height;

        /**
         * @brief Root asset directory.
         *
         */
        std::string root_asset_directory;
    };

    /**
     * @brief Dynamo Application.
     *
     */
    class Application {
        std::unique_ptr<Display> _display;
        std::unique_ptr<Sound::Jukebox> _jukebox;

        std::thread _audio_thread;

        Clock _clock;

      public:
        /**
         * @brief Initialize a new Application.
         *
         * @param settings
         */
        Application(const ApplicationSettings &settings);
        ~Application();

        /**
         * @brief Check if the application is running.
         *
         * @return true
         * @return false
         */
        bool is_running() const;

        /**
         * @brief Get the display.
         *
         * @return Display&
         */
        Display &get_display();

        /**
         * @brief Get the input handler.
         *
         * @return Input&
         */
        Input &get_input();

        /**
         * @brief Get the clock.
         *
         * @return Clock&
         */
        Clock &get_clock();

        /**
         * @brief Get the audio engine.
         *
         * @return Sound::Jukebox&
         */
        Sound::Jukebox &get_jukebox();

        /**
         * @brief Update the application state.
         *
         */
        void update();
    };

    /**
     * @brief Application reference.
     *
     */
    using ApplicationRef = std::reference_wrapper<Application>;
} // namespace Dynamo