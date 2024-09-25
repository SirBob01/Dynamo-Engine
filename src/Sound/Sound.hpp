#pragma once

#include <array>
#include <functional>
#include <vector>

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
    static constexpr float DEFAULT_SAMPLE_RATE = 44100;

    // TODO: Implement upmix and downmix
    // https://developer.mozilla.org/en-US/docs/Web/API/Web_Audio_API/Basic_concepts_behind_Web_Audio_API#up-mixing_and_down-mixing

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
              float sample_rate = DEFAULT_SAMPLE_RATE);

        /**
         * @brief Construct a new Sound object from an existing buffer
         *
         * @param samples     Sample buffer
         * @param channels    Number of channels
         * @param sample_rate Sample rate
         */
        Sound(std::vector<WaveSample> samples,
              unsigned channels,
              float sample_rate);

        /**
         * @brief Get the sample rate of the signal
         *
         * @return float
         */
        float sample_rate() const;

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
     * @brief Sound reference.
     *
     */
    using SoundRef = std::reference_wrapper<Sound>;
} // namespace Dynamo::Sound