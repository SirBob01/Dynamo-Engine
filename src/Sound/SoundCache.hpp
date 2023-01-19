#pragma once

#include <array>
#include <string>

#include <sndfile.hh>

#include "../Asset/AssetCache.hpp"
#include "../Log/Log.hpp"
#include "./Resample.hpp"
#include "./Sound.hpp"

namespace Dynamo::Sound {
    /**
     * @brief Sound asset cache
     *
     */
    class SoundCache : public AssetCache<Sound> {
      protected:
        /**
         * @brief Load a sound from a file
         *
         * @param filepath
         * @return Sound*
         */
        Sound *load(const std::string filepath) override;

      public:
        /**
         * @brief Construct a new SoundCache object
         *
         * @param asset_directory Root asset directory
         */
        SoundCache(const std::string asset_directory);
    };
} // namespace Dynamo::Sound