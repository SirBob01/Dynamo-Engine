#pragma once

#include <array>

#include "../Types.hpp"

namespace Dynamo {
    /**
     * @brief A circular buffer such that writing will automatically loop back
     * at the start of the buffer as it overflows, discarding old data.
     *
     * The buffer keeps track of a read and write pointer.
     * The buffer is full if write is just behind read.
     * The buffer is empty if the read and write point to the same index.
     *
     * @tparam T Type of element, must be trivially copyable.
     * @tparam N Maximum size of the container (power of 2).
     */
    template <typename T, u32 N>
    class RingBuffer {
        static const u32 MASK = N - 1;
        static_assert(N > 0 && (N & MASK) == 0,
                      "RingBuffer size (> 0) should be a power of 2");
        static_assert(std::is_trivially_copyable<T>::value,
                      "RingBuffer element type must be trivially copyable");

        std::array<T, N> _buffer;
        u32 _read;
        u32 _write;

      public:
        /**
         * @brief Construct a new RingBuffer object
         *
         */
        constexpr RingBuffer() : _read(0), _write(0) {}

        /**
         * @brief Check if the buffer is full
         *
         * @return true
         * @return false
         */
        inline b8 full() const { return size() == N; }

        /**
         * @brief Check if the buffer is empty
         *
         * @return true
         * @return false
         */
        inline b8 empty() const { return size() == 0; }

        /**
         * @brief Get the size of the buffer
         *
         * @return u32
         */
        inline u32 size() const { return (_write - _read); }

        /**
         * @brief Get the number of writes that can still be performed
         *
         * @return u32
         */
        inline u32 remaining() const { return N - size(); }

        /**
         * @brief Read a value from the buffer, advancing the read pointer
         *
         * @return T
         */
        inline T read() {
            DYN_ASSERT(!empty());
            return _buffer[_read++ & MASK];
        }

        /**
         * @brief Write a value into the buffer, advancing the write pointer
         *
         * @param value
         */
        inline void write(T value) {
            DYN_ASSERT(!full());
            _buffer[_write++ & MASK] = value;
        }

        /**
         * @brief Read up to n values from the buffer, advancing the read
         * pointer
         *
         * This will return the number of elements read
         *
         * @param dst
         * @param n
         * @return u32
         */
        inline u32 read(T *dst, const u32 n) {
            // Compute copy partitions
            u32 offset = _read & MASK;
            u32 length = std::min(n, size());
            u32 l_length = std::min(length, N - offset);
            u32 r_length = length - l_length;

            T *src = _buffer.data();
            std::copy(src + offset, src + offset + l_length, dst);
            std::copy(src, src + r_length, dst + l_length);

            // Update read pointer
            _read += length;
            return length;
        }

        /**
         * @brief Write up to n values into the buffer, advancing the write
         * pointer
         *
         * This will return the number of elements written
         *
         * @param src
         * @param n
         * @return u32
         */
        inline u32 write(const T *src, const u32 n) {
            // Compute copy partitions
            u32 offset = _write & MASK;
            u32 length = std::min(n, remaining());
            u32 l_length = std::min(length, N - offset);

            T *dst = _buffer.data();
            std::copy(src, src + l_length, dst + offset);
            std::copy(src + l_length, src + length, dst);

            // Update write pointer
            _write += length;
            return length;
        }

        /**
         * @brief Pop a value from the buffer, shifting back the write pointer
         *
         */
        inline void pop() {
            DYN_ASSERT(!empty());
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