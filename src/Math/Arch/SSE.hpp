#pragma once

#include <immintrin.h>

#include <Math/Arch/Scalar.hpp>

namespace Dynamo::Vectorize::SSE {
    inline void
    smul(const float *src, const float scalar, float *dst, unsigned length) {
        __m128 scalar_v = _mm_set1_ps(scalar);
        unsigned rem = length % 4;
        float *dst_end = dst + length - rem;
        while (dst < dst_end) {
            __m128 src_v = _mm_loadu_ps(src);
            __m128 dst_v = _mm_mul_ps(src_v, scalar_v);
            _mm_storeu_ps(dst, dst_v);
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
            __m128 src_a_v = _mm_loadu_ps(src_a);
            __m128 src_b_v = _mm_loadu_ps(src_b);
            __m128 dst_v = _mm_add_ps(src_a_v, src_b_v);
            _mm_storeu_ps(dst, dst_v);
            src_a += 4;
            src_b += 4;
            dst += 4;
        }
        Scalar::vadd(src_a, src_b, dst, rem);
    }

    inline void
    vsub(const float *src_a, const float *src_b, float *dst, unsigned length) {
        unsigned rem = length % 4;
        float *dst_end = dst + length - rem;
        while (dst < dst_end) {
            __m128 src_a_v = _mm_loadu_ps(src_a);
            __m128 src_b_v = _mm_loadu_ps(src_b);
            __m128 dst_v = _mm_sub_ps(src_a_v, src_b_v);
            _mm_storeu_ps(dst, dst_v);
            src_a += 4;
            src_b += 4;
            dst += 4;
        }
        Scalar::vsub(src_a, src_b, dst, rem);
    }

    inline void
    vsma(const float *src, const float scalar, float *dst, unsigned length) {
        __m128 scalar_v = _mm_set1_ps(scalar);
        unsigned rem = length % 4;
        float *dst_end = dst + length - rem;
        while (dst < dst_end) {
            __m128 src_v = _mm_loadu_ps(src);
            __m128 dst_v = _mm_loadu_ps(dst);
            dst_v = _mm_add_ps(_mm_mul_ps(src_v, scalar_v), dst_v);
            _mm_storeu_ps(dst, dst_v);
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
        __m128 lo_v = _mm_set1_ps(lo);
        __m128 hi_v = _mm_set1_ps(hi);
        unsigned rem = length % 4;
        float *dst_end = dst + length - rem;
        while (dst < dst_end) {
            __m128 src_v = _mm_loadu_ps(src);
            __m128 dst_v = _mm_max_ps(lo_v, _mm_min_ps(hi_v, src_v));
            _mm_storeu_ps(dst, dst_v);
            src += 4;
            dst += 4;
        }
        Scalar::vclamp(src, lo, hi, dst, rem);
    }
} // namespace Dynamo::Vectorize::SSE