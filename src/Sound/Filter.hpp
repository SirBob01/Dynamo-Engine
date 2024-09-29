#pragma once

#include <Sound/Listener.hpp>
#include <Sound/Material.hpp>
#include <Sound/Sound.hpp>

namespace Dynamo::Sound {
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
         * @param src      Source sound buffer
         * @param dst      Destination sound buffer
         * @param material Playback properties
         * @param listener Listener
         * @return Sound&
         */
        virtual void apply(const Sound &src,
                           Sound &dst,
                           const Material &material,
                           const Listener &listener) = 0;
    };

    /**
     * @brief Filter reference.
     *
     */
    using FilterRef = std::reference_wrapper<Filter>;
} // namespace Dynamo::Sound