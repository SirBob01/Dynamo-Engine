#include <Math/Vectorize.hpp>
#include <Sound/Filters/Stereo.hpp>
#include <Sound/Source.hpp>

namespace Dynamo::Sound {
    void Stereo::apply(const Buffer &src, Buffer &dst, const Source &source, const Listener &listener) {
        Vec3 delta = source.position - listener.position;
        Vec3 up = listener.rotation.up();
        Vec3 right = listener.rotation.right();
        Vec3 displacement = (delta - up * (delta * up));
        float distance = displacement.length();

        // Assume that if the distance is 0, the sound is centered
        float pan = 0.5;
        if (distance > 0) {
            Vec3 direction = displacement / distance;
            pan = ((direction * right) + 1) * 0.5;
        }

        // Downmix the source buffer to mono
        _mono.resize(src.frames(), 1);
        _mono.silence();
        src.remix(_mono);

        // Resize the destination buffer
        dst.resize(src.frames(), 2);

        // Square-law panning
        Vectorize::smul(_mono[0], std::sqrt(1.0f - pan), dst[0], src.frames());
        Vectorize::smul(_mono[0], std::sqrt(pan), dst[1], src.frames());
    }
} // namespace Dynamo::Sound