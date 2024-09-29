#pragma once

#include <arm_neon.h>

#include <Math/Arch/Scalar.hpp>

namespace Dynamo::Vectorize::Neon {
    inline void
    smul(const float *src, const float scalar, float *dst, unsigned length) {
        unsigned rem = length % 4;
        float *dst_end = dst + length - rem;
        while (dst < dst_end) {
            float32x4_t src_v = vld1q_f32(src);
            float32x4_t dst_v = vmulq_n_f32(src_v, scalar);
            vst1q_f32(dst, dst_v);
            src += 4;
            dst += 4;
        }
        Scalar::smul(src, scalar, dst, rem);
    }

    inline void
    vadd(const float *src_a, const float *src_b, float *dst, unsigned length) {
        unsigned rem = length % 4;
        float *dst_end = dst + length - rem;
        while (dst < dst_end) {
            float32x4_t src_a_v = vld1q_f32(src_a);
            float32x4_t src_b_v = vld1q_f32(src_b);
            float32x4_t dst_v = vaddq_f32(src_a_v, src_b_v);
            vst1q_f32(dst, dst_v);
            src_a += 4;
            src_b += 4;
            dst += 4;
        }
        Scalar::vadd(src_a, src_b, dst, rem);
    }

    inline void
    vsma(const float *src, const float scalar, float *dst, unsigned length) {
        float32x4_t scalar_v = vdupq_n_f32(scalar);
        unsigned rem = length % 4;
        float *dst_end = dst + length - rem;
        while (dst < dst_end) {
            float32x4_t src_v = vld1q_f32(src);
            float32x4_t dst_v = vld1q_f32(dst);
            dst_v = vmlaq_f32(dst_v, src_v, scalar_v);
            vst1q_f32(dst, dst_v);
            src += 4;
            dst += 4;
        }
        Scalar::vsma(src, scalar, dst, rem);
    }

    inline void vclamp(const float *src,
                       const float lo,
                       const float hi,
                       float *dst,
                       unsigned length) {
        float32x4_t lo_v = vdupq_n_f32(lo);
        float32x4_t hi_v = vdupq_n_f32(hi);
        unsigned rem = length % 4;
        float *dst_end = dst + length - rem;
        while (dst < dst_end) {
            float32x4_t src_v = vld1q_f32(src);
            float32x4_t dst_v = vmaxq_f32(lo_v, vminq_f32(hi_v, src_v));
            vst1q_f32(dst, dst_v);
            src += 4;
            dst += 4;
        }
        Scalar::vclamp(src, lo, hi, dst, rem);
    }
} // namespace Dynamo::Vectorize::Neon