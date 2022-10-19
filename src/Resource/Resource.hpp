#pragma once

namespace Dynamo {
    /**
     * @brief Resource handle
     *
     * @tparam T Structure of the resource data
     */
    template <typename T>
    struct Resource {
        /**
         * @brief Index for accessing the resource
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