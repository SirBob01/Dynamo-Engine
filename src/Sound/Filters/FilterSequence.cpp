#include "./FilterSequence.hpp"

namespace Dynamo::Sound {
    Sound &FilterSequence::apply(Sound &src,
                                 const unsigned offset,
                                 const unsigned length,
                                 const Material &material,
                                 const ListenerProperties &listener) {
        _output = src;
        for (Filter &filter : _sequence) {
            _output = filter.apply(_output, offset, length, material, listener);
        }
        return _output;
    }

    void FilterSequence::push(Filter &filter) { _sequence.push_back(filter); }
} // namespace Dynamo::Sound