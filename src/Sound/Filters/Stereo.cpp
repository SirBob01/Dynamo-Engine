#include "./Stereo.hpp"

namespace Dynamo::Sound {
    Stereo::Stereo() : Filter(2) {}

    void Stereo::transform(FilterContext context) {
        Sound &src = context.input;
        Sound &dst = context.output;
        ListenerSet &listeners = context.listeners;
        ListenerProperties &listener = listeners.find_closest(position);

        // Compute distance
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

        dst.resize(src.frames(), 2);
        for (u32 c = 0; c < 2; c++) {
            f32 gain = c == 0 ? l_gain : r_gain;
            for (u32 f = 0; f < src.frames(); f++) {
                dst[c][f] = src[c][f] * gain;
            }
        }
    }
} // namespace Dynamo::Sound