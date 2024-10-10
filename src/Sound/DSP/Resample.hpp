#pragma once
#define _USE_MATH_DEFINES

#include <array>

#include <Sound/Buffer.hpp>

namespace Dynamo::Sound {
    /**
     * @brief Number of zero-crossings in the filter
     *
     */
    static constexpr unsigned FILTER_ZERO_CROSSINGS = 32;

    /**
     * @brief Number of coefficients per zero crossing
     *
     */
    static constexpr unsigned FILTER_PRECISION = 4;

    /**
     * @brief Length of one wing in the filter
     *
     */
    static constexpr unsigned FILTER_HALF_LENGTH = FILTER_ZERO_CROSSINGS * FILTER_PRECISION + 1;

    /**
     * @brief Normalized sinc function
     *
     * @param x
     * @return constexpr double
     */
    constexpr double sinc(double x) {
        if (x == 0) return 1;
        double f = M_PI * x;
        return std::sin(f) / f;
    }

    /**
     * @brief Compute the zeroth-order modified Bessel function
     *
     * @param x
     * @return constexpr double
     */
    constexpr double i0(double x) {
        constexpr double epsilon = 0.00000001;
        double sum = 1;
        double term = 1;
        double m = 0;
        while (term > epsilon * sum) {
            sum += term;
            m++;
            term *= (x * x) / (4 * m * m);
        }
        return sum;
    }

    /**
     * @brief Compute the Kaiser window
     *
     * @param x
     * @return constexpr double
     */
    constexpr double filter_window(double n, unsigned N = 35, double beta = 6) {
        double m = 2.0 * n / N;
        double num = i0(beta * std::sqrt(1 - m * m));
        double den = i0(beta);
        return num / den;
    }

    /**
     * @brief Compulte the right-wing of the filter coefficients
     *
     * @return constexpr std::array<double, FILTER_HALF_LENGTH>
     */
    constexpr std::array<double, FILTER_HALF_LENGTH> construct_filter_table() {
        std::array<double, FILTER_HALF_LENGTH> coeffs = {0};
        for (double l = 0; l < FILTER_HALF_LENGTH; l++) {
            double f = l / FILTER_PRECISION;
            coeffs[l] = sinc(f) * filter_window(l);
        }
        return coeffs;
    }

    /**
     * @brief Compute the filter coefficient difference table
     *
     * @param coeffs
     * @return constexpr std::array<double, FILTER_HALF_LENGTH>
     */
    constexpr std::array<double, FILTER_HALF_LENGTH>
    construct_difference_table(const std::array<double, FILTER_HALF_LENGTH> &coeffs) {
        std::array<double, FILTER_HALF_LENGTH> diffs = {0};
        for (unsigned i = 0; i < FILTER_HALF_LENGTH - 1; i++) {
            diffs[i] = coeffs[i + 1] - coeffs[i];
        }
        return diffs;
    }

    /**
     * @brief Low pass filter coefficient table, precomputed on
     * initialization
     *
     */
    static const std::array<double, FILTER_HALF_LENGTH> FILTER_RWING = construct_filter_table();

    /**
     * @brief Difference table for filter coefficients, precomputed on
     * initialization
     *
     */
    static const std::array<double, FILTER_HALF_LENGTH> FILTER_DIFFS = construct_difference_table(FILTER_RWING);

    /**
     * @brief Change the sample rate of a signal
     *
     * This routine implements the algorithm described in
     * https://ccrma.stanford.edu/~jos/resample/
     *
     * @param src         Signal source buffer
     * @param dst         Signal destination buffer
     * @param time_offset Frame offset in the signal source
     * @param src_length  Length of the signal source
     * @param src_rate    Source sample rate
     * @param dst_rate    Destination sample rate
     */
    void resample_signal(WaveSample *src,
                         WaveSample *dst,
                         double time_offset,
                         double src_length,
                         double src_rate,
                         double dst_rate);
} // namespace Dynamo::Sound