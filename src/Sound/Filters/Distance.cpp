#include <Math/Vectorize.hpp>
#include <Sound/Filters/Distance.hpp>

namespace Dynamo::Sound {
    float Distance::linear(float distance) {
        if (distance < _inner_radius) {
            return 1;
        }
        if (distance > _outer_radius) {
            return 0;
        }
        return (_outer_radius - distance) / (_outer_radius - _inner_radius);
    }

    void Distance::set_inner_radius(float radius) { _inner_radius = radius; }

    void Distance::set_outer_radius(float radius) { _outer_radius = radius; }

    void Distance::apply(const Sound &src,
                         Sound &dst,
                         const Material &material,
                         const Listener &listener) {
        float distance = (material.position - listener.position).length();
        float gain = linear(distance);

        // Apply gain
        for (unsigned c = 0; c < src.channels(); c++) {
            Vectorize::smul(src[c], gain, dst[c], src.frames());
        }
    }
} // namespace Dynamo::Sound