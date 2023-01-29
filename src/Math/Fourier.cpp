#include "Fourier.hpp"

namespace Dynamo::Fourier {
    void transform(Complex *signal, u32 N) {
        DYN_ASSERT((N & (N - 1)) == 0);

        // Bit reversal element reordering
        for (u32 i = 1, j = 0; i < N; i++) {
            u32 bit = N >> 1;
            while (bit & j) {
                j ^= bit;
                bit >>= 1;
            }
            j ^= bit;
            if (i < j) {
                std::swap(signal[i], signal[j]);
            }
        }

        // Radix-2 FFT
        for (u32 s = 1; s <= find_lsb(N); s++) {
            u32 m = 1 << s;
            u32 half_m = m >> 1;
            Complex omega_m = TWIDDLE_TABLE_FFT[s];

            for (u32 k = 0; k < N; k += m) {
                Complex omega(1);
                for (i32 j = 0; j < half_m; j++) {
                    u32 u_i = k + j;
                    u32 t_i = u_i + half_m;
                    Complex t = omega * signal[t_i];
                    Complex u = signal[u_i];
                    signal[u_i] = u + t;
                    signal[t_i] = u - t;
                    omega *= omega_m;
                }
            }
        }
    }

    void inverse(Complex *signal, u32 N) {
        DYN_ASSERT((N & (N - 1)) == 0);

        // Bit reversal element reordering
        for (u32 i = 1, j = 0; i < N; i++) {
            u32 bit = N >> 1;
            while (bit & j) {
                j ^= bit;
                bit >>= 1;
            }
            j ^= bit;
            if (i < j) {
                std::swap(signal[i], signal[j]);
            }
        }

        // Radix-2 IFFT
        for (u32 s = 1; s <= find_lsb(N); s++) {
            u32 m = 1 << s;
            u32 half_m = m >> 1;
            Complex omega_m = TWIDDLE_TABLE_IFFT[s];

            for (u32 k = 0; k < N; k += m) {
                Complex omega(1);
                for (i32 j = 0; j < half_m; j++) {
                    u32 u_i = k + j;
                    u32 t_i = u_i + half_m;
                    Complex t = omega * signal[t_i];
                    Complex u = signal[u_i];
                    signal[u_i] = u + t;
                    signal[t_i] = u - t;
                    omega *= omega_m;
                }
            }
        }

        // Normalize
        f32 inv_N = 1.0 / N;
        for (i32 f = 0; f < N; f++) {
            signal[f] *= inv_N;
        }
    }
} // namespace Dynamo::Fourier