#pragma once

#include <memory>

namespace Dynamo {
    /**
     * @brief Forward declaration of the asset cache
     *
     * @tparam T
     */
    template <typename T>
    class AssetCache;

    /**
     * @brief Asset handle
     *
     * @tparam T Structure of the asset resource
     */
    template <typename T>
    class Asset {
        std::shared_ptr<T> _data;

        /**
         * @brief Construct a new Asset object
         *
         * @param data Heap allocated pointer to the asset resource
         */
        explicit Asset(const std::shared_ptr<T> &data) : _data(data) {}

      public:
        friend class AssetCache<T>;

        /**
         * @brief Dereference operator
         *
         * @return T&
         */
        inline T &operator*() { return *_data; }

        /**
         * @brief Data pointer access operator
         *
         * @return T*
         */
        inline T *operator->() { return _data; }
    };
} // namespace Dynamo