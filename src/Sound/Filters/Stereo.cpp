#include <Math/Vectorize.hpp>
#include <Sound/Filters/Stereo.hpp>

namespace Dynamo::Sound {
    Sound &Stereo::apply(Sound &src,
                         const unsigned offset,
                         const unsigned length,
                         const Material &material,
                         const Listener &listener) {
        Vec3 delta = material.position - listener.position;
        Vec3 up = listener.rotation.up();
        Vec3 right = listener.rotation.right();
        Vec3 displacement = (delta - up * (delta * up));
        float distance = displacement.length();

        // Assume that if the distance is 0, the sound is centered
        float pan = 0.5;
        if (distance > 0) {
            Vec3 direction = displacement / distance;
            pan = ((direction * right) + 1) * 0.5;
        }

        // Square-law panning
        std::array<float, 2> channel_gains = {std::sqrt(1 - pan),
                                              std::sqrt(pan)};

        // Resize the scratch buffers
        _output.resize(length, 2);
        _remixed.resize(length, 2);

        // Remix to the desired number of channels
        _remixed.clear();
        src.remix(_remixed, offset, 0, length);

        // Apply per-channel gain
        for (unsigned c = 0; c < 2; c++) {
            Vectorize::smul(_remixed[c] + offset,
                            channel_gains[c],
                            _output[c],
                            length);
        }
        return _output;
    }
} // namespace Dynamo::Sound