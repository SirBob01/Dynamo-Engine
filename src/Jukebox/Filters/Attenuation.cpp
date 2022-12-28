#include "./Attenuation.hpp"

namespace Dynamo {
    Attenuation::Attenuation(float inner_radius, float cutoff_radius) :
        _inner_radius(inner_radius), _cutoff_radius(cutoff_radius) {}

    float Attenuation::linear(float distance) {
        if (distance < _inner_radius) {
            return 1;
        }
        if (distance > _cutoff_radius) {
            return 0;
        }
        return (_cutoff_radius - distance) / (_cutoff_radius - _inner_radius);
    }

    Sound &Attenuation::apply(Sound &src,
                              const unsigned src_offset,
                              const unsigned length,
                              const DynamicSoundMaterial &material,
                              const ListenerProperties &listener) {
        _output.resize(length, src.channels());
        float distance = (material.position - listener.position).length();
        float gain = linear(distance);

        for (unsigned c = 0; c < _output.channels(); c++) {
            for (unsigned f = 0; f < _output.frames(); f++) {
                _output[c][f] = src[c][f + src_offset] * gain;
            }
        }
        return _output;
    }
} // namespace Dynamo