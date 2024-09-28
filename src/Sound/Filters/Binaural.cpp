#include <Sound/Filters/Binaural.hpp>

namespace Dynamo::Sound {
    Binaural::Binaural(HRTF &hrtf) :
        _hrtf(hrtf), _impulse_response(HRIR_LENGTH, 2) {}

    Sound &Binaural::apply(Sound &src,
                           const unsigned offset,
                           const unsigned length,
                           const Material &material,
                           const Listener &listener) {
        _hrtf.get().calculate_HRIR(listener.position,
                                   listener.rotation,
                                   material.position,
                                   _impulse_response);

        // Resize the scratch buffers
        _output.resize(length, 2);
        _remixed.resize(length, 2);

        // Remix to the desired number of channels
        _remixed.clear();
        src.remix(_remixed, offset, 0, length);

        // Apply convolution
        for (unsigned c = 0; c < 2; c++) {
            _convolvers[c].initialize(_impulse_response[c],
                                      _impulse_response.frames());
            _convolvers[c].compute(_remixed[c] + offset, _output[c], length);
        }
        return _output;
    }
} // namespace Dynamo::Sound