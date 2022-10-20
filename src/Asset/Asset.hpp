#pragma once

namespace Dynamo {
    /**
     * @brief Asset handle
     *
     * @tparam T Structure of the asset data
     */
    template <typename T>
    struct Asset {
        /**
         * @brief Index for accessing the asset
         *
         */
        const unsigned long index;

        /**
         * @brief Generation number
         *
         */
        const unsigned long generation;
    };
} // namespace Dynamo