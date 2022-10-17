#pragma once

#define JUKEBOX_SAMPLE_RATE 44100

#include <memory>
#include <queue>
#include <string>
#include <vector>

#include <portaudio.h>

#include "../Log/Log.hpp"
#include "../Math/Vec3.hpp"

namespace Dynamo {
    /**
     * @brief Raw sound data is an array of PCM values
     *
     * Each PCM value ranges between [-1.0, 1.0]
     *
     */
    using Sound = std::vector<float>;

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
         * @brief Callback for passing data between the runtime and the sound
         * devices
         *
         * @param input        Input sound stream
         * @param output       Output sound stream
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
    };
} // namespace Dynamo