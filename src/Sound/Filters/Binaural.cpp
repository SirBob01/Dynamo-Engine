#include <Sound/Filters/Binaural.hpp>

namespace Dynamo::Sound {
    Binaural::Binaural() : _impulse_response(HRIR_LENGTH, 2) {}

    void Binaural::apply(const Sound &src,
                         Sound &dst,
                         const Material &material,
                         const Listener &listener) {
        _hrtf.calculate_HRIR(listener.position,
                             listener.rotation,
                             material.position,
                             _impulse_response);

        // Remix the source buffer to the desired number of channels
        _remixed.clear();
        _remixed.resize(src.frames(), 2);
        src.remix(_remixed, 0, 0, src.frames());

        // Resize the destination buffer
        dst.resize(src.frames(), 2);

        // Apply convolution
        for (unsigned c = 0; c < 2; c++) {
            _convolvers[c].initialize(_impulse_response[c],
                                      _impulse_response.frames());
            _convolvers[c].compute(_remixed[c], dst[c], src.frames());
        }
    }
} // namespace Dynamo::Sound