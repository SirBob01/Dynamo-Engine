#pragma once

#include <memory>
#include <queue>
#include <string>
#include <vector>

#include <portaudio.h>

#include "../Asset/Asset.hpp"
#include "../Log/Log.hpp"
#include "../Math/Vec3.hpp"
#include "../Utils/RingBuffer.hpp"

#include "./Sound.hpp"
#include "./SoundManager.hpp"

namespace Dynamo {
    /**
     * @brief Size of the input and output ring buffers
     *
     */
    constexpr unsigned int BUFFER_SIZE = 1 << 12;

    /**
     * @brief A stream on which audio tracks can be played
     *
     */
    using SoundStream = unsigned int;

    /**
     * @brief In-house audio engine supporting static sounds, spatial audio, and
     * multilayer tracks
     *
     * To be performant, Jukebox assigns a worker thread for audio mixing
     * processes.
     *
     */
    class Jukebox {
        SoundManager &_assets;
        PaStream *_stream;

        float _volume;

        bool _playing;
        bool _recording;

        /**
         * @brief Internal state
         *
         */
        struct State {
            RingBuffer<short, BUFFER_SIZE> input_buffer;
            RingBuffer<short, BUFFER_SIZE> output_buffer;

            int input_channels;
            int output_channels;
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
        Jukebox(SoundManager &assets);

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
         * @brief Pause audio playback
         *
         */
        void pause();

        /**
         * @brief Resume audio playback
         *
         */
        void resume();

        /**
         * @brief Set the master volume
         *
         * @param volume
         */
        void set_volume(float volume);

        /**
         * @brief Play a static sound
         *
         * @param sound  Sound asset
         * @param volume Local volume
         * @param start  Starting time in seconds
         */
        void play_static(Asset<Sound> &sound, float volume, float start = 0);
    };
} // namespace Dynamo