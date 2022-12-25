#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <vector>

#include "../Log/Log.hpp"
#include "../Math/Fourier.hpp"
#include "../Utils/ChannelData.hpp"

namespace Dynamo {
    /**
     * @brief Indiviual sample ranges between [−32,767, +32,767]
     *
     */
    using WaveSample = short;

    /**
     * @brief An array of samples, a discrete representation of a sound wave
     *
     * Sources with multiple channels may have their values interleaved
     *
     */
    using WaveForm = std::vector<WaveSample>;

    /**
     * @brief Frame in the audio waveform containing mono or stereo samples
     *
     */
    struct WaveFrame {
        std::array<WaveSample, 2> samples;
        unsigned channels;
    };

    /**
     * @brief Channels in a waveform
     *
     */
    enum class Channel {
        FL = 0,
        FC = 1,
        FR = 2,
        SL = 3,
        SR = 4,
        BL = 5,
        BC = 6,
        BR = 7,
        LFE = 8
    };

    /**
     * @brief Order of the channels depending on number of channels
     *
     */
    static const std::vector<Channel> CHANNEL_ORDERS[8] = {
        {Channel::FC},
        {Channel::FL, Channel::FR},
        {Channel::FL, Channel::FC, Channel::FR},
        {Channel::FL, Channel::FR, Channel::BL, Channel::BR},
        {Channel::FL, Channel::FC, Channel::FR, Channel::BL, Channel::BR},
        {Channel::FL,
         Channel::FC,
         Channel::FR,
         Channel::BL,
         Channel::BR,
         Channel::LFE},
        {Channel::FL,
         Channel::FC,
         Channel::FR,
         Channel::BL,
         Channel::BR,
         Channel::BC,
         Channel::LFE},
        {Channel::FL,
         Channel::FC,
         Channel::FR,
         Channel::SL,
         Channel::SR,
         Channel::BL,
         Channel::BR,
         Channel::LFE},
    };

    /**
     * @brief Gain coefficients for downmixing a waveform to mono or stereo
     *
     */
    static const double DOWNMIX_COEFFS[2][9][2] = {
        {
            {M_SQRT1_2},
            {1},
            {M_SQRT1_2},
            {0.5},
            {0.5},
            {0.5},
            {0.25},
            {0.5},
            {0.25},
        },
        {
            {1, 0},
            {M_SQRT1_2, M_SQRT1_2},
            {0, 1},
            {M_SQRT1_2, 0},
            {0, M_SQRT1_2},
            {M_SQRT1_2, 0},
            {0.5, 0.5},
            {0, M_SQRT1_2},
            {0.5, 0.5},
        },
    };

    /**
     * @brief The default sample rate is defined to be 44.1KHz
     *
     */
    constexpr double DEFAULT_SAMPLE_RATE = 44100;

    /**
     * @brief Sound asset represented as a signal holding multiple channels of
     * WaveSample data
     *
     */
    class Sound : public ChannelData<WaveSample> {
        double _sample_rate;

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
              double sample_rate = DEFAULT_SAMPLE_RATE) :
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
              double sample_rate) :
            ChannelData<WaveSample>(samples, channels),
            _sample_rate(sample_rate) {}

        /**
         * @brief Get the sample rate of the signal
         *
         * @return double
         */
        inline double sample_rate() const { return _sample_rate; }

        /**
         * @brief Grab a frame in the waveform and upmix or downmix to the
         * desired number of channels
         *
         * @param frame        Frame index
         * @param out_channels Target number of channels
         * @return WaveFrame
         */
        WaveFrame get_frame(const unsigned frame, const unsigned out_channels);
    };

    /**
     * @brief Normalize a wave sample
     *
     * @param sample Native sample format representation
     * @return double
     */
    inline double norm_sample(const WaveSample sample) {
        constexpr double scale = 1.0 / __INT16_MAX__;
        return sample * scale;
    }

    /**
     * @brief Clip and denormalize a wave sample
     *
     * @param sample Normalized double representation
     * @return WaveSample
     */
    inline WaveSample denorm_sample(const double sample) {
        return std::clamp(sample, -1.0, 1.0) * __INT16_MAX__;
    }
} // namespace Dynamo