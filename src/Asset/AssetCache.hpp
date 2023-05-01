#pragma once

#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>

#include "../Log/Log.hpp"
#include "../Types.hpp"
#include "./Asset.hpp"
#include "./Loader/GeometryLoader.hpp"
#include "./Loader/SoundLoader.hpp"
#include "./Loader/TextureLoader.hpp"

namespace Dynamo {
    /**
     * @brief Load and cache engine resources loaded from disk.
     *
     * This will automatically release resource memory as soon as all fetched
     * handles are out of scope.
     *
     */
    class AssetCache {
        std::unordered_map<std::string, std::weak_ptr<void>> _pool;
        std::string _root;

        /**
         * @brief Store a new asset in the cache with a key.
         *
         * @tparam T
         * @param key
         * @param ptr
         */
        template <typename T>
        Asset<T> store(const std::string key, T &&object) {
            // Remove item from cache as soon as all handles are destroyed
            auto deleter = [&](T *data) {
                delete data;
                if (contains(key) && _pool.at(key).expired()) {
                    _pool.erase(key);
                }
            };
            Asset<T> asset(std::shared_ptr<T>(new T(object), deleter));
            _pool[key] = asset._data;
            return asset;
        }

        /**
         * @brief Load an asset from disk with the appropriate loader.
         *
         * @tparam T
         * @param filename
         */
        template <typename T>
        Asset<T> load(const std::string filename) {
            const std::string fullpath = _root + filename;
            if constexpr (std::is_same_v<T, Graphics::Geometry>) {
                return store(filename, GeometryLoader::load(fullpath));
            } else if constexpr (std::is_same_v<T, Sound::Sound>) {
                return store(filename, SoundLoader::load(fullpath));
            } else if constexpr (std::is_same_v<T, Graphics::Texture>) {
                return store(filename, TextureLoader::load(fullpath));
            } else {
                Log::error("Attempted to load unsupported asset type.");
            }
        }

      public:
        /**
         * @brief Construct a new AssetCache object.
         *
         * @param asset_directory Root asset directory.
         */
        AssetCache(const std::string asset_directory) :
            _root(asset_directory) {}

        /**
         * @brief Count the number of loaded assets.
         *
         * @return u32
         */
        inline u32 size() { return _pool.size(); }

        /**
         * @brief Check if an asset file is in cache.
         *
         * @param filename Path to the file relative to the root directory.
         * @return true
         * @return false
         */
        inline b8 contains(const std::string filename) {
            return _pool.count(filename) > 0;
        }

        /**
         * @brief Fetch an asset from the cache.
         *
         * @param filename Path to the file relative to the root directory.
         * @return Asset<T>
         */
        template <typename T>
        inline Asset<T> get(const std::string filename) {
            if (!contains(filename)) {
                return load<T>(filename);
            }
            auto ptr = std::static_pointer_cast<T>(_pool.at(filename).lock());
            return Asset<T>(ptr);
        }
    };
} // namespace Dynamo