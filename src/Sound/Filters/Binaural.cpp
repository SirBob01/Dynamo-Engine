#include "./Binaural.hpp"

namespace Dynamo::Sound {
    Binaural::Binaural(HRTF &hrtf) : _hrtf(hrtf) {
        Sound &output = get_output();
        output.set_channels(2);
        _impulse_response.resize(HRIR_LENGTH, 2);
    }

    void Binaural::transform(u32 offset, u32 length, ListenerSet &listeners) {
        ListenerProperties &listener = listeners.find_closest(position);
        _hrtf.get().calculate_HRIR(listener.position,
                                   listener.rotation,
                                   position,
                                   _impulse_response);

        Sound &src = get_input();
        Sound &dst = get_output();
        dst.set_frames(length);
        for (i32 c = 0; c < 2; c++) {
            _convolvers[c].initialize(_impulse_response[c],
                                      _impulse_response.frames());
            _convolvers[c].compute(src[c] + offset, dst[c], length);
        }
    }
} // namespace Dynamo::Sound