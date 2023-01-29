#pragma once

#include <array>
#include <vector>

#include "../Log/Log.hpp"
#include "../Types.hpp"
#include "../Utils/Bits.hpp"

#include "./Complex.hpp"

namespace Dynamo::Fourier {
    /**
     * @brief Pre-compute the twiddle factor tables
     *
     * @param inverse Forward or inverse transform table
     * @return std::array<Complex, 32>
     */
    static std::array<Complex, 32> construct_twiddle_table(b8 inverse) {
        std::array<Complex, 32> table;
        f32 sign = inverse ? 1 : -1;
        for (u32 i = 0; i < 32; i++) {
            table[i] = Complex(0, sign * 2 * M_PI / (1 << i)).exp();
        }
        return table;
    }

    /**
     * @brief Pre-compute twiddle factors for the forward fourier transform
     *
     */
    static const std::array<Complex, 32> TWIDDLE_TABLE_FFT =
        construct_twiddle_table(false);

    /**
     * @brief Pre-compute twiddle factors for the inverse fourier transform
     *
     */
    static const std::array<Complex, 32> TWIDDLE_TABLE_IFFT =
        construct_twiddle_table(true);

    /**
     * @brief Implementation of the fourier transform algorithm to extract
     * the frequency-domain of a time-domain signal in-place
     *
     * @param signal Signal buffer
     * @param N      Total number of frames (must be a power of 2)
     */
    void transform(Complex *signal, u32 N);

    /**
     * @brief Implementation of the inverse fourier transform algorithm to
     * extract the time-domain of a frequency-domain signal in-place
     *
     * @param signal Signal buffer
     * @param N      Total number of frames (must be a power of 2)
     */
    void inverse(Complex *signal, u32 N);
} // namespace Dynamo::Fourier