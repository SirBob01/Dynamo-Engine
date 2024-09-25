#include <Math/Fourier.hpp>
#include <Sound/DSP/Convolver.hpp>

namespace Dynamo::Sound {
    void Convolver::initialize(WaveSample *ir, unsigned M) {
        // Initialize the partition buffer
        _partition_count = std::ceil(static_cast<float>(M) / BLOCK_LENGTH);
        _partitions.resize(_partition_count * BLOCK_LENGTH_2);
        std::fill(_partitions.begin(), _partitions.end(), 0);

        // Copy impulse response and pre-compute the FFT of each partition
        for (unsigned i = 0; i < _partition_count; i++) {
            unsigned ir_offset = i * BLOCK_LENGTH;
            unsigned partition_offset = i * BLOCK_LENGTH_2;

            unsigned copy_size = std::min(BLOCK_LENGTH, M);
            std::copy(ir + ir_offset,
                      ir + ir_offset + copy_size,
                      _partitions.data() + partition_offset);
            Fourier::transform(_partitions.data() + partition_offset,
                               BLOCK_LENGTH_2);

            M -= copy_size;
        }

        // Resize the frequency delay-line, do not zero out
        _fdl.resize(_partition_count * BLOCK_LENGTH_2);
    }

    void Convolver::compute(WaveSample *src, WaveSample *dst, unsigned N) {
        // Shift back the second half of the input buffer
        std::copy(_input.begin() + BLOCK_LENGTH,
                  _input.begin() + BLOCK_LENGTH_2,
                  _input.begin());

        // Read the latest samples, zeroing out the remainder of buffer
        std::copy(src, src + N, _input.begin() + BLOCK_LENGTH);
        std::fill(_input.begin() + BLOCK_LENGTH + N,
                  _input.begin() + BLOCK_LENGTH_2,
                  0);

        // Shift up the frequency delay-line by one partition
        std::copy(_fdl.begin(),
                  _fdl.end() - BLOCK_LENGTH_2,
                  _fdl.begin() + BLOCK_LENGTH_2);

        // Forward transform the input block onto the frequency delay-line
        std::copy(_input.begin(), _input.end(), _fdl.begin());
        Fourier::transform(_fdl.data(), BLOCK_LENGTH_2);

        // Convolve with each partition
        std::fill(_output.begin(), _output.end(), 0);
        for (unsigned i = 0; i < _partition_count; i++) {
            unsigned offset = BLOCK_LENGTH_2 * i;

            // Pointwise multiply and accumulate onto the output buffer
            for (unsigned j = 0; j < BLOCK_LENGTH_2; j++) {
                unsigned j_offset = offset + j;
                _output[j] += _fdl[j_offset] * _partitions[j_offset];
            }
        }

        // Inverse transform the output buffer
        Fourier::inverse(_output.begin(), _output.size());

        // Write the second half of the output buffer
        for (unsigned i = 0; i < N; i++) {
            dst[i] = _output[i + BLOCK_LENGTH].re;
        }
    }
} // namespace Dynamo::Sound