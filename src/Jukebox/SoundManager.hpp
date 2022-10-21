#pragma once
#define STB_VORBIS_HEADER_ONLY

#include <string>

#include <stb_vorbis.c>

#include "../Asset/AssetManager.hpp"
#include "../Log/Log.hpp"
#include "./DSP.hpp"
#include "./Sound.hpp"

namespace Dynamo {
    /**
     * @brief Sound asset manager
     *
     */
    class SoundManager : public AssetManager<Sound> {
      public:
        /**
         * @brief Load a sound from disk
         *
         * @param filename
         * @return Asset<Sound>
         */
        Asset<Sound> load_file(std::string filename);

        /**
         * @brief Create a sound asset from raw PCM data
         *
         * @param waveform    PCM data
         * @param channels    Number of interleaved channels
         * @param sample_rate Sample rate of the waveform
         * @return Asset<Sound>
         */
        Asset<Sound>
        load_raw(WaveForm waveform, int channels, unsigned int sample_rate);
    };
} // namespace Dynamo