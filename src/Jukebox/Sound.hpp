#pragma once
#define STB_VORBIS_HEADER_ONLY

#include <string>

#include <stb_vorbis.c>

#include "../Log/Log.hpp"

namespace Dynamo {
    /**
     * @brief An array of PCM values, a discrete representation of a sound wave
     *
     * Each value ranges between [-1.0, 1.0]
     *
     */
    using WaveForm = std::vector<float>;

    /**
     * @brief A sound resource
     *
     */
    struct Sound {
        WaveForm waveform;
        int channels;

        /**
         * @brief Construct a new Sound object from a file
         *
         * @param filename
         */
        Sound(std::string filename);
    };
} // namespace Dynamo