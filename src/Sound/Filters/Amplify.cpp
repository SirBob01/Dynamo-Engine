#include <Math/Vectorize.hpp>
#include <Sound/Filters/Amplify.hpp>
#include <Sound/Source.hpp>

namespace Dynamo::Sound {
    void Amplify::apply(const Buffer &src,
                        Buffer &dst,
                        const Source &source,
                        const Listener &listener) {
        for (unsigned c = 0; c < src.channels(); c++) {
            Vectorize::smul(src[c], gain, dst[c], src.frames());
        }
    }
} // namespace Dynamo::Sound