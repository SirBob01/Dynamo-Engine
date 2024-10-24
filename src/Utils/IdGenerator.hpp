#pragma once

#include <cstdint>
#include <type_traits>

#define DYN_DEFINE_ID_TYPE(T) using T = struct T##_t *

namespace Dynamo {
    /**
     * @brief Generate typesafe handles.
     *
     * @tparam Id
     */
    template <typename Id>
    class IdGenerator {
        static_assert(std::is_pointer<Id>::value, "Id must be a valid handle type (opaque pointer).");
        static_assert(sizeof(Id) == sizeof(uintptr_t), "Id and its integer mode must be the same size.");
        static inline uintptr_t _counter = 0;

      public:
        /**
         * @brief Generate an id.
         *
         * @return Id
         */
        static inline Id generate() { return reinterpret_cast<Id>(_counter++); }

        /**
         * @brief Get the key of the id.
         *
         * @param id
         * @return uintptr_t
         */
        static inline uintptr_t key(Id id) { return reinterpret_cast<uintptr_t>(id); }
    };
} // namespace Dynamo