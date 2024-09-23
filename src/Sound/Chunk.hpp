#pragma once

#include "./Material.hpp"
#include "./Sound.hpp"

namespace Dynamo::Sound {
    /**
     * @brief Maximum number of frames in a chunk.
     *
     * A larger chunk will demand more compute time, especially when
     * processing multiple simultneously, causing latency.
     *
     * However, a smaller chunk may not allow the output device
     * to receive enough data when requested, causing glitches.
     *
     */
    static constexpr unsigned MAX_CHUNK_LENGTH = 1 << 9;

    /**
     * @brief A chunk contains information to process a sound in small sections
     * every tick.
     *
     * @tparam Material Playback properties.
     */
    template <typename Material>
    struct Chunk {
        static_assert(std::is_convertible<Material, StaticMaterial>::value ||
                      std::is_convertible<Material, DynamicMaterial>::value);
        /**
         * @brief Reference to the Sound data.
         *
         */
        std::reference_wrapper<Sound> sound;

        /**
         * @brief Reference to the playback material.
         *
         */
        std::reference_wrapper<Material> material;

        /**
         * @brief Frame offset of the chunk.
         *
         */
        float frame;
    };
} // namespace Dynamo::Sound