#include "Fourier.hpp"
#include "../Utils/Bits.hpp"
#include "../Utils/Log.hpp"

namespace Dynamo::Fourier {
    void transform(Complex *signal, unsigned N) {
        DYN_ASSERT((N & (N - 1)) == 0);

        // Bit reversal element reordering
        for (unsigned i = 1, j = 0; i < N; i++) {
            unsigned bit = N >> 1;
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
        for (unsigned s = 1; s <= find_lsb(N); s++) {
            unsigned m = 1 << s;
            unsigned half_m = m >> 1;
            Complex omega_m = TWIDDLE_TABLE_FFT[s];

            for (unsigned k = 0; k < N; k += m) {
                Complex omega(1);
                for (unsigned j = 0; j < half_m; j++) {
                    unsigned u_i = k + j;
                    unsigned t_i = u_i + half_m;
                    Complex t = omega * signal[t_i];
                    Complex u = signal[u_i];
                    signal[u_i] = u + t;
                    signal[t_i] = u - t;
                    omega *= omega_m;
                }
            }
        }
    }

    void inverse(Complex *signal, unsigned N) {
        DYN_ASSERT((N & (N - 1)) == 0);

        // Bit reversal element reordering
        for (unsigned i = 1, j = 0; i < N; i++) {
            unsigned bit = N >> 1;
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
        for (unsigned s = 1; s <= find_lsb(N); s++) {
            unsigned m = 1 << s;
            unsigned half_m = m >> 1;
            Complex omega_m = TWIDDLE_TABLE_IFFT[s];

            for (unsigned k = 0; k < N; k += m) {
                Complex omega(1);
                for (unsigned j = 0; j < half_m; j++) {
                    unsigned u_i = k + j;
                    unsigned t_i = u_i + half_m;
                    Complex t = omega * signal[t_i];
                    Complex u = signal[u_i];
                    signal[u_i] = u + t;
                    signal[t_i] = u - t;
                    omega *= omega_m;
                }
            }
        }

        // Normalize
        float inv_N = 1.0 / N;
        for (unsigned f = 0; f < N; f++) {
            signal[f] *= inv_N;
        }
    }
} // namespace Dynamo::Fourier