#ifndef DYNAMO_UTIL_H_
#define DYNAMO_UTIL_H_

#include <algorithm>

namespace Dynamo::Util {
    template <typename T, typename R>
    T lerp(T v0, T v1, R t) {
        return (1 - t) * v0 + t * v1;
    }

    template <typename T>
    T clamp(T x, T min, T max) {
        return std::min(max, std::max(x, min));
    }
}

#endif