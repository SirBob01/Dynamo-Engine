#include "./Binaural.hpp"

namespace Dynamo::Sound {
    Binaural::Binaural(HRTF &hrtf) :
        _hrtf(hrtf), _impulse_response(HRIR_LENGTH, 2), _output(0, 2) {}

    Sound &Binaural::apply(Sound &src,
                           const unsigned offset,
                           const unsigned length,
                           const Material &material,
                           const ListenerProperties &listener) {
        _output.set_frames(length);
        _hrtf.get().calculate_HRIR(listener.position,
                                   listener.rotation,
                                   material.position,
                                   _impulse_response);
        for (unsigned c = 0; c < 2; c++) {
            _convolvers[c].initialize(_impulse_response[c],
                                      _impulse_response.frames());
            _convolvers[c].compute(src[c] + offset, _output[c], length);
        }
        return _output;
    }
} // namespace Dynamo::Sound