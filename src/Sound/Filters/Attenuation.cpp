#include <Sound/Filters/Attenuation.hpp>

namespace Dynamo::Sound {
    Attenuation::Attenuation(float inner_radius, float cutoff_radius) :
        _inner_radius(inner_radius), _outer_radius(cutoff_radius),
        _denominator(1 / (_outer_radius - _inner_radius)) {}

    float Attenuation::linear(float distance) {
        if (distance < _inner_radius) {
            return 1;
        }
        if (distance > _outer_radius) {
            return 0;
        }
        return (_outer_radius - distance) * _denominator;
    }

    Sound &Attenuation::apply(Sound &src,
                              const unsigned offset,
                              const unsigned length,
                              const Material &material,
                              const ListenerProperties &listener) {
        _output.resize(length, src.channels());
        float distance = (material.position - listener.position).length();
        float gain = linear(distance);

        for (unsigned c = 0; c < _output.channels(); c++) {
            for (unsigned f = 0; f < _output.frames(); f++) {
                _output[c][f] = src[c][f + offset] * gain;
            }
        }
        return _output;
    }
} // namespace Dynamo::Sound