#include "Sound.hpp"

namespace Dynamo::Sound {
    WaveFrame Sound::get_frame(const unsigned frame,
                               const unsigned out_channels) {
        WaveFrame output;
        output.channels = out_channels;

        std::array<float, 2> samples = {0, 0};
        for (unsigned src_c = 0; src_c < channels(); src_c++) {
            WaveSample sample = at(frame, src_c);

            // Perform weighted sum using downmixing coefficient matrix
            int index = static_cast<int>(CHANNEL_ORDERS[channels() - 1][src_c]);
            for (unsigned dst_c = 0; dst_c < output.channels; dst_c++) {
                float coeff = DOWNMIX_COEFFS[out_channels - 1][index][dst_c];
                samples[dst_c] += sample * coeff;
            }
        }

        // Write denormalzed samples onto the output frame
        for (int i = 0; i < samples.size(); i++) {
            output.samples[i] = samples[i];
        }
        return output;
    }
} // namespace Dynamo::Sound