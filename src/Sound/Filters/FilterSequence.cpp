#include <Sound/Filters/FilterSequence.hpp>

namespace Dynamo::Sound {
    void FilterSequence::apply(const Buffer &src,
                               Buffer &dst,
                               const Source &source,
                               const Listener &listener) {
        for (Filter &filter : _sequence) {
            filter.apply(src, dst, source, listener);
        }
    }

    void FilterSequence::push(Filter &filter) {
        _sequence.emplace_back(filter);
    }
} // namespace Dynamo::Sound