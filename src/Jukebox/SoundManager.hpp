#pragma once
#define STB_VORBIS_HEADER_ONLY

#include <string>

#include <stb_vorbis.c>

#include "../Asset/AssetManager.hpp"
#include "../Log/Log.hpp"
#include "./Sound.hpp"

namespace Dynamo {
    /**
     * @brief Sound asset manager
     *
     */
    class SoundManager : public AssetManager<Sound> {
      public:
        /**
         * @brief Load a sound from disk
         *
         * @param filename
         * @return Asset<Sound>
         */
        Asset<Sound> load_file(std::string filename);
    };
} // namespace Dynamo