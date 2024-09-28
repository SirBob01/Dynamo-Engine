#pragma once

#include <optional>

#include <Sound/Filter.hpp>
#include <Sound/Material.hpp>
#include <Sound/Sound.hpp>

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
     * @brief A unit of Sound with associated processing data.
     *
     */
    struct Chunk {
        /**
         * @brief Reference to the Sound data.
         *
         */
        SoundRef sound;

        /**
         * @brief Material properties.
         *
         */
        MaterialRef material;

        /**
         * @brief Optional filter.
         *
         */
        std::optional<FilterRef> filter;

        /**
         * @brief Frame offset of the chunk.
         *
         */
        float frame;
    };
} // namespace Dynamo::Sound