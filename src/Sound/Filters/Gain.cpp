#include "./Gain.hpp"

namespace Dynamo::Sound {
    Sound &Gain::transform(Sound &src,
                           u32 offset,
                           u32 length,
                           ListenerSet &listeners) {
        _output.resize(length, src.channels());
        for (u32 c = 0; c < src.channels(); c++) {
            for (u32 f = 0; f < length; f++) {
                _output[c][f] = src[c][f + offset] * gain;
            }
        }
        return _output;
    }
} // namespace Dynamo::Sound