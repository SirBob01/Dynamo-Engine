#include "RingBuffer.hpp"

namespace Dynamo {
    RingBuffer::RingBuffer(int size) {
        _size = size;
        _buffer = new char[_size];
        for (int i = 0; i < _size; i++) {
            _buffer[i] = 0;
        }

        _read = 0;
        _write = 0;
    }

    RingBuffer::~RingBuffer() { delete[] _buffer; }

    bool RingBuffer::is_full() { return _write == (_read + _size - 1) % _size; }

    bool RingBuffer::is_empty() { return _write == _read; }

    char RingBuffer::read() {
        if (is_empty()) {
            return 0;
        }
        char byte = _buffer[_read];
        _read = (_read + 1) % _size;
        return byte;
    }

    void RingBuffer::write(char byte) {
        if (is_full()) {
            return;
        }
        _buffer[_write] = byte;
        _write = (_write + 1) % _size;
    }

    void RingBuffer::pop() {
        if (is_empty()) {
            return;
        }
        _write = (_write + _size - 1) % _size;
    }

    void RingBuffer::clear() {
        _read = 0;
        _write = 0;
    }
} // namespace Dynamo