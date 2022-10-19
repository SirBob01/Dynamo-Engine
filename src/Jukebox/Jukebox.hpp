#pragma once

#define JUKEBOX_SAMPLE_RATE 44100

#include <memory>
#include <queue>
#include <string>
#include <vector>

#include <portaudio.h>

#include "../Log/Log.hpp"
#include "../Math/Vec3.hpp"
#include "./Sound.hpp"

namespace Dynamo {
    /**
     * @brief A stream on which audio tracks can be played
     *
     */
    using SoundStream = unsigned int;

    /**
     * @brief In-house audio engine
     *
     * Supports sound effects, streaming tracks, and spatial audio
     *
     */
    class Jukebox {
        PaStream *_stream;

        /**
         * @brief Internal state
         *
         */
        struct State {
            WaveForm base;

            int input_channels;
            int output_channels;

            float volume;
            bool playing;
            bool recording;
        };
        State _state;

        /**
         * @brief Callback for passing data between the runtime and the sound
         * devices
         *
         * @param input        Input sound buffer (recording device)
         * @param output       Output sound buffer (speaker device)
         * @param frame_count  Frames per buffer
         * @param time_info    Time information in seconds
         * @param status_flags Status flags
         * @param data         User data
         * @return int
         */
        static int stream_callback(const void *input,
                                   void *output,
                                   unsigned long frame_count,
                                   const PaStreamCallbackTimeInfo *time_info,
                                   PaStreamCallbackFlags status_flags,
                                   void *data);

      public:
        /**
         * @brief Construct a new Jukebox object
         *
         */
        Jukebox();

        /**
         * @brief Destroy the Jukebox object
         *
         */
        ~Jukebox();

        /**
         * @brief Is the output device playing?
         *
         * @return true
         * @return false
         */
        bool is_playing();

        /**
         * @brief Is the input device listening
         *
         * @return true
         * @return false
         */
        bool is_recording();

        /**
         * @brief Get the master volume
         *
         * @return float
         */
        float get_volume();

        /**
         * @brief Set the master volume
         *
         * @param volume
         */
        void set_volume(float volume);
    };
} // namespace Dynamo