#pragma once

namespace Dynamo {
    class RingBuffer {
        char *_buffer;

        int _size;
        int _read, _write;

      public:
        /**
         * @brief Construct a new RingBuffer object of a certain size
         *
         * @param size
         */
        RingBuffer(int size);
        ~RingBuffer();

        /**
         * @brief Check if the buffer is full
         *
         * @return true
         * @return false
         */
        bool is_full();

        /**
         * @brief Check if the buffer is empty
         *
         * @return true
         * @return false
         */
        bool is_empty();

        /**
         * @brief Read a byte from the buffer
         *
         * @return char
         */
        char read();

        /**
         * @brief Write a byte into the buffer
         *
         * @param byte
         */
        void write(char byte);

        /**
         * @brief Pop a byte from the buffer
         *
         */
        void pop();

        /**
         * @brief Empty the buffer
         *
         */
        void clear();
    };
} // namespace Dynamo