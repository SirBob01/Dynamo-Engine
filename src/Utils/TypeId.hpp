#pragma once

#include "../Types.hpp"

namespace Dynamo {
    /**
     * @brief Generate a unique identifier for a type at runtime.
     *
     */
    class TypeId {
        inline static u32 _id_counter = 0;

      public:
        /**
         * @brief Get the unique identifier of a type.
         *
         * @tparam Type.
         * @return u32
         */
        template <typename Type>
        inline static u32 get() {
            static const u32 id = _id_counter++;
            return id;
        }
    };
} // namespace Dynamo
