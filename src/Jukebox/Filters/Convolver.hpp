#pragma once

#include "../../Math/Complex.hpp"
#include "../../Math/Fourier.hpp"
#include "../../Utils/Bits.hpp"
#include "../../Utils/ChannelData.hpp"

#include "../Sound.hpp"

namespace Dynamo {
    /**
     * @brief Signal convolution engine
     *
     * This implements the overlap-save block algorithm to compute convolutions
     * in real-time
     *
     * TODO: Implement partitioned convolution for long impulse responses
     *
     */
    class Convolver {
        /**
         * @brief FFT buffers
         *
         */
        std::vector<Complex> _fft_samples;
        std::vector<Complex> _fft_coeffs;

        /**
         * @brief Input buffer
         *
         */
        std::vector<Complex> _buffer;

        /**
         * @brief Resize all buffers to fit a desired number of frames
         *
         * @param frames
         */
        void initialize(unsigned frames);

      public:
        /**
         * @brief Construct a new Convolver object
         *
         */
        Convolver();

        /**
         * @brief Apply the impulse repsonse to a sound
         *
         * @param src    Source sound buffer
         * @param dst    Destination sound buffer
         * @param coeffs Impulse response coefficient buffer
         * @param N      Length of the sound to be processed
         * @param M      Length of the impulse response buffer
         * @return Sound&
         */
        void compute(WaveSample *src,
                     WaveSample *dst,
                     Complex *coeffs,
                     const unsigned N,
                     const unsigned M);
    };
} // namespace Dynamo