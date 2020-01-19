#include "sparse.h"

namespace Dynamo {
    Sparse::Sparse() {
        length_ = 0;
        max_capacity_ = 2;
        max_value_ = 4;

        dense_ = static_cast<uint32_t *>(
            std::malloc(sizeof(uint32_t) * max_capacity_)
        );
        sparse_ = static_cast<uint32_t *>(
            std::malloc(sizeof(uint32_t) * max_value_)
        );
    }

    Sparse::~Sparse() {
        std::free(dense_);
        std::free(sparse_);
    }

    int Sparse::get_length() {
        return length_;
    }

    int Sparse::get_at(int index) {
        if(index < 0 || index >= length_) {
            return -1;
        }
        return dense_[index];
    }

    int Sparse::search(uint32_t x) {
        if(x > max_value_) {
            return -1;
        }
        if(sparse_[x] < length_ && dense_[sparse_[x]] == x) {
            return sparse_[x];
        }
        return -1;
    }

    void Sparse::insert(uint32_t x) {
        if(search(x) != -1) {
            return;
        }
        if(length_+1 >= max_capacity_) {
            max_capacity_ *= 2;
            dense_ = static_cast<uint32_t *>(
                std::realloc(dense_, max_capacity_ * sizeof(uint32_t))
            );
        }
        if(x >= max_value_) {
            max_value_ = x + 1;
            sparse_ = static_cast<uint32_t *>(
                std::realloc(sparse_, max_value_ * sizeof(uint32_t))
            );
        }

        dense_[length_] = x;
        sparse_[x] = length_;
        length_++;
    }

    void Sparse::remove(uint32_t x) {
        int index = search(x);
        if(index < 0) {
            return;
        }

        uint32_t temp = dense_[length_-1];
        dense_[index] = temp;
        sparse_[temp] = sparse_[x];
        length_--;
    }

    void Sparse::clear() {
        length_ = 0;
    }
}