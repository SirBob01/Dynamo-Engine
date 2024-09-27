#include <Math/Vectorize.hpp>
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
        float distance = (material.position - listener.position).length();
        float gain = linear(distance);

        // Resize the output buffer
        _output.resize(length, src.channels());

        // Apply gain
        for (unsigned c = 0; c < _output.channels(); c++) {
            Vectorize::smul(src[c] + offset, gain, _output[c], length);
        }
        return _output;
    }
} // namespace Dynamo::Sound