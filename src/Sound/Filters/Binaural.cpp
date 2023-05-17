#include "./Binaural.hpp"

namespace Dynamo::Sound {
    Binaural::Binaural(HRTF &hrtf) : _hrtf(hrtf) {
        Sound &output = get_output();
        output.set_channels(2);
        _impulse_response.resize(HRIR_LENGTH, 2);
    }

    void Binaural::transform(Sound &src,
                             u32 offset,
                             u32 length,
                             ListenerSet &listeners) {
        ListenerProperties &listener = listeners.find_closest(position);
        _hrtf.get().calculate_HRIR(listener.position,
                                   listener.rotation,
                                   position,
                                   _impulse_response);

        Sound &output = get_output();
        output.set_frames(length);
        for (i32 c = 0; c < 2; c++) {
            _convolvers[c].initialize(_impulse_response[c],
                                      _impulse_response.frames());
            _convolvers[c].compute(src[c] + offset, output[c], length);
        }
    }
} // namespace Dynamo::Sound