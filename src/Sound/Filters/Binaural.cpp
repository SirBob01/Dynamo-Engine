#include "./Binaural.hpp"

namespace Dynamo::Sound {
    Binaural::Binaural(HRTF &hrtf) : _hrtf(hrtf) {
        _impulse_response.resize(HRIR_LENGTH, 2);
        _output.set_channels(2);
    }

    Sound &Binaural::process(Sound &src,
                             u32 offset,
                             u32 length,
                             ListenerSet &listeners) {
        ListenerProperties &listener = listeners.find_closest(position);
        _hrtf.get().calculate_HRIR(listener.position,
                                   listener.rotation,
                                   position,
                                   _impulse_response);
        _output.set_frames(length);
        for (i32 c = 0; c < 2; c++) {
            _convolvers[c].initialize(_impulse_response[c],
                                      _impulse_response.frames());
            _convolvers[c].compute(src[c] + offset, _output[c], length);
        }
        return _output;
    }
} // namespace Dynamo::Sound