#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "../Log/Log.hpp"

#include "Asset.hpp"

namespace Dynamo {
    /**
     * @brief Load and cache engine resources loaded from disk
     *
     * This will automatically release resource memory as soon as all fetched
     * handles are out of scope
     *
     */
    template <typename T>
    class AssetCache {
        std::unordered_map<std::string, std::weak_ptr<T>> _pool;
        std::string _root;

      protected:
        /**
         * @brief Load the resource from disk
         *
         * This must return a heap allocated pointer to the resource
         *
         * @param filepath Full path to the file
         * @return T*
         */
        virtual T *load(const std::string filepath) = 0;

      public:
        /**
         * @brief Construct a new AssetCache object
         *
         * @param asset_directory Root asset directory
         */
        AssetCache(const std::string asset_directory) :
            _root(asset_directory) {}

        /**
         * @brief Count the number of loaded assets
         *
         * @return unsigned
         */
        inline unsigned size() { return _pool.size(); }

        /**
         * @brief Check if an asset file is in cache
         *
         * @param filename
         * @return true
         * @return false
         */
        inline bool contains(const std::string filename) {
            return _pool.count(filename) > 0;
        }

        /**
         * @brief Fetch an asset from the cache
         *
         * @param filename Path to the file relative to the root directory
         * @return Asset<T>
         */
        inline Asset<T> get(const std::string filename) {
            if (!contains(filename)) {
                // Custom deleter to release the resource as soon as all handles
                // are destroyed
                auto deleter = [&](T *data) {
                    delete data;
                    if (contains(filename) && _pool.at(filename).expired()) {
                        _pool.erase(filename);
                    }
                };

                T *ptr = load(_root + filename);
                Asset<T> asset(std::shared_ptr<T>(ptr, deleter));
                _pool[filename] = asset._data;

                return asset;
            }
            return Asset<T>(_pool.at(filename).lock());
        }
    };
} // namespace Dynamo