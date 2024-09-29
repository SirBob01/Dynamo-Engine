#include <Math/Vectorize.hpp>
#include <Sound/Filters/Amplify.hpp>

namespace Dynamo::Sound {
    void Amplify::set_gain(float gain) { _gain = gain; }

    void Amplify::apply(const Sound &src,
                        Sound &dst,
                        const Material &material,
                        const Listener &listener) {
        for (unsigned c = 0; c < src.channels(); c++) {
            Vectorize::smul(src[c], _gain, dst[c], src.frames());
        }
    }
} // namespace Dynamo::Sound