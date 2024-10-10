#pragma once

#include <Sound/Filter.hpp>

namespace Dynamo::Sound {
    /**
     * @brief Sequence of filters to be applied in order.
     *
     */
    class FilterSequence : public Filter {
        std::vector<FilterRef> _sequence;

      public:
        void apply(const Buffer &src, Buffer &dst, const Source &source, const Listener &listener) override;

        /**
         * @brief Add a filter.
         *
         * @param filter
         */
        void push(Filter &filter);
    };
} // namespace Dynamo::Sound