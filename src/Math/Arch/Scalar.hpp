#pragma once

#include <algorithm>

namespace Dynamo::Vectorize::Scalar {
    inline void
    smul(const float *src_a, const float scalar, float *dst, unsigned length) {
        for (unsigned i = 0; i < length; i++) {
            dst[i] = src_a[i] * scalar;
        }
    }

    inline void
    vadd(const float *src_a, const float *src_b, float *dst, unsigned length) {
        for (unsigned i = 0; i < length; i++) {
            dst[i] = src_a[i] + src_b[i];
        }
    }

    inline void
    vsma(const float *src, const float scalar, float *dst, unsigned length) {
        for (unsigned i = 0; i < length; i++) {
            dst[i] += src[i] * scalar;
        }
    }

    inline void vclamp(const float *src,
                       const float lo,
                       const float hi,
                       float *dst,
                       unsigned length) {
        for (unsigned i = 0; i < length; i++) {
            dst[i] = std::clamp(src[i], lo, hi);
        }
    }
} // namespace Dynamo::Vectorize::Scalar