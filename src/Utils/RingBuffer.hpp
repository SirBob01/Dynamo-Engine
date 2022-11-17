#pragma once

#include <array>

namespace Dynamo {
    /**
     * @brief A circular buffer such that writing will automatically loop back
     * at the start of the buffer as it overflows, discarding old data
     *
     * The buffer keeps track of a read and write pointer
     * The buffer is full if write is just behind read
     * The buffer is empty if the read and write point to the same index
     *
     * @tparam T Type of element
     * @tparam N Maximum size of the container (power of 2)
     */
    template <typename T, unsigned N>
    class RingBuffer {
        static const unsigned MASK = N - 1;
        static_assert(N > 0 && (N & MASK) == 0,
                      "RingBuffer size (> 0) should be a power of 2");

        std::array<T, N> _buffer;
        unsigned _read;
        unsigned _write;

      public:
        /**
         * @brief Construct a new RingBuffer object
         *
         */
        RingBuffer() : _read(0), _write(0) {}

        /**
         * @brief Check if the buffer is full
         *
         * @return true
         * @return false
         */
        inline bool is_full() { return size() == N; }

        /**
         * @brief Check if the buffer is empty
         *
         * @return true
         * @return false
         */
        inline bool is_empty() { return size() == 0; }

        /**
         * @brief Get the size of the buffer
         *
         * @return unsigned
         */
        inline unsigned size() { return (_write - _read); }

        /**
         * @brief Get the number of writes that can still be performed
         *
         * @return unsigned
         */
        inline unsigned remaining() { return N - size(); }

        /**
         * @brief Read a value from the buffer, advancing the read pointer
         *
         * @return T
         */
        inline T read() {
            DYN_ASSERT(!is_empty());
            return _buffer[_read++ & MASK];
        }

        /**
         * @brief Write a value into the buffer, advancing the write pointer
         *
         * @param value
         */
        inline void write(T value) {
            DYN_ASSERT(!is_full());
            _buffer[_write++ & MASK] = value;
        }

        /**
         * @brief Pop a value from the buffer, shifting back the write pointer
         *
         */
        inline void pop() {
            DYN_ASSERT(!is_empty());
            _write--;
        }

        /**
         * @brief Empty the buffer
         *
         */
        inline void clear() {
            _read = 0;
            _write = 0;
        }
    };
} // namespace Dynamo