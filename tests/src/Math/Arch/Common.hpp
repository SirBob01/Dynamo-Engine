#pragma once

#include <array>

constexpr unsigned LENGTH = 123435;
using FloatArray = std::array<float, LENGTH>;

inline void fill_array(FloatArray &arr) {
    for (unsigned i = 0; i < LENGTH; i++) {
        arr[i] = i;
    }
}
