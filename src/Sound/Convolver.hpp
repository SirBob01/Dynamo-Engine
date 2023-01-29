#pragma once

#include <array>
#include <vector>

#include "../Math/Complex.hpp"
#include "../Math/Fourier.hpp"
#include "../Types.hpp"
#include "../Utils/Bits.hpp"
#include "../Utils/ChannelData.hpp"

#include "./Chunk.hpp"
#include "./Sound.hpp"

namespace Dynamo::Sound {
    /**
     * @brief Length of a subfilter unit for convolutional processing
     *
     */
    static constexpr u32 BLOCK_LENGTH = round_pow2(MAX_CHUNK_LENGTH);

    /**
     * @brief Length of a partition unit on which FFT is performed for
     * convolutional processing
     *
     */
    static constexpr u32 PARTITION_LENGTH = BLOCK_LENGTH * 2;

    /**
     * @brief Signal convolution engine
     *
     * This implements the overlap-save block algorithm to compute convolutions
     * in real-time
     *
     */
    class Convolver {
        /**
         * @brief Input sample buffer
         *
         */
        std::array<WaveSample, BLOCK_LENGTH * 2> _input = {0};

        /**
         * @brief Output sample buffer
         *
         */
        std::array<Complex, BLOCK_LENGTH * 2> _output = {0};

        /**
         * @brief Frequency delay line
         *
         */
        std::vector<Complex> _fdl;

        /**
         * @brief Partitioned FFT transforms of the impulse response
         *
         */
        std::vector<Complex> _partitions;

        /**
         * @brief Number of partitions
         *
         */
        u32 _partition_count;

      public:
        /**
         * @brief Set the impulse response to convolve
         *
         * @param ir Impulse response buffer
         * @param M  Length of the impulse response
         */
        void initialize(WaveSample *ir, u32 M);

        /**
         * @brief Apply the impulse repsonse to a sound chunk
         *
         * @param src Source sound buffer
         * @param dst Destination sound buffer
         * @param N   Length of the sound, must be <= MAX_CHUNK_LENGTH
         */
        void compute(WaveSample *src, WaveSample *dst, u32 N);
    };
} // namespace Dynamo::Sound