#include "./Binaural.hpp"

namespace Dynamo::Sound {
    Binaural::Binaural() {
        _impulse_response.resize(HRIR_LENGTH, 2);
        _output.set_channels(2);
    }

    Sound &Binaural::apply(Sound &src,
                           const u32 src_offset,
                           const u32 length,
                           const DynamicMaterial &material,
                           const ListenerProperties &listener) {
        _output.set_frames(length);
        calculate_HRIR(listener.position,
                       listener.rotation,
                       material.position,
                       _impulse_response);
        for (i32 c = 0; c < 2; c++) {
            _convolvers[c].initialize(_impulse_response[c],
                                      _impulse_response.frames());
            _convolvers[c].compute(src[c] + src_offset, _output[c], length);
        }
        return _output;
    }
} // namespace Dynamo::Sound