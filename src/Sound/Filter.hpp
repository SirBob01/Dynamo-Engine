#pragma once

#include <Sound/Listener.hpp>
#include <Sound/Material.hpp>
#include <Sound/Sound.hpp>

namespace Dynamo::Sound {
    /**
     * @brief Abstract audio filter node.
     *
     */
    class Filter {
      public:
        virtual ~Filter() = default;

        /**
         * @brief Apply the filter to a sound
         *
         * @param src        Source sound buffer
         * @param offset     Source frame start index
         * @param length     Length of the chunk to be processed
         * @param material   Material properties of the sound
         * @param listener   Listener
         * @return Sound&
         */
        virtual const Sound &apply(Sound &src,
                                   const unsigned offset,
                                   const unsigned length,
                                   const Material &material,
                                   const Listener &listener) = 0;
    };

    /**
     * @brief Filter reference.
     *
     */
    using FilterRef = std::reference_wrapper<Filter>;
} // namespace Dynamo::Sound