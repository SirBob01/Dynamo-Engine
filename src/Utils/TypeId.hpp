#pragma once

namespace Dynamo {
    /**
     * @brief Generate a unique identifier for a type at runtime.
     *
     */
    class TypeId {
        inline static unsigned _id_counter = 0;

      public:
        /**
         * @brief Get the unique identifier of a type.
         *
         * @tparam Type.
         * @return unsigned
         */
        template <typename Type>
        inline static unsigned get() {
            static const unsigned id = _id_counter++;
            return id;
        }
    };
} // namespace Dynamo
