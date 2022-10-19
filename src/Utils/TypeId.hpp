#pragma once

namespace Dynamo {
    /**
     * @brief Bespoke method of generating a unique identifier
     * for a type at runtime
     *
     */
    class TypeId {
        unsigned _id_counter;

      public:
        /**
         * @brief Initialize a new counter for assigning identifiers to types
         *
         */
        TypeId() : _id_counter(0){};

        /**
         * @brief Get the unique identifier of a type
         *
         * @tparam Type
         * @return unsigned
         */
        template <typename Type>
        inline unsigned get() {
            static const unsigned id = _id_counter++;
            return id;
        }
    };
} // namespace Dynamo
