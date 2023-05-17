#pragma once

#include "../Types.hpp"
#include "./Filter.hpp"
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
    static constexpr u32 MAX_CHUNK_LENGTH = 1 << 9;

    /**
     * @brief A chunk contains information to process a sound in small sections
     * every tick.
     *
     */
    struct Chunk {
        /**
         * @brief Reference to the Sound data.
         *
         */
        std::reference_wrapper<Sound> sound;

        /**
         * @brief Reference to the audio processing filter graph.
         *
         */
        std::reference_wrapper<Filter> filter;

        /**
         * @brief Frame offset of the chunk.
         *
         */
        f32 frame;
    };
} // namespace Dynamo::Sound