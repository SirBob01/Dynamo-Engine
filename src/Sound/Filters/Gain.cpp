#include "./Gain.hpp"

namespace Dynamo::Sound {
    void Gain::transform(u32 offset, u32 length, ListenerSet &listeners) {
        Sound &src = get_input();
        Sound &dst = get_output();
        dst.resize(length, src.channels());
        for (u32 c = 0; c < src.channels(); c++) {
            for (u32 f = 0; f < length; f++) {
                dst[c][f] = src[c][f + offset] * gain;
            }
        }
    }
} // namespace Dynamo::Sound