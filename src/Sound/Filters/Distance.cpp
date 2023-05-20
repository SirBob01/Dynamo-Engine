#include "./Distance.hpp"

namespace Dynamo::Sound {
    Distance::Distance(f32 inner_radius, f32 cutoff_radius) :
        _inner_radius(inner_radius), _cutoff_radius(cutoff_radius) {}

    f32 Distance::linear(f32 distance) {
        if (distance < _inner_radius) {
            return 1;
        }
        if (distance > _cutoff_radius) {
            return 0;
        }
        return (_cutoff_radius - distance) / (_cutoff_radius - _inner_radius);
    }

    void Distance::transform(FilterContext context) {
        Sound &src = context.input;
        Sound &dst = context.output;
        ListenerSet &listeners = context.listeners;
        ListenerProperties &listener = listeners.find_closest(position);
        f32 distance = (position - listener.position).length();
        f32 gain = linear(distance);

        dst.resize(src.frames(), src.channels());
        for (u32 c = 0; c < dst.channels(); c++) {
            for (u32 f = 0; f < dst.frames(); f++) {
                dst[c][f] = src[c][f] * gain;
            }
        }
    }
} // namespace Dynamo::Sound