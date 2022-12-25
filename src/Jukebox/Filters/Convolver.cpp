#include "./Convolver.hpp"

namespace Dynamo {
    Convolver::Convolver() {}

    void Convolver::initialize(unsigned frames) {
        _fft_samples.resize(frames);
        std::fill(_fft_samples.begin(), _fft_samples.end(), 0);

        _fft_coeffs.resize(frames);
        std::fill(_fft_coeffs.begin(), _fft_coeffs.end(), 0);

        // Do not zero-fill, we want to store a queue of the input samples
        _buffer.resize(frames);
    }

    void Convolver::compute(WaveSample *src,
                            WaveSample *dst,
                            Complex *coeffs,
                            const unsigned N,
                            const unsigned M) {
        // Resize the buffers
        unsigned overlap = M - 1;
        unsigned K = N + overlap;
        unsigned fft_length = round_pow2(K);
        initialize(fft_length);

        // Shift back first M - 1 input samples
        for (unsigned f = 0; f < overlap; f++) {
            _buffer[f] = _buffer[f + N];
        }

        // Write input samples to the tail of the buffer
        for (unsigned f = 0; f < N; f++) {
            _buffer[f + overlap].re = norm_sample(src[f]);
        }

        // Write impulse response and input samples to the FFT buffers
        std::copy(coeffs, coeffs + M, _fft_coeffs.data());
        std::copy(_buffer.begin(), _buffer.end(), _fft_samples.begin());

        // Forward transform
        Fourier::transform(_fft_samples.data(), fft_length);
        Fourier::transform(_fft_coeffs.data(), fft_length);

        // Multiplication
        for (unsigned f = 0; f < fft_length; f++) {
            _fft_samples[f] *= _fft_coeffs[f];
        }

        // Inverse transform
        Fourier::inverse(_fft_samples.data(), fft_length);

        // Write the last N samples the destination buffer
        for (unsigned f = 0; f < N; f++) {
            dst[f] = denorm_sample(_fft_samples[f + overlap].re);
        }
    }
} // namespace Dynamo