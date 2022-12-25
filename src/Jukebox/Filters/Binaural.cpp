#include "./Binaural.hpp"

namespace Dynamo {
    Binaural::Binaural(HRTF &hrtf) : _hrtf(hrtf) {
        _coeffs.resize(_hrtf.get_filter_length(), 2);
        _output.set_channels(2);
    }

    Sound &Binaural::apply(Sound &src,
                           const unsigned src_offset,
                           const unsigned length,
                           const DynamicSoundMaterial &material,
                           const ListenerProperties &listener) {
        _output.set_frames(length);
        _hrtf.get_coefficients(listener.position,
                               listener.rotation,
                               material.position,
                               _coeffs);
        for (int c = 0; c < 2; c++) {
            _convolvers[c].compute(src[c] + src_offset,
                                   _output[c],
                                   _coeffs[c],
                                   length,
                                   _coeffs.frames());
        }
        return _output;
    }
} // namespace Dynamo