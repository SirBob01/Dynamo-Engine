#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <vector>

#include "../Log/Log.hpp"
#include "../Math/Fourier.hpp"
#include "../Types.hpp"
#include "../Utils/ChannelData.hpp"

namespace Dynamo::Sound {
    /**
     * @brief Indiviual sample ranges between [−1.0, +1.0]
     *
     */
    using WaveSample = f32;

    /**
     * @brief An array of samples, a discrete representation of a sound wave.
     *
     * Sources with multiple channels are stored in a planar format.
     *
     */
    using WaveForm = std::vector<WaveSample>;

    /**
     * @brief Frame in the audio waveform containing mono or stereo samples
     *
     */
    struct WaveFrame {
        std::array<WaveSample, 2> samples;
        u32 channels;
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
    static const f64 DOWNMIX_COEFFS[2][9][2] = {
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
    static constexpr f32 DEFAULT_SAMPLE_RATE = 44100;

    /**
     * @brief Sound asset represented as a signal holding multiple channels of
     * WaveSample data
     *
     */
    class Sound : public ChannelData<WaveSample> {
        f32 _sample_rate;

      public:
        /**
         * @brief Construct a new Sound object
         *
         * @param frames      Number of frames
         * @param channels    Number of channels
         * @param sample_rate Sample rate
         */
        Sound(u32 frames = 0,
              u32 channels = 0,
              f32 sample_rate = DEFAULT_SAMPLE_RATE) :
            ChannelData<WaveSample>(frames, channels),
            _sample_rate(sample_rate) {}

        /**
         * @brief Construct a new Sound object from an existing buffer
         *
         * @param samples     Sample buffer
         * @param channels    Number of channels
         * @param sample_rate Sample rate
         */
        Sound(std::vector<WaveSample> samples, u32 channels, f32 sample_rate) :
            ChannelData<WaveSample>(samples, channels),
            _sample_rate(sample_rate) {}

        /**
         * @brief Get the sample rate of the signal
         *
         * @return f32
         */
        inline f32 sample_rate() const { return _sample_rate; }
    };
} // namespace Dynamo::Sound