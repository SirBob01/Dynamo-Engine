#pragma once

#include <array>
#include <vector>

#include "../Log/Log.hpp"
#include "../Utils/Bits.hpp"

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
     * the frequency-domain of a time-domain signal in-place
     *
     * @param signal Signal buffer
     * @param N      Total number of frames (must be a power of 2)
     */
    void transform(Complex *signal, unsigned N);

    /**
     * @brief Implementation of the inverse fourier transform algorithm to
     * extract the time-domain of a frequency-domain signal in-place
     *
     * @param signal Signal buffer
     * @param N      Total number of frames (must be a power of 2)
     */
    void inverse(Complex *signal, unsigned N);
} // namespace Dynamo::Fourier