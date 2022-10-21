#pragma once

#include <vector>

namespace Dynamo {
    /**
     * @brief Internal sample rate used by the Jukebox engine
     *
     * All loaded sounds are resampled to match this rate for consistency
     *
     */
    constexpr unsigned int SAMPLE_RATE = 44100;

    /**
     * @brief An array of PCM values, a discrete representation of a sound wave
     *
     * Each value ranges between [−32,767, +32,767] and sources with multiple
     * channels may have their values interleaved
     *
     */
    using WaveForm = std::vector<short>;

    /**
     * @brief A sound asset
     *
     */
    struct Sound {
        /**
         * @brief PCM data
         *
         */
        WaveForm waveform;

        /**
         * @brief Number of channels interleaved within the waveform
         *
         * This determines the size of a single frame
         *
         */
        int channels;
    };
} // namespace Dynamo