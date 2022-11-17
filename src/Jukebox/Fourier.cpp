#include "Fourier.hpp"

namespace Dynamo {
    void fourier_transform(ComplexSignal &signal,
                           ComplexSignal &freqs,
                           unsigned N,
                           unsigned src_offset,
                           unsigned dst_offset) {
        DYN_ASSERT(dst_offset + N <= freqs.frames() &&
                   freqs.channels() == signal.channels() && (N & (N - 1)) == 0);

        // Bit reversal element reordering
        unsigned log_N = find_lsb(N);
        unsigned bit_offset = sizeof(unsigned) * 8 - log_N;
        unsigned max_frame = signal.frames();
        for (int c = 0; c < freqs.channels(); c++) {
            for (unsigned f = 0; f < N; f++) {
                unsigned f_r = reverse_bits(f) >> bit_offset;

                Complex sample;
                if (f + src_offset < max_frame) {
                    sample = signal.at(f + src_offset, c);
                }
                freqs.at(f_r + dst_offset, c) = sample;
            }
        }

        // Radix-2 FFT
        for (unsigned s = 1; s <= log_N; s++) {
            unsigned m = 1 << s;
            unsigned half_m = m >> 1;
            Complex omega_m = TWIDDLE_TABLE_FFT[s];

            for (unsigned c = 0; c < freqs.channels(); c++) {
                for (unsigned k = 0; k < N; k += m) {
                    Complex omega(1, 0);
                    for (int j = 0; j < half_m; j++) {
                        unsigned u_i = dst_offset + k + j;
                        unsigned t_i = u_i + half_m;
                        Complex t = omega * freqs.at(t_i, c);
                        Complex u = freqs.at(u_i, c);
                        freqs.at(u_i, c) = u + t;
                        freqs.at(t_i, c) = u - t;
                        omega *= omega_m;
                    }
                }
            }
        }
    }

    void inverse_fourier_transform(ComplexSignal &freqs,
                                   ComplexSignal &signal,
                                   unsigned N,
                                   unsigned src_offset,
                                   unsigned dst_offset) {
        DYN_ASSERT(dst_offset + N <= signal.frames() &&
                   signal.channels() == freqs.channels() && (N & (N - 1)) == 0);

        // Bit reversal element reordering
        unsigned log_N = find_lsb(N);
        unsigned bit_offset = sizeof(unsigned) * 8 - log_N;
        unsigned max_frame = freqs.frames();
        for (int c = 0; c < signal.channels(); c++) {
            for (unsigned f = 0; f < N; f++) {
                unsigned f_r = reverse_bits(f) >> bit_offset;

                Complex freq(0, 0);
                if (f + src_offset < max_frame) {
                    freq = freqs.at(f + src_offset, c);
                }
                signal.at(f_r + dst_offset, c) = freq;
            }
        }

        // Radix-2 IFFT
        for (unsigned s = 1; s <= log_N; s++) {
            unsigned m = 1 << s;
            unsigned half_m = m >> 1;
            Complex omega_m = TWIDDLE_TABLE_IFFT[s];

            for (unsigned c = 0; c < signal.channels(); c++) {
                for (unsigned k = 0; k < N; k += m) {
                    Complex omega(1, 0);
                    for (int j = 0; j < half_m; j++) {
                        unsigned u_i = dst_offset + k + j;
                        unsigned t_i = u_i + half_m;
                        Complex t = omega * signal.at(t_i, c);
                        Complex u = signal.at(u_i, c);
                        signal.at(u_i, c) = u + t;
                        signal.at(t_i, c) = u - t;
                        omega *= omega_m;
                    }
                }
            }
        }

        double inv_N = 1.0 / N;
        for (int c = 0; c < signal.channels(); c++) {
            for (int f = 0; f < N; f++) {
                signal.at(f + dst_offset, c) *= inv_N;
            }
        }
    }
} // namespace Dynamo