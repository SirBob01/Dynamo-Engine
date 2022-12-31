#pragma once

#include "../../Math/Quaternion.hpp"
#include "../../Math/Vec3.hpp"

#include "../Listener.hpp"
#include "../Sound.hpp"
#include "../Material.hpp"

namespace Dynamo::Sound {
    /**
     * @brief Abstract base class for a static audio filter
     *
     */
    class StaticFilter {
      public:
        virtual ~StaticFilter() = default;

        /**
         * @brief Apply the filter to a sound
         *
         * @param src        Source sound buffer
         * @param src_offset Source frame start index
         * @param length     Length of the chunk to be processed
         * @return Sound&
         */
        virtual Sound &
        apply(Sound &src, const unsigned src_offset, const unsigned length) = 0;
    };

    /**
     * @brief Abstract base class for a dynamic audio filter, which considers
     * spatial information about the listener and sound source
     *
     */
    class DynamicFilter {
      public:
        virtual ~DynamicFilter() = default;

        /**
         * @brief Apply the filter to a sound
         *
         * @param src        Source sound buffer
         * @param src_offset Source frame start index
         * @param length     Length of the chunk to be processed
         * @param material   Sound material
         * @param listener   Listener
         * @return Sound&
         */
        virtual Sound &apply(Sound &src,
                             const unsigned src_offset,
                             const unsigned length,
                             const DynamicMaterial &material,
                             const ListenerProperties &listener) = 0;
    };
} // namespace Dynamo::Sound