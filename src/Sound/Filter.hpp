#pragma once

#include <Sound/Buffer.hpp>
#include <Sound/Listener.hpp>

namespace Dynamo::Sound {
    class Source;

    /**
     * @brief Abstract sound filter node.
     *
     */
    struct Filter {
      public:
        virtual ~Filter() = default;

        /**
         * @brief Apply the filter to a sound.
         *
         * Implementations must support the case where src == dst.
         *
         * @param src
         * @param dst
         * @param source
         * @param listener
         * @return Sound&
         */
        virtual void apply(const Buffer &src, Buffer &dst, const Source &source, const Listener &listener) = 0;
    };

    /**
     * @brief Filter reference.
     *
     */
    using FilterRef = std::reference_wrapper<Filter>;
} // namespace Dynamo::Sound