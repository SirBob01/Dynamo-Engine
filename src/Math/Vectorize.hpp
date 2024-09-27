#pragma once

// TODO: Implement utilies using intrinsics or std::experimental::simd

namespace Dynamo::Vectorize {
    /**
     * @brief dst[i] = src_a[i] + scalar
     *
     * @param src_a
     * @param scalar
     * @param dst
     * @param length
     */
    inline void
    smul(const float *src_a, const float scalar, float *dst, unsigned length) {
        for (unsigned i = 0; i < length; i++) {
            dst[i] = src_a[i] * scalar;
        }
    }

    /**
     * @brief dst[i] = src_a[i] + src_b[i]
     *
     * @param src_a
     * @param src_b
     * @param dst
     * @param length
     */
    inline void
    vadd(const float *src_a, const float *src_b, float *dst, unsigned length) {
        for (unsigned i = 0; i < length; i++) {
            dst[i] = src_a[i] + src_b[i];
        }
    }

    /**
     * @brief dst[i] += src_a[i] * scalar
     *
     * @param src_a
     * @param src_b
     * @param scalar
     * @param dst
     * @param length
     */
    inline void
    vsma(const float *src, const float scalar, float *dst, unsigned length) {
        for (unsigned i = 0; i < length; i++) {
            dst[i] += src[i] * scalar;
        }
    }
} // namespace Dynamo::Vectorize