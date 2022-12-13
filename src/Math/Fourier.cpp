#include "Fourier.hpp"

namespace Dynamo::Fourier {
    void transform(ChannelData<Complex> &signal, unsigned N, unsigned offset) {
        DYN_ASSERT(offset + N <= signal.frames() && (N & (N - 1)) == 0);

        // Bit reversal element reordering
        for (int c = 0; c < signal.channels(); c++) {
            for (unsigned i = 1, j = 0; i < N; i++) {
                unsigned bit = N >> 1;
                while (bit & j) {
                    j ^= bit;
                    bit >>= 1;
                }
                j ^= bit;
                if (i < j) {
                    std::swap(signal.at(i + offset, c),
                              signal.at(j + offset, c));
                }
            }
        }

        // Radix-2 FFT
        for (unsigned s = 1; s <= find_lsb(N); s++) {
            unsigned m = 1 << s;
            unsigned half_m = m >> 1;
            Complex omega_m = TWIDDLE_TABLE_FFT[s];

            for (unsigned c = 0; c < signal.channels(); c++) {
                for (unsigned k = 0; k < N; k += m) {
                    Complex omega(1, 0);
                    for (int j = 0; j < half_m; j++) {
                        unsigned u_i = offset + k + j;
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
    }

    void inverse(ChannelData<Complex> &signal, unsigned N, unsigned offset) {
        DYN_ASSERT(offset + N <= signal.frames() && (N & (N - 1)) == 0);

        // Bit reversal element reordering
        for (int c = 0; c < signal.channels(); c++) {
            for (unsigned i = 1, j = 0; i < N; i++) {
                unsigned bit = N >> 1;
                while (bit & j) {
                    j ^= bit;
                    bit >>= 1;
                }
                j ^= bit;
                if (i < j) {
                    std::swap(signal.at(i + offset, c),
                              signal.at(j + offset, c));
                }
            }
        }

        // Radix-2 IFFT
        for (unsigned s = 1; s <= find_lsb(N); s++) {
            unsigned m = 1 << s;
            unsigned half_m = m >> 1;
            Complex omega_m = TWIDDLE_TABLE_IFFT[s];

            for (unsigned c = 0; c < signal.channels(); c++) {
                for (unsigned k = 0; k < N; k += m) {
                    Complex omega(1, 0);
                    for (int j = 0; j < half_m; j++) {
                        unsigned u_i = offset + k + j;
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
                signal.at(f + offset, c) *= inv_N;
            }
        }
    }
} // namespace Dynamo::Fourier