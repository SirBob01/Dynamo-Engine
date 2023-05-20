#include "./Binaural.hpp"

namespace Dynamo::Sound {
    Binaural::Binaural(HRTF &hrtf) : Filter(2), _hrtf(hrtf) {
        _impulse_response.resize(HRIR_LENGTH, 2);
    }

    void Binaural::transform(FilterContext context) {
        Sound &src = context.input;
        Sound &dst = context.output;
        ListenerSet &listeners = context.listeners;
        ListenerProperties &listener = listeners.find_closest(position);
        _hrtf.get().calculate_HRIR(listener.position,
                                   listener.rotation,
                                   position,
                                   _impulse_response);

        dst.set_frames(src.frames());
        for (i32 c = 0; c < 2; c++) {
            _convolvers[c].initialize(_impulse_response[c],
                                      _impulse_response.frames());
            _convolvers[c].compute(src[c], dst[c], src.frames());
        }
    }
} // namespace Dynamo::Sound