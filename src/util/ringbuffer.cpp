#include "ringbuffer.h"

namespace Dynamo {
    RingBuffer::RingBuffer(int size) {
        size_ = size;
        buffer_ = new char[size_];
        for(int i = 0; i < size_; i++) {
            buffer_[i] = 0;
        }

        read_ = 0;
        write_ = 0;
    }

    RingBuffer::~RingBuffer() {
        delete[] buffer_;
    }

    bool RingBuffer::is_full() {
        return write_ == (read_ + size_ - 1) % size_;
    }

    bool RingBuffer::is_empty() {
        return write_ == read_;
    }

    char RingBuffer::read() {
        if(is_empty()) {
            return 0;
        }
        char byte = buffer_[read_];
        read_ = (read_ + 1) % size_;
        return byte;
    }

    void RingBuffer::write(char byte) {
        if(is_full()) {
            return;
        }
        buffer_[write_] = byte;
        write_ = (write_ + 1) % size_;
    }

    void RingBuffer::pop() {
        if(is_empty()) {
            return;
        }
        write_ = (write_ + size_ - 1) % size_;
    }

    void RingBuffer::clear() {
        read_ = 0;
        write_ = 0;
    }
}