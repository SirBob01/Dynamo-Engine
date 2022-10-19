#pragma once

#include <memory>
#include <vector>

#include "../Log/Log.hpp"

#include "Resource.hpp"

namespace Dynamo {
    /**
     * @brief Resource management pool
     *
     */
    template <typename T>
    class ResourceManager {
        /**
         * @brief Instance of a resource within a pool
         *
         * Generation number ensures that recycling indices does not cause
         * collisions
         *
         */
        struct ResourceInstance {
            std::unique_ptr<T> data;
            unsigned long generation = 0;
        };

        std::vector<ResourceInstance> _pool;
        std::vector<unsigned long> _free_cells;

        /**
         * @brief Find a free cell in the resource list
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
         * @brief Clear all resources
         *
         */
        inline void clear() { _pool.clear(); }

        /**
         * @brief Count the number of loaded resources
         *
         * @return unsigned
         */
        inline unsigned size() { return _pool.size() - _free_cells.size(); }

        /**
         * @brief Test if a resource exists in the pool
         *
         * @param resource Resource handle
         * @return true
         * @return false
         */
        inline bool contains(Resource<T> resource) {
            // Test if instance is in the pool
            if (resource.index >= _pool.size()) {
                return false;
            }

            // Test if instance and handle have matching generations
            auto &instance = _pool[resource.index];
            if (resource.generation != instance.generation) {
                return false;
            }
            return true;
        }

        /**
         * @brief Load a resource and grab its handle
         *
         * @param args Parameters for constructing the resource
         * @return Resource<T> Resource handle
         */
        template <typename... Param>
        inline Resource<T> load(Param... args) {
            unsigned long index = find_free_cell();
            if (index >= _pool.size()) {
                _pool.emplace_back();
            }

            // Actually load the resource
            _pool[index].data = std::make_unique<T>(args...);
            return {index, _pool[index].generation};
        }

        /**
         * @brief Unload a resource
         *
         * @param resource Resource handle
         */
        inline void unload(Resource<T> resource) {
            if (!contains(resource)) {
                Log::error("Resource does not exist");
            }
            // Free the resource and increment the generation
            auto &instance = _pool[resource.index];
            instance.data.reset();
            instance.generation++;

            // Recycle the index so it can be reused
            _free_cells.push_back(resource.index);
        }

        /**
         * @brief Get the raw data to a resource
         *
         * @param resource Resource handle
         * @return T& Data of the resource
         */
        inline T &get(Resource<T> resource) {
            if (!contains(resource)) {
                Log::error("Resource does not exist");
            }
            auto &instance = _pool[resource.index];
            return *instance.data;
        }
    };
} // namespace Dynamo