#pragma once

#include <functional>

#include <Utils/Log.hpp>

namespace Dynamo::Sound {
    /**
     * @brief The standard sample rate for processing is defined to be 44.1KHz.
     *
     */
    static constexpr double STANDARD_SAMPLE_RATE = 44100;

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
     * @brief Indiviual sample ranges between [−1.0, +1.0].
     *
     */
    using WaveSample = float;

    /**
     * @brief Deinterleaved multi-channel buffer for WaveSamples.
     *
     */
    class Buffer {
        WaveSample *_samples;

        unsigned _frames;
        unsigned _channels;

      public:
        /**
         * @brief Construct an empty Buffer.
         *
         * @param frames   Number of frames.
         * @param channels Number of channels.
         */
        Buffer(unsigned frames = 0, unsigned channels = 0);

        /**
         * @brief Construct a Buffer with an existing sample array.
         *
         * @param data     Data buffer.
         * @param channels Number of channels.
         */
        Buffer(WaveSample *samples, unsigned frames, unsigned channels);

        /**
         * @brief Copy constructor.
         *
         * @param rhs
         */
        Buffer(const Buffer &rhs);

        /**
         * @brief Destroy a Buffer.
         *
         */
        ~Buffer();

        /**
         * @brief Copy assignment.
         *
         * @param rhs
         * @return Buffer&
         */
        Buffer &operator=(const Buffer &rhs);

        /**
         * @brief Get the pointer to the start of a channel.
         *
         * @param channel Channel index.
         * @return WaveSample*
         */
        WaveSample *operator[](const unsigned channel);

        /**
         * @brief Get the const pointer to the start of a channel.
         *
         * @param channel Channel index.
         * @return WaveSample*
         */
        const WaveSample *operator[](const unsigned channel) const;

        /**
         * @brief Get internal data pointer.
         *
         * @return WaveSample*
         */
        WaveSample *data();

        /**
         * @brief Get internal data pointer.
         *
         * @return WaveSample*
         */
        const WaveSample *data() const;

        /**
         * @brief Get the number of frames.
         *
         * @return unsigned
         */
        unsigned frames() const;

        /**
         * @brief Get the number of channels.
         *
         * @return unsigned
         */
        unsigned channels() const;

        /**
         * @brief Silence the buffer.
         *
         */
        void silence();

        /**
         * @brief Resize the container to fit a number of frames and channels.
         *
         * This will not preserve the existing data.
         *
         * @param frames   Number of frames.
         * @param channels Number of channels.
         */
        void resize(const unsigned frames, const unsigned channels);

        /**
         * @brief Remix to the desired channel count in the destination buffer.
         *
         * @param dst
         */
        void remix(Buffer &dst) const;
    };

    using BufferRef = std::reference_wrapper<Buffer>;
} // namespace Dynamo::Sound