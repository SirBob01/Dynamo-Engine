#ifndef DYNAMO_UTIL_H_
#define DYNAMO_UTIL_H_

#include <algorithm>

namespace Dynamo {
    class Util {
    public:
        template <typename T, typename R>
        static T lerp(T v0, T v1, R t) {
            return (1 - t) * v0 + t * v1;
        }

        template <typename T>
        static T clamp(T x, T min, T max) {
            return std::min(max, std::max(x, min));
        }
    };
}

#endif