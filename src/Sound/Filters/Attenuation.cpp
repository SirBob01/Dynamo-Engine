#include "./Attenuation.hpp"

namespace Dynamo::Sound {
    Attenuation::Attenuation(f32 inner_radius, f32 cutoff_radius) :
        _inner_radius(inner_radius), _cutoff_radius(cutoff_radius) {}

    f32 Attenuation::linear(f32 distance) {
        if (distance < _inner_radius) {
            return 1;
        }
        if (distance > _cutoff_radius) {
            return 0;
        }
        return (_cutoff_radius - distance) / (_cutoff_radius - _inner_radius);
    }

    Sound &Attenuation::process(Sound &src,
                                u32 offset,
                                u32 length,
                                ListenerSet &listeners) {
        ListenerProperties &listener = listeners.find_closest(position);
        f32 distance = (position - listener.position).length();
        f32 gain = linear(distance);

        _output.resize(length, src.channels());
        for (u32 c = 0; c < _output.channels(); c++) {
            for (u32 f = 0; f < _output.frames(); f++) {
                _output[c][f] = src[c][f + offset] * gain;
            }
        }
        return _output;
    }
} // namespace Dynamo::Sound