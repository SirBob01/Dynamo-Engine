#pragma once

#include <array>
#include <string>

#include <sndfile.hh>

#include "../Asset/AssetManager.hpp"
#include "../Log/Log.hpp"
#include "./Resample.hpp"
#include "./Sound.hpp"

namespace Dynamo::Sound {
    /**
     * @brief Sound asset manager
     *
     */
    class SoundManager : public AssetManager<Sound> {
      public:
        /**
         * @brief Load a sound from a file
         *
         * @param filename
         * @return Asset<Sound>
         */
        Asset<Sound> load_file(const std::string filename);

        /**
         * @brief Create a sound asset from raw PCM data
         *
         * This is an alias of the allocate() method
         *
         * @param waveform    PCM data
         * @param channels    Number of interleaved channels
         * @param sample_rate Sample rate of the waveform
         * @return Asset<Sound>
         */
        Asset<Sound> load_raw(const WaveForm waveform,
                              const unsigned channels,
                              const float sample_rate);
    };
} // namespace Dynamo::Sound