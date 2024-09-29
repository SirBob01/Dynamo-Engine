#include <Sound/Filters/FilterSequence.hpp>

namespace Dynamo::Sound {
    void FilterSequence::apply(const Sound &src,
                               Sound &dst,
                               const Material &material,
                               const Listener &listener) {
        for (Filter &filter : _sequence) {
            filter.apply(src, dst, material, listener);
        }
    }

    void FilterSequence::push(Filter &filter) { _sequence.push_back(filter); }
} // namespace Dynamo::Sound