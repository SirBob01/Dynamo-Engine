#pragma once
#include <arm_neon.h>

namespace Dynamo::Vectorize::Neon {
    inline void
    smul(const float *src, const float scalar, float *dst, unsigned length) {
        unsigned i = 0;
        unsigned mul4 = length - (length % 4);
        for (; i < mul4; i += 4) {
            float32x4_t a_v = vld1q_f32(src + i);
            vst1q_f32(dst + i, vmulq_n_f32(a_v, scalar));
        }
        for (; i < length; i++) {
            dst[i] = src[i] * scalar;
        }
    }

    inline void
    vadd(const float *src_a, const float *src_b, float *dst, unsigned length) {
        unsigned i = 0;
        unsigned mul4 = length - (length % 4);
        for (; i < mul4; i += 4) {
            float32x4_t a_v = vld1q_f32(src_a + i);
            float32x4_t b_v = vld1q_f32(src_b + i);
            vst1q_f32(dst + i, vaddq_f32(a_v, b_v));
        }
        for (; i < length; i++) {
            dst[i] = src_a[i] + src_b[i];
        }
    }

    inline void
    vsma(const float *src, const float scalar, float *dst, unsigned length) {
        unsigned i = 0;
        unsigned mul4 = length - (length % 4);
        float32x4_t scalar_v = vdupq_n_f32(scalar);
        for (; i < mul4; i += 4) {
            float32x4_t a_v = vld1q_f32(src + i);
            float32x4_t b_v = vld1q_f32(dst + i);
            float32x4_t c_v = vmlaq_f32(b_v, a_v, scalar_v);
            vst1q_f32(dst + i, c_v);
        }
        for (; i < length; i++) {
            dst[i] += src[i] * scalar;
        }
    }
} // namespace Dynamo::Vectorize::Neon