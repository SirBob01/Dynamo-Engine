#pragma once

#include <memory>
#include <vector>

#include "../Log/Log.hpp"

#include "Asset.hpp"

namespace Dynamo {
    /**
     * @brief Asset management pool
     *
     */
    template <typename T>
    class AssetManager {
        /**
         * @brief Instance of a asset within a pool
         *
         * Generation number ensures that recycling indices does not cause
         * collisions
         *
         */
        struct AssetInstance {
            std::unique_ptr<T> data;
            unsigned long generation = 0;
        };

        std::vector<AssetInstance> _pool;
        std::vector<unsigned long> _free_cells;

        /**
         * @brief Find a free cell in the asset list
         *
         * @return unsigned long
         */
        inline unsigned long find_free_cell() {
            if (_free_cells.empty()) {
                return _pool.size();
            }
            unsigned long index = _free_cells.back();
            _free_cells.pop_back();
            return index;
        }

      public:
        /**
         * @brief Clear all assets
         *
         */
        inline void clear() { _pool.clear(); }

        /**
         * @brief Count the number of loaded assets
         *
         * @return unsigned
         */
        inline unsigned size() { return _pool.size() - _free_cells.size(); }

        /**
         * @brief Test if a asset exists in the pool
         *
         * @param asset Asset handle
         * @return true
         * @return false
         */
        inline bool contains(Asset<T> asset) {
            // Test if instance is in the pool
            if (asset.index >= _pool.size()) {
                return false;
            }

            // Test if instance and handle have matching generations
            auto &instance = _pool[asset.index];
            if (asset.generation != instance.generation) {
                return false;
            }
            return true;
        }

        /**
         * @brief Allocate the asset and grab its handle
         *
         * This handle must be returned
         *
         * @param args Parameters for constructing the asset
         * @return Asset<T> Asset handle
         */
        template <typename... Params>
        Asset<T> allocate(Params &&...args) {
            unsigned long index = find_free_cell();
            if (index >= _pool.size()) {
                _pool.emplace_back();
            }
            if constexpr (std::is_constructible<T, Params...>::value) {
                _pool[index].data =
                    std::unique_ptr<T>(new T(std::forward<Params>(args)...));
            } else {
                _pool[index].data =
                    std::unique_ptr<T>(new T{std::forward<Params>(args)...});
            }
            return {index, _pool[index].generation};
        }

        /**
         * @brief Unload a asset
         *
         * @param asset Asset handle
         */
        inline void unload(Asset<T> asset) {
            if (!contains(asset)) {
                Log::error("Asset does not exist");
            }
            // Free the asset and increment the generation
            auto &instance = _pool[asset.index];
            instance.data.reset();
            instance.generation++;

            // Recycle the index so it can be reused
            _free_cells.push_back(asset.index);
        }

        /**
         * @brief Get the raw data to a asset
         *
         * @param asset Asset handle
         * @return T& Data of the asset
         */
        inline T &get(Asset<T> asset) {
            if (!contains(asset)) {
                Log::error("Asset does not exist");
            }
            auto &instance = _pool[asset.index];
            return *instance.data;
        }
    };
} // namespace Dynamo