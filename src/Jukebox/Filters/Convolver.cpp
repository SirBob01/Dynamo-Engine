#include "./Convolver.hpp"

namespace Dynamo {
    Convolver::Convolver() {}

    void Convolver::initialize(unsigned frames) {
        _fft_samples.resize(frames);
        std::fill(_fft_samples.begin(), _fft_samples.end(), 0);

        _fft_ir.resize(frames);
        std::fill(_fft_ir.begin(), _fft_ir.end(), 0);

        // Do not zero-fill, we want to store a queue of the input samples
        _buffer.resize(frames);
    }

    void Convolver::compute(WaveSample *src,
                            WaveSample *dst,
                            WaveSample *ir,
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
            _buffer[f + overlap].re = src[f];
        }

        // Write input samples and impulse response to the FFT buffers
        std::copy(_buffer.begin(), _buffer.end(), _fft_samples.begin());
        for (unsigned f = 0; f < overlap; f++) {
            _fft_ir[f].re = ir[f];
        }

        // Forward transform
        Fourier::transform(_fft_samples.data(), fft_length);
        Fourier::transform(_fft_ir.data(), fft_length);

        // Multiplication
        for (unsigned f = 0; f < fft_length; f++) {
            _fft_samples[f] *= _fft_ir[f];
        }

        // Inverse transform
        Fourier::inverse(_fft_samples.data(), fft_length);

        // Write the last N samples the destination buffer
        for (unsigned f = 0; f < N; f++) {
            dst[f] = _fft_samples[f + overlap].re;
        }
    }
} // namespace Dynamo