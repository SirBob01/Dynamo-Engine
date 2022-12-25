#include "./Stereo.hpp"

namespace Dynamo {
    Stereo::Stereo() { _output.set_channels(2); }

    Sound &Stereo::apply(Sound &src,
                         const unsigned src_offset,
                         const unsigned length,
                         const DynamicSoundMaterial &material,
                         const ListenerProperties &listener) {
        Vec3 delta = material.position - listener.position;
        Vec3 up = listener.rotation.up();
        Vec3 right = listener.rotation.right();
        Vec3 displacement = (delta - up * (delta * up));
        double distance = displacement.length();

        // Assume that if the distance is 0, the sound is centered
        double pan = 0.5;
        if (distance > 0) {
            Vec3 direction = displacement / distance;
            pan = ((direction * right) + 1) * 0.5;
        }

        // Square-law panning
        double l_gain = std::sqrt(1 - pan);
        double r_gain = std::sqrt(pan);

        _output.set_frames(length);
        for (unsigned c = 0; c < 2; c++) {
            double gain = c == 0 ? l_gain : r_gain;
            for (unsigned f = 0; f < length; f++) {
                _output[c][f] = src[c][f + src_offset] * gain;
            }
        }
        return _output;
    }
} // namespace Dynamo