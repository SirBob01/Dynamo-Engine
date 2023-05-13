#include "Sound.hpp"

namespace Dynamo::Sound {
    WaveFrame Sound::get_frame(const u32 frame, const u32 out_channels) {
        WaveFrame output;
        output.channels = out_channels;

        std::array<f32, 2> samples = {0, 0};
        for (u32 src_c = 0; src_c < channels(); src_c++) {
            WaveSample sample = at(frame, src_c);

            // Perform weighted sum using downmixing coefficient matrix
            i32 index = static_cast<i32>(CHANNEL_ORDERS[channels() - 1][src_c]);
            for (u32 dst_c = 0; dst_c < output.channels; dst_c++) {
                f32 coeff = DOWNMIX_COEFFS[out_channels - 1][index][dst_c];
                samples[dst_c] += sample * coeff;
            }
        }

        // Write denormalzed samples onto the output frame
        for (u32 i = 0; i < samples.size(); i++) {
            output.samples[i] = samples[i];
        }
        return output;
    }
} // namespace Dynamo::Sound