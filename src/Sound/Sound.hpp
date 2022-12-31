#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <vector>

#include "../Log/Log.hpp"
#include "../Math/Fourier.hpp"
#include "../Utils/ChannelData.hpp"

namespace Dynamo::Sound {
    /**
     * @brief Indiviual sample ranges between [−1.0, +1.0]
     *
     */
    using WaveSample = float;

    /**
     * @brief An array of samples, a discrete representation of a sound wave
     *
     * Sources with multiple channels may have their values interleaved
     *
     */
    using WaveForm = std::vector<WaveSample>;

    /**
     * @brief The default sample rate is defined to be 44.1KHz
     *
     */
    constexpr float DEFAULT_SAMPLE_RATE = 44100;

    /**
     * @brief Sound asset represented as a signal holding multiple channels of
     * WaveSample data
     *
     */
    class Sound : public ChannelData<WaveSample> {
        float _sample_rate;

      public:
        /**
         * @brief Construct a new Sound object
         *
         * @param frames      Number of frames
         * @param channels    Number of channels
         * @param sample_rate Sample rate
         */
        Sound(unsigned frames = 0,
              unsigned channels = 0,
              float sample_rate = DEFAULT_SAMPLE_RATE) :
            ChannelData<WaveSample>(frames, channels),
            _sample_rate(sample_rate) {}

        /**
         * @brief Construct a new Sound object from an existing buffer
         *
         * @param samples     Sample buffer
         * @param channels    Number of channels
         * @param sample_rate Sample rate
         */
        Sound(std::vector<WaveSample> samples,
              unsigned channels,
              float sample_rate) :
            ChannelData<WaveSample>(samples, channels),
            _sample_rate(sample_rate) {}

        /**
         * @brief Get the sample rate of the signal
         *
         * @return float
         */
        inline float sample_rate() const { return _sample_rate; }
    };
} // namespace Dynamo::Sound