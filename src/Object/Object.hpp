#pragma once

#include <atomic>

#include <Object/Ref.hpp>

namespace Dynamo {
    /**
     * @brief Base Dynamo object.
     *
     */
    class Object {
        mutable std::atomic_uint _ref_count = 1;

        template <class T>
        friend class Ref;

        /**
         * @brief Increase the reference counter.
         *
         */
        void ref() { _ref_count.fetch_add(1, std::memory_order_relaxed); }

        /**
         * @brief Decrease the reference counter.
         *
         */
        void deref() { _ref_count.fetch_sub(1, std::memory_order_relaxed); }

      public:
        virtual ~Object() = default;
    };
} // namespace Dynamo