#include <Sound/Filters/Stereo.hpp>

namespace Dynamo::Sound {
    Stereo::Stereo() : _output(0, 2) {}

    Sound &Stereo::apply(Sound &src,
                         const unsigned offset,
                         const unsigned length,
                         const Material &material,
                         const ListenerProperties &listener) {
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
        float l_gain = std::sqrt(1 - pan);
        float r_gain = std::sqrt(pan);

        _output.set_frames(length);
        for (unsigned c = 0; c < 2; c++) {
            float gain = c == 0 ? l_gain : r_gain;
            for (unsigned f = 0; f < length; f++) {
                _output[c][f] = src[c][f + offset] * gain;
            }
        }
        return _output;
    }
} // namespace Dynamo::Sound