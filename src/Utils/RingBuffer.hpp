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
     * @tparam N Maximum size of the ring buffer
     */
    template <typename T, int N>
    class RingBuffer {
        std::array<T, N> _buffer;
        int _read;
        int _write;

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
        inline bool is_full() { return _write == (_read + N - 1) % N; }

        /**
         * @brief Check if the buffer is empty
         *
         * @return true
         * @return false
         */
        inline bool is_empty() { return _write == _read; }

        /**
         * @brief Read a value from the buffer, advancing the read pointer
         *
         * @return T
         */
        inline T read() {
            DYN_ASSERT(!is_empty());
            T value = _buffer[_read];
            _read = (_read + 1) % N;
            return value;
        }

        /**
         * @brief Write a value into the buffer, advancing the write pointer
         *
         * @param value
         */
        inline void write(T value) {
            if (is_full()) return;
            _buffer[_write] = value;
            _write = (_write + 1) % N;
        }

        /**
         * @brief Pop a value from the buffer, shifting back the write pointer
         *
         */
        inline void pop() {
            if (is_empty()) return;
            _write = (_write + N - 1) % N;
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