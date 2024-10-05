#include <Sound/Filters/Binaural.hpp>
#include <Sound/Source.hpp>

namespace Dynamo::Sound {
    void Binaural::apply(const Buffer &src,
                         Buffer &dst,
                         const Source &source,
                         const Listener &listener) {
        _impulse_response.resize(HRIR_LENGTH, 2);
        _hrtf.calculate_HRIR(listener.position,
                             listener.rotation,
                             source.position,
                             _impulse_response);

        // Downmix the source buffer to mono
        _mono.resize(src.frames(), 1);
        _mono.silence();
        src.remix(_mono);

        // Resize the destination buffer
        dst.resize(src.frames(), 2);

        // Initialize channel convolvers
        _convolvers[0].initialize(_impulse_response[0],
                                  _impulse_response.frames());
        _convolvers[1].initialize(_impulse_response[1],
                                  _impulse_response.frames());

        // Apply convolution
        _convolvers[0].compute(_mono[0], dst[0], src.frames());
        _convolvers[1].compute(_mono[0], dst[1], src.frames());
    }
} // namespace Dynamo::Sound