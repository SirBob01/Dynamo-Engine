#include <Math/Vectorize.hpp>
#include <Sound/Filters/Stereo.hpp>

namespace Dynamo::Sound {
    void Stereo::apply(const Sound &src,
                       Sound &dst,
                       const Material &material,
                       const Listener &listener) {
        Vec3 delta = material.position - listener.position;
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

        // Square-law panning
        std::array<float, 2> gains = {std::sqrt(1.0f - pan), std::sqrt(pan)};

        // Remix the source buffer to the desired number of channels
        _remixed.clear();
        _remixed.resize(src.frames(), 2);
        src.remix(_remixed, 0, 0, src.frames());

        // Resize the destination buffer
        dst.resize(src.frames(), 2);

        // Apply per-channel gain
        for (unsigned c = 0; c < 2; c++) {
            Vectorize::smul(_remixed[c], gains[c], dst[c], src.frames());
        }
    }
} // namespace Dynamo::Sound