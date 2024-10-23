#pragma once

#include <cstdint>
#include <queue>
#include <vector>

#define DYN_DEFINE_ID_TYPE(T) using T = struct T##_t *

static_assert(sizeof(void *) == sizeof(uintptr_t), "Pointer and its integer mode must be same size.");

namespace Dynamo {
    /**
     * @brief Id index shift.
     *
     */
    constexpr unsigned long long ID_INDEX_SHIFT = (sizeof(uintptr_t) * 8) >> 1;

    /**
     * @brief Id version mask.
     *
     */
    constexpr unsigned long long ID_VERSION_MASK = (1ULL << ID_INDEX_SHIFT) - 1;

    /**
     * @brief Generate and discard typesafe handles.
     *
     * @tparam Id
     */
    template <typename Id>
    class IdTracker {
        unsigned _index_counter;

        std::vector<unsigned> _version_map;
        std::queue<unsigned> _free_indices;

      public:
        /**
         * @brief Construct a new IdTracker object.
         *
         */
        IdTracker() : _index_counter(0) {}

        /**
         * @brief Get the index of an id.
         *
         * @param id
         * @return unsigned
         */
        static inline unsigned get_index(Id id) { return reinterpret_cast<uintptr_t>(id) >> ID_INDEX_SHIFT; }

        /**
         * @brief Get the version of an id.
         *
         * @param id
         * @return unsigned
         */
        static inline unsigned get_version(Id id) { return reinterpret_cast<uintptr_t>(id) & ID_VERSION_MASK; }

        /**
         * @brief Test if an id is active.
         *
         * @param id
         * @return true
         * @return false
         */
        inline bool is_active(Id id) {
            unsigned index = IdTracker::get_index(id);
            unsigned version = IdTracker::get_version(id);

            return index < _index_counter && _version_map[index] == version;
        }

        /**
         * @brief Generate an id.
         *
         * This will recycle previously discarded ids if available.
         *
         * @return Id
         */
        inline Id generate() {
            unsigned index, version;
            if (_free_indices.size() > 0) {
                index = _free_indices.front();
                version = _version_map[index];
                _free_indices.pop();
            } else {
                index = _index_counter++;
                version = 0;
                _version_map.push_back(version);
            }

            // Move index to longer bitstring before compositing
            uintptr_t id = index;
            return reinterpret_cast<Id>((id << ID_INDEX_SHIFT) | version);
        }

        /**
         * @brief Discard an id. This will tag it as inactive and recyclable.
         *
         * @param id
         */
        inline void discard(Id id) {
            if (!is_active(id)) return;
            unsigned index = IdTracker::get_index(id);
            _version_map[index]++;
            _free_indices.push(index);
        }

        /**
         * @brief Clear the id pool.
         *
         */
        inline void clear() {
            _index_counter = 0;
            _version_map.clear();
            _free_indices = {};
        }
    };
} // namespace Dynamo