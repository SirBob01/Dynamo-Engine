#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

namespace Dynamo {
    class RingBuffer {
        char *buffer_;

        int size_;
        int read_, write_;

    public:
        RingBuffer(int size);
        ~RingBuffer();

        // Check if the buffer is full
        bool is_full();

        // Check if the buffer is empty
        bool is_empty();

        // Read a byte from the buffer
        char read();

        // Write a byte into the buffer
        void write(char byte);

        // Remove a byte from the buffer
        void pop();
    };
}

#endif