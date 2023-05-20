#include "./Gain.hpp"

namespace Dynamo::Sound {
    void Gain::transform(FilterContext context) {
        Sound &src = context.input;
        Sound &dst = context.output;
        dst.resize(src.frames(), src.channels());
        for (u32 c = 0; c < src.channels(); c++) {
            for (u32 f = 0; f < src.frames(); f++) {
                dst[c][f] = src[c][f] * gain;
            }
        }
    }
} // namespace Dynamo::Sound