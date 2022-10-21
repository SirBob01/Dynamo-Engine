#pragma once

#include "./Sound.hpp"

namespace Dynamo {
    /**
     * @brief Convert a waveform from its source sample rate to its destination
     * sample rate
     *
     * This will modify the waveform in-place
     *
     * @param waveform Raw PCM data
     * @param channels Number of interleaved channels
     * @param src      Source sample rate
     * @param dst      Target sample rate
     */
    void resample_waveform(WaveForm &waveform,
                           int channels,
                           unsigned int src,
                           unsigned int dst);
} // namespace Dynamo