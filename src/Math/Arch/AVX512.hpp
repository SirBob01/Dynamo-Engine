#pragma once

#include <immintrin.h>

#include <Math/Arch/Scalar.hpp>

namespace Dynamo::Vectorize::AVX512 {
    inline void
    smul(const float *src, const float scalar, float *dst, unsigned length) {
        __m512 scalar_v = _mm512_set1_ps(scalar);
        unsigned rem = length % 16;
        float *dst_end = dst + length - rem;
        while (dst < dst_end) {
            __m512 src_v = _mm512_loadu_ps(src);
            __m512 dst_v = _mm512_mul_ps(src_v, scalar_v);
            _mm512_storeu_ps(dst, dst_v);
            src += 16;
            dst += 16;
        }
        Scalar::smul(src, scalar, dst, rem);
    }

    inline void
    vadd(const float *src_a, const float *src_b, float *dst, unsigned length) {
        unsigned rem = length % 16;
        float *dst_end = dst + length - rem;
        while (dst < dst_end) {
            __m512 src_a_v = _mm512_loadu_ps(src_a);
            __m512 src_b_v = _mm512_loadu_ps(src_b);
            __m512 dst_v = _mm512_add_ps(src_a_v, src_b_v);
            _mm512_storeu_ps(dst, dst_v);
            src_a += 16;
            src_b += 16;
            dst += 16;
        }
        Scalar::vadd(src_a, src_b, dst, rem);
    }

    inline void
    vsma(const float *src, const float scalar, float *dst, unsigned length) {
        __m512 scalar_v = _mm512_set1_ps(scalar);
        unsigned rem = length % 16;
        float *dst_end = dst + length - rem;
        while (dst < dst_end) {
            __m512 src_v = _mm512_loadu_ps(src);
            __m512 dst_v = _mm512_loadu_ps(dst);
            dst_v = _mm512_fmadd_ps(src_v, scalar_v, dst_v);
            _mm512_storeu_ps(dst, dst_v);
            src += 16;
            dst += 16;
        }
        Scalar::vsma(src, scalar, dst, rem);
    }

    inline void vclamp(const float *src,
                       const float lo,
                       const float hi,
                       float *dst,
                       unsigned length) {
        __m512 lo_v = _mm512_set1_ps(lo);
        __m512 hi_v = _mm512_set1_ps(hi);
        unsigned rem = length % 16;
        float *dst_end = dst + length - rem;
        while (dst < dst_end) {
            __m512 src_v = _mm512_loadu_ps(src);
            __m512 dst_v = _mm512_max_ps(lo_v, _mm512_min_ps(hi_v, src_v));
            _mm512_storeu_ps(dst, dst_v);
            src += 16;
            dst += 16;
        }
        Scalar::vclamp(src, lo, hi, dst, rem);
    }
} // namespace Dynamo::Vectorize::AVX512