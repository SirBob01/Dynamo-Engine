#pragma once

#include <arm_neon.h>

#include <Math/Arch/Scalar.hpp>

namespace Dynamo::Vectorize::Neon {
    inline void smul(const float *src, const float scalar, float *dst, unsigned length) {
        float32x4_t scalar_v = vdupq_n_f32(scalar);
        unsigned rem = length % 16;
        float *dst_end = dst + length - rem;
        while (dst < dst_end) {
            float32x4x4_t src_m = vld1q_f32_x4(src);
            float32x4x4_t dst_m = {
                vmulq_f32(src_m.val[0], scalar_v),
                vmulq_f32(src_m.val[1], scalar_v),
                vmulq_f32(src_m.val[2], scalar_v),
                vmulq_f32(src_m.val[3], scalar_v),
            };
            vst1q_f32_x4(dst, dst_m);

            src += 16;
            dst += 16;
        }

        // Multiples of 4
        unsigned rem_1 = rem % 4;
        float *dst_end_1 = dst + rem - rem_1;
        while (dst < dst_end_1) {
            float32x4_t src_v = vld1q_f32(src);
            float32x4_t dst_v = vmulq_f32(src_v, scalar_v);
            vst1q_f32(dst, dst_v);

            src += 4;
            dst += 4;
        }

        Scalar::smul(src, scalar, dst, rem_1);
    }

    inline void vadd(const float *src_a, const float *src_b, float *dst, unsigned length) {
        unsigned rem = length % 16;
        float *dst_end = dst + length - rem;
        while (dst < dst_end) {
            float32x4x4_t src_a_m = vld1q_f32_x4(src_a);
            float32x4x4_t src_b_m = vld1q_f32_x4(src_b);
            float32x4x4_t dst_m = {
                vaddq_f32(src_a_m.val[0], src_b_m.val[0]),
                vaddq_f32(src_a_m.val[1], src_b_m.val[1]),
                vaddq_f32(src_a_m.val[2], src_b_m.val[2]),
                vaddq_f32(src_a_m.val[3], src_b_m.val[3]),
            };
            vst1q_f32_x4(dst, dst_m);

            src_a += 16;
            src_b += 16;
            dst += 16;
        }

        // Multiples of 4
        unsigned rem_1 = rem % 4;
        float *dst_end_1 = dst + rem - rem_1;
        while (dst < dst_end_1) {
            float32x4_t src_a_v = vld1q_f32(src_a);
            float32x4_t src_b_v = vld1q_f32(src_b);
            float32x4_t dst_v = vaddq_f32(src_a_v, src_b_v);
            vst1q_f32(dst, dst_v);

            src_a += 4;
            src_b += 4;
            dst += 4;
        }

        Scalar::vadd(src_a, src_b, dst, rem_1);
    }

    inline void vsub(const float *src_a, const float *src_b, float *dst, unsigned length) {
        unsigned rem = length % 16;
        float *dst_end = dst + length - rem;
        while (dst < dst_end) {
            float32x4x4_t src_a_m = vld1q_f32_x4(src_a);
            float32x4x4_t src_b_m = vld1q_f32_x4(src_b);
            float32x4x4_t dst_m = {
                vsubq_f32(src_a_m.val[0], src_b_m.val[0]),
                vsubq_f32(src_a_m.val[1], src_b_m.val[1]),
                vsubq_f32(src_a_m.val[2], src_b_m.val[2]),
                vsubq_f32(src_a_m.val[3], src_b_m.val[3]),
            };
            vst1q_f32_x4(dst, dst_m);

            src_a += 16;
            src_b += 16;
            dst += 16;
        }

        // Multiples of 4
        unsigned rem_1 = rem % 4;
        float *dst_end_1 = dst + rem - rem_1;
        while (dst < dst_end_1) {
            float32x4_t src_a_v = vld1q_f32(src_a);
            float32x4_t src_b_v = vld1q_f32(src_b);
            float32x4_t dst_v = vsubq_f32(src_a_v, src_b_v);
            vst1q_f32(dst, dst_v);

            src_a += 4;
            src_b += 4;
            dst += 4;
        }

        Scalar::vsub(src_a, src_b, dst, rem_1);
    }

    inline void vsma(const float *src, const float scalar, float *dst, unsigned length) {
        float32x4_t scalar_v = vdupq_n_f32(scalar);
        unsigned rem = length % 16;
        float *dst_end = dst + length - rem;
        while (dst < dst_end) {
            float32x4x4_t src_m = vld1q_f32_x4(src);
            float32x4x4_t dst_m = vld1q_f32_x4(dst);
            dst_m = {
                vmlaq_f32(dst_m.val[0], src_m.val[0], scalar_v),
                vmlaq_f32(dst_m.val[1], src_m.val[1], scalar_v),
                vmlaq_f32(dst_m.val[2], src_m.val[2], scalar_v),
                vmlaq_f32(dst_m.val[3], src_m.val[3], scalar_v),
            };
            vst1q_f32_x4(dst, dst_m);

            src += 16;
            dst += 16;
        }

        // Multiples of 4
        unsigned rem_1 = rem % 4;
        float *dst_end_1 = dst + rem - rem_1;
        while (dst < dst_end_1) {
            float32x4_t src_v = vld1q_f32(src);
            float32x4_t dst_v = vld1q_f32(dst);
            dst_v = vmlaq_f32(dst_v, src_v, scalar_v);
            vst1q_f32(dst, dst_v);

            src += 4;
            dst += 4;
        }

        Scalar::vsma(src, scalar, dst, rem_1);
    }

    inline void vclamp(const float *src, const float lo, const float hi, float *dst, unsigned length) {
        float32x4_t lo_v = vdupq_n_f32(lo);
        float32x4_t hi_v = vdupq_n_f32(hi);
        unsigned rem = length % 16;
        float *dst_end = dst + length - rem;
        while (dst < dst_end) {
            float32x4x4_t src_m = vld1q_f32_x4(src);
            float32x4x4_t dst_m = {
                vmaxq_f32(lo_v, vminq_f32(hi_v, src_m.val[0])),
                vmaxq_f32(lo_v, vminq_f32(hi_v, src_m.val[1])),
                vmaxq_f32(lo_v, vminq_f32(hi_v, src_m.val[2])),
                vmaxq_f32(lo_v, vminq_f32(hi_v, src_m.val[3])),
            };
            vst1q_f32_x4(dst, dst_m);

            src += 16;
            dst += 16;
        }

        // Multiples of 4
        unsigned rem_1 = rem % 4;
        float *dst_end_1 = dst + rem - rem_1;
        while (dst < dst_end_1) {
            float32x4_t src_v = vld1q_f32(src);
            float32x4_t dst_v = vmaxq_f32(lo_v, vminq_f32(hi_v, src_v));
            vst1q_f32(dst, dst_v);

            src += 4;
            dst += 4;
        }

        Scalar::vclamp(src, lo, hi, dst, rem_1);
    }
} // namespace Dynamo::Vectorize::Neon