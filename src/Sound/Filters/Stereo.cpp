#include "./Stereo.hpp"

namespace Dynamo::Sound {
    void Stereo::transform(u32 offset, u32 length, ListenerSet &listeners) {
        ListenerProperties &listener = listeners.find_closest(position);
        Vec3 delta = position - listener.position;
        Vec3 up = listener.rotation.up();
        Vec3 right = listener.rotation.right();
        Vec3 displacement = (delta - up * (delta * up));
        f32 distance = displacement.length();

        // Assume that if the distance is 0, the sound is centered
        f32 pan = 0.5;
        if (distance > 0) {
            Vec3 direction = displacement / distance;
            pan = ((direction * right) + 1) * 0.5;
        }

        // Square-law panning
        f32 l_gain = std::sqrt(1 - pan);
        f32 r_gain = std::sqrt(pan);

        Sound &src = get_input();
        Sound &dst = get_output();
        dst.set_frames(length);
        for (u32 c = 0; c < 2; c++) {
            f32 gain = c == 0 ? l_gain : r_gain;
            for (u32 f = 0; f < length; f++) {
                dst[c][f] = src[c][f + offset] * gain;
            }
        }
    }
} // namespace Dynamo::Sound