#pragma once

#include <cstdint>
#include <queue>
#include <vector>

namespace Dynamo {
    /**
     * @brief Unique identifier supports up to 2^32 - 1 entities simultaneously
     *
     */
    using Id = uint64_t;

    /**
     * @brief Generate and discard ids
     *
     */
    class IdTracker {
        uint32_t _index_counter;

        std::vector<uint32_t> _version_map;
        std::queue<uint32_t> _free_indices;

      public:
        /**
         * @brief Construct a new IdTracker object
         *
         */
        IdTracker() : _index_counter(0) {}

        /**
         * @brief Get the index of an id
         *
         * @param id
         * @return unsigned
         */
        static inline unsigned get_index(Id id) { return id >> 32; }

        /**
         * @brief Get the version of an id
         *
         * @param id
         * @return unsigned
         */
        static inline unsigned get_version(Id id) { return id & 0xFFFFFFFF; }

        /**
         * @brief Test if an id is active
         *
         * @param id
         * @return true
         * @return false
         */
        inline bool is_active(Id id) {
            uint32_t index = IdTracker::get_index(id);
            uint32_t version = IdTracker::get_version(id);

            return index < _index_counter && _version_map[index] == version;
        }

        /**
         * @brief Generate an id
         *
         * This will recycle previously discarded ids if available
         *
         * @return Id
         */
        inline Id generate() {
            uint32_t index, version;
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
            Id id = index;
            return (id << 32) | version;
        }

        /**
         * @brief Discard an id
         *
         * This will tag the id as inactive and recyclable
         *
         * @param id
         */
        inline void discard(Id id) {
            if (!is_active(id)) return;
            uint32_t index = IdTracker::get_index(id);
            _version_map[index]++;
            _free_indices.push(index);
        }

        /**
         * @brief Clear the id pool
         *
         */
        inline void clear() {
            _index_counter = 0;
            _version_map.clear();
            _free_indices = {};
        }
    };
} // namespace Dynamo