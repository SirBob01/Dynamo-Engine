#include "./Gain.hpp"

namespace Dynamo::Sound {
    void Gain::transform(Sound &src,
                         u32 offset,
                         u32 length,
                         ListenerSet &listeners) {
        Sound &output = get_output();
        output.resize(length, src.channels());
        for (u32 c = 0; c < src.channels(); c++) {
            for (u32 f = 0; f < length; f++) {
                output[c][f] = src[c][f + offset] * gain;
            }
        }
    }
} // namespace Dynamo::Sound