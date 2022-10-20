#pragma once

namespace Dynamo {
    /**
     * @brief An array of PCM values, a discrete representation of a sound wave
     *
     * Each value ranges between [-1.0, 1.0] and sources with multiple channels
     * may have their values interleaved
     *
     */
    using WaveForm = std::vector<float>;

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