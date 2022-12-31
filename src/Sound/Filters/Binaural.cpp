#include "./Binaural.hpp"

namespace Dynamo::Sound {
    Binaural::Binaural(HRTF &hrtf) : _hrtf(hrtf) {
        _impulse_response.resize(_hrtf.get_length(), 2);
        _output.set_channels(2);
    }

    Sound &Binaural::apply(Sound &src,
                           const unsigned src_offset,
                           const unsigned length,
                           const DynamicMaterial &material,
                           const ListenerProperties &listener) {
        _output.set_frames(length);
        _hrtf.get_impulse_response(listener.position,
                                   listener.rotation,
                                   material.position,
                                   _impulse_response);
        for (int c = 0; c < 2; c++) {
            _convolvers[c].initialize(_impulse_response[c],
                                      _impulse_response.frames());
            _convolvers[c].compute(src[c] + src_offset, _output[c], length);
        }
        return _output;
    }
} // namespace Dynamo::Sound