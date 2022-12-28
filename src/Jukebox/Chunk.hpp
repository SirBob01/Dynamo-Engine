#pragma once

#include "./Sound.hpp"
#include "./SoundMaterial.hpp"

namespace Dynamo {
    /**
     * @brief Number of frames in a chunk
     *
     * A larger chunk will demand more compute time, especially when
     * processing multiple simultneously, causing latency
     *
     * However, a smaller chunk may not allow the output device
     * to receive enough data when requested, causing glitches
     *
     */
    constexpr unsigned CHUNK_LENGTH = 1 << 8;

    /**
     * @brief A chunk contains information to process a sound in small sections
     * every tick
     *
     * @tparam SoundMaterial Playback properties
     */
    template <typename SoundMaterial>
    struct Chunk {
        static_assert(
            std::is_convertible<SoundMaterial, StaticSoundMaterial>::value ||
            std::is_convertible<SoundMaterial, DynamicSoundMaterial>::value);
        /**
         * @brief Reference to the Sound data
         *
         */
        std::reference_wrapper<Sound> sound;

        /**
         * @brief Reference to the playback material
         *
         */
        std::reference_wrapper<SoundMaterial> material;

        /**
         * @brief Frame offset of the chunk
         *
         */
        float frame;
    };
} // namespace Dynamo