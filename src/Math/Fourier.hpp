#pragma once

#include <array>
#include <vector>

#include "../Log/Log.hpp"
#include "../Utils/Bits.hpp"
#include "../Utils/ChannelData.hpp"

#include "./Complex.hpp"

namespace Dynamo::Fourier {
    /**
     * @brief Pre-compute twiddle factors for the forward fourier transform
     *
     */
    static const std::array<Complex, 32> TWIDDLE_TABLE_FFT = {
        Complex(0, -2 * M_PI / (1 << 0)).exp(),
        Complex(0, -2 * M_PI / (1 << 1)).exp(),
        Complex(0, -2 * M_PI / (1 << 2)).exp(),
        Complex(0, -2 * M_PI / (1 << 3)).exp(),
        Complex(0, -2 * M_PI / (1 << 4)).exp(),
        Complex(0, -2 * M_PI / (1 << 5)).exp(),
        Complex(0, -2 * M_PI / (1 << 6)).exp(),
        Complex(0, -2 * M_PI / (1 << 7)).exp(),
        Complex(0, -2 * M_PI / (1 << 8)).exp(),
        Complex(0, -2 * M_PI / (1 << 9)).exp(),
        Complex(0, -2 * M_PI / (1 << 10)).exp(),
        Complex(0, -2 * M_PI / (1 << 11)).exp(),
        Complex(0, -2 * M_PI / (1 << 12)).exp(),
        Complex(0, -2 * M_PI / (1 << 13)).exp(),
        Complex(0, -2 * M_PI / (1 << 14)).exp(),
        Complex(0, -2 * M_PI / (1 << 15)).exp(),
        Complex(0, -2 * M_PI / (1 << 17)).exp(),
        Complex(0, -2 * M_PI / (1 << 18)).exp(),
        Complex(0, -2 * M_PI / (1 << 19)).exp(),
        Complex(0, -2 * M_PI / (1 << 20)).exp(),
        Complex(0, -2 * M_PI / (1 << 21)).exp(),
        Complex(0, -2 * M_PI / (1 << 22)).exp(),
        Complex(0, -2 * M_PI / (1 << 23)).exp(),
        Complex(0, -2 * M_PI / (1 << 24)).exp(),
        Complex(0, -2 * M_PI / (1 << 25)).exp(),
        Complex(0, -2 * M_PI / (1 << 26)).exp(),
        Complex(0, -2 * M_PI / (1 << 27)).exp(),
        Complex(0, -2 * M_PI / (1 << 28)).exp(),
        Complex(0, -2 * M_PI / (1 << 29)).exp(),
        Complex(0, -2 * M_PI / (1 << 30)).exp(),
        Complex(0, -2 * M_PI / (1 << 31)).exp(),
    };

    /**
     * @brief Pre-compute twiddle factors for the inverse fourier transform
     *
     */
    static const std::array<Complex, 32> TWIDDLE_TABLE_IFFT = {
        Complex(0, 2 * M_PI / (1 << 0)).exp(),
        Complex(0, 2 * M_PI / (1 << 1)).exp(),
        Complex(0, 2 * M_PI / (1 << 2)).exp(),
        Complex(0, 2 * M_PI / (1 << 3)).exp(),
        Complex(0, 2 * M_PI / (1 << 4)).exp(),
        Complex(0, 2 * M_PI / (1 << 5)).exp(),
        Complex(0, 2 * M_PI / (1 << 6)).exp(),
        Complex(0, 2 * M_PI / (1 << 7)).exp(),
        Complex(0, 2 * M_PI / (1 << 8)).exp(),
        Complex(0, 2 * M_PI / (1 << 9)).exp(),
        Complex(0, 2 * M_PI / (1 << 10)).exp(),
        Complex(0, 2 * M_PI / (1 << 11)).exp(),
        Complex(0, 2 * M_PI / (1 << 12)).exp(),
        Complex(0, 2 * M_PI / (1 << 13)).exp(),
        Complex(0, 2 * M_PI / (1 << 14)).exp(),
        Complex(0, 2 * M_PI / (1 << 15)).exp(),
        Complex(0, 2 * M_PI / (1 << 17)).exp(),
        Complex(0, 2 * M_PI / (1 << 18)).exp(),
        Complex(0, 2 * M_PI / (1 << 19)).exp(),
        Complex(0, 2 * M_PI / (1 << 20)).exp(),
        Complex(0, 2 * M_PI / (1 << 21)).exp(),
        Complex(0, 2 * M_PI / (1 << 22)).exp(),
        Complex(0, 2 * M_PI / (1 << 23)).exp(),
        Complex(0, 2 * M_PI / (1 << 24)).exp(),
        Complex(0, 2 * M_PI / (1 << 25)).exp(),
        Complex(0, 2 * M_PI / (1 << 26)).exp(),
        Complex(0, 2 * M_PI / (1 << 27)).exp(),
        Complex(0, 2 * M_PI / (1 << 28)).exp(),
        Complex(0, 2 * M_PI / (1 << 29)).exp(),
        Complex(0, 2 * M_PI / (1 << 30)).exp(),
        Complex(0, 2 * M_PI / (1 << 31)).exp(),
    };

    /**
     * @brief Implementation of the fourier transform algorithm to extract
     * the frequency domain of a signal
     *
     * @param signal     Input signal
     * @param freqs      Output frequency domain
     * @param N          Total number of frames (must be a power of 2)
     * @param src_offset Start frame index of the input
     * @param dst_offset Start frame index of the output
     */
    void transform(ChannelData<Complex> &signal,
                   ChannelData<Complex> &freqs,
                   unsigned N,
                   unsigned src_offset = 0,
                   unsigned dst_offset = 0);

    /**
     * @brief Implementation of the inverse fourier transform algorithm to
     * convert a frequency domain into a time domain signal
     *
     * @param freqs      Input frequency domain
     * @param signal     Output signal
     * @param N          Total number of frames (must be a power of 2)
     * @param src_offset Start frame index of the input
     * @param dst_offset Start frame index of the output
     */
    void inverse(ChannelData<Complex> &freqs,
                 ChannelData<Complex> &signal,
                 unsigned N,
                 unsigned src_offset = 0,
                 unsigned dst_offset = 0);
} // namespace Dynamo::Fourier