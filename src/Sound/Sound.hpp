#pragma once

#include <functional>
#include <vector>

#include <Utils/ChannelData.hpp>

namespace Dynamo::Sound {
    /**
     * @brief The default sample rate is defined to be 44.1KHz.
     *
     */
    static constexpr float DEFAULT_SAMPLE_RATE = 44100;

    /**
     * @brief Indiviual sample ranges between [−1.0, +1.0].
     *
     */
    using WaveSample = float;

    /**
     * @brief Supported channel remix combinations.
     *
     */
    enum class ChannelRemixMode : unsigned {
        C_1_2 = ((1 << (1 + 7)) | (1 << (2 - 1))),
        C_1_4 = ((1 << (1 + 7)) | (1 << (4 - 1))),
        C_1_6 = ((1 << (1 + 7)) | (1 << (6 - 1))),
        C_2_1 = ((1 << (2 + 7)) | (1 << (1 - 1))),
        C_2_4 = ((1 << (2 + 7)) | (1 << (4 - 1))),
        C_2_6 = ((1 << (2 + 7)) | (1 << (6 - 1))),
        C_4_1 = ((1 << (4 + 7)) | (1 << (1 - 1))),
        C_4_2 = ((1 << (4 + 7)) | (1 << (2 - 1))),
        C_4_6 = ((1 << (4 + 7)) | (1 << (6 - 1))),
        C_6_1 = ((1 << (6 + 7)) | (1 << (1 - 1))),
        C_6_2 = ((1 << (6 + 7)) | (1 << (2 - 1))),
        C_6_4 = ((1 << (6 + 7)) | (1 << (4 - 1))),
    };

    /**
     * @brief A signal holding multiple channels of WaveSample data.
     *
     */
    class Sound : public ChannelData<WaveSample> {
        float _sample_rate;

        void vadd_channel(Sound &dst,
                          unsigned src_channel,
                          unsigned dst_channel,
                          unsigned src_offset,
                          unsigned dst_offset,
                          unsigned length) const;

        void vsma_channel(Sound &dst,
                          float scalar,
                          unsigned src_channel,
                          unsigned dst_channel,
                          unsigned src_offset,
                          unsigned dst_offset,
                          unsigned length) const;

      public:
        /**
         * @brief Construct a new Sound object.
         *
         * @param frames      Number of frames
         * @param channels    Number of channels
         * @param sample_rate Sample rate
         */
        Sound(unsigned frames = 0,
              unsigned channels = 0,
              float sample_rate = DEFAULT_SAMPLE_RATE);

        /**
         * @brief Construct a new Sound object from an existing buffer.
         *
         * @param samples     Sample buffer
         * @param channels    Number of channels
         * @param sample_rate Sample rate
         */
        Sound(const std::vector<WaveSample> &samples,
              unsigned channels,
              float sample_rate);

        /**
         * @brief Get the sample rate of the signal.
         *
         * @return float
         */
        float sample_rate() const;

        /**
         * @brief Remix to the desired channel count in the destination buffer.
         *
         * @param dst
         * @param src_offset
         * @param dst_offset
         * @param length
         */
        void remix(Sound &dst,
                   unsigned src_offset,
                   unsigned dst_offset,
                   unsigned length) const;
    };

    /**
     * @brief Sound reference.
     *
     */
    using SoundRef = std::reference_wrapper<Sound>;
} // namespace Dynamo::Sound