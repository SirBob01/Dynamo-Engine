#include "./Convolver.hpp"

namespace Dynamo::Sound {
    void Convolver::initialize(WaveSample *ir, u32 M) {
        // Initialize the partition buffer
        _partition_count = std::ceil(static_cast<f32>(M) / BLOCK_LENGTH);
        _partitions.resize(_partition_count * PARTITION_LENGTH);
        std::fill(_partitions.begin(), _partitions.end(), 0);

        // Copy impulse response and pre-compute the FFT of each partition
        for (u32 i = 0; i < _partition_count; i++) {
            u32 ir_offset = i * BLOCK_LENGTH;
            u32 partition_offset = i * PARTITION_LENGTH;

            u32 copy_size = std::min(BLOCK_LENGTH, M);
            std::copy(ir + ir_offset,
                      ir + ir_offset + copy_size,
                      _partitions.data() + partition_offset);
            Fourier::transform(_partitions.data() + partition_offset,
                               PARTITION_LENGTH);

            M -= copy_size;
        }

        // Resize the frequency delay-line, do not zero out
        _fdl.resize(_partition_count * PARTITION_LENGTH);
    }

    void Convolver::compute(WaveSample *src, WaveSample *dst, u32 N) {
        // Shift back the second half of the input buffer
        for (u32 i = 0; i < BLOCK_LENGTH; i++) {
            _input[i] = _input[i + BLOCK_LENGTH];
        }

        // Read the latest samples, zeroing out the remainder of buffer
        for (u32 i = 0; i < N; i++) {
            _input[i + BLOCK_LENGTH] = src[i];
        }
        for (u32 i = N; i < BLOCK_LENGTH; i++) {
            _input[i + BLOCK_LENGTH] = 0;
        }

        // Shift up the frequency delay-line by one partition
        for (u32 i = _fdl.size() - 1; i >= PARTITION_LENGTH; i--) {
            _fdl[i] = _fdl[i - PARTITION_LENGTH];
        }

        // Forward transform the input block onto the frequency delay-line
        std::copy(_input.begin(), _input.end(), _fdl.begin());
        Fourier::transform(_fdl.data(), PARTITION_LENGTH);

        // Convolve with each partition
        std::fill(_output.begin(), _output.end(), 0);
        for (u32 i = 0; i < _partition_count; i++) {
            u32 offset = PARTITION_LENGTH * i;

            // Pointwise multiply and accumulate onto the output buffer
            for (u32 j = 0; j < PARTITION_LENGTH; j++) {
                u32 j_offset = offset + j;
                _output[j] += _fdl[j_offset] * _partitions[j_offset];
            }
        }

        // Inverse transform the output buffer
        Fourier::inverse(_output.begin(), _output.size());

        // Write the second half of the output buffer
        for (u32 i = 0; i < N; i++) {
            dst[i] = _output[i + BLOCK_LENGTH].re;
        }
    }
} // namespace Dynamo::Sound