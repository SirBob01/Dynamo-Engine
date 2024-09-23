#include "Resample.hpp"

namespace Dynamo::Sound {
    void resample_signal(WaveSample *src,
                         WaveSample *dst,
                         double time_offset,
                         double src_length,
                         double src_rate,
                         double dst_rate) {
        double factor = dst_rate / src_rate;
        double scale = std::min(1.0, factor);
        double time_increment = 1.0 / factor;

        unsigned dst_length = src_length * factor;
        unsigned filter_step = scale * FILTER_PRECISION;

        double time = time_offset;
        double time_end = src_length + time_offset;

        for (unsigned dst_f = 0; dst_f < dst_length; dst_f++) {
            // Target sample
            double dst_sample = 0;

            // Get the source frame
            unsigned src_f = time;
            double P = scale * (time - src_f);

            // Calculate filter offset and interpolation factor
            double P_frac = P * FILTER_PRECISION;
            unsigned l = P_frac;
            double interp = P_frac - l;

            // Left wing
            for (unsigned i = 0; i < FILTER_HALF_LENGTH; i++) {
                int s_i = src_f - i;
                int h_i = l + i * filter_step;
                if (s_i < 0 || h_i >= static_cast<int>(FILTER_HALF_LENGTH))
                    break;
                double weight = FILTER_RWING[h_i] + interp * FILTER_DIFFS[h_i];
                dst_sample += src[s_i] * weight;
            }

            // Invert
            P = scale - P;

            // Calculate filter offset and interpolation factor
            P_frac = P * FILTER_PRECISION;
            l = P_frac;
            interp = P_frac - l;

            // Right wing
            for (unsigned i = 0; i < FILTER_HALF_LENGTH; i++) {
                int s_i = src_f + i + 1;
                int h_i = l + i * filter_step;
                if (s_i >= time_end ||
                    h_i >= static_cast<int>(FILTER_HALF_LENGTH))
                    break;
                double weight = FILTER_RWING[h_i] + interp * FILTER_DIFFS[h_i];
                dst_sample += src[s_i] * weight;
            }

            // Set the new sample
            dst[dst_f] = dst_sample;

            // Increment time
            time += time_increment;
        }
    }
} // namespace Dynamo::Sound
