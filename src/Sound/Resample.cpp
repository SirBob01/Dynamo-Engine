#include "Resample.hpp"

namespace Dynamo::Sound {
    void resample_signal(WaveSample *src,
                         WaveSample *dst,
                         f64 time_offset,
                         f64 src_length,
                         f64 src_rate,
                         f64 dst_rate) {
        f64 factor = dst_rate / src_rate;
        f64 scale = std::min(1.0, factor);
        f64 time_increment = 1.0 / factor;

        u32 dst_length = src_length * factor;
        u32 filter_step = scale * FILTER_PRECISION;

        f64 time = time_offset;
        f64 time_end = src_length + time_offset;

        for (u32 dst_f = 0; dst_f < dst_length; dst_f++) {
            // Target sample
            f64 dst_sample = 0;

            // Get the source frame
            u32 src_f = time;
            f64 P = scale * (time - src_f);

            // Calculate filter offset and interpolation factor
            f64 P_frac = P * FILTER_PRECISION;
            u32 l = P_frac;
            f64 interp = P_frac - l;

            // Left wing
            for (u32 i = 0; i < FILTER_HALF_LENGTH; i++) {
                i32 s_i = src_f - i;
                i32 h_i = l + i * filter_step;
                if (s_i < 0 || h_i >= static_cast<i32>(FILTER_HALF_LENGTH))
                    break;
                f64 weight = FILTER_RWING[h_i] + interp * FILTER_DIFFS[h_i];
                dst_sample += src[s_i] * weight;
            }

            // Invert
            P = scale - P;

            // Calculate filter offset and interpolation factor
            P_frac = P * FILTER_PRECISION;
            l = P_frac;
            interp = P_frac - l;

            // Right wing
            for (u32 i = 0; i < FILTER_HALF_LENGTH; i++) {
                i32 s_i = src_f + i + 1;
                i32 h_i = l + i * filter_step;
                if (s_i >= time_end ||
                    h_i >= static_cast<i32>(FILTER_HALF_LENGTH))
                    break;
                f64 weight = FILTER_RWING[h_i] + interp * FILTER_DIFFS[h_i];
                dst_sample += src[s_i] * weight;
            }

            // Set the new sample
            dst[dst_f] = dst_sample;

            // Increment time
            time += time_increment;
        }
    }
} // namespace Dynamo::Sound
