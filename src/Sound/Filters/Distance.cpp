#include <Math/Vectorize.hpp>
#include <Sound/Filters/Distance.hpp>
#include <Sound/Source.hpp>

namespace Dynamo::Sound {
    float Distance::linear(float distance) {
        if (distance < inner_radius) {
            return 1;
        }
        if (distance > outer_radius) {
            return 0;
        }
        return (outer_radius - distance) / (outer_radius - inner_radius);
    }

    void Distance::apply(const Buffer &src, Buffer &dst, const Source &source, const Listener &listener) {
        float distance = (source.position - listener.position).length();
        float gain = linear(distance);

        // Apply gain
        for (unsigned c = 0; c < src.channels(); c++) {
            Vectorize::smul(src[c], gain, dst[c], src.frames());
        }
    }
} // namespace Dynamo::Sound