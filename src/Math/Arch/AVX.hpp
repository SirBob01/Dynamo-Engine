#pragma once

#include <immintrin.h>

#include <Math/Arch/Scalar.hpp>

namespace Dynamo::Vectorize::AVX {
    inline void
    smul(const float *src, const float scalar, float *dst, unsigned length) {
        __m256 scalar_v = _mm256_set1_ps(scalar);
        unsigned rem = length % 8;
        float *dst_end = dst + length - rem;
        while (dst < dst_end) {
            __m256 src_v = _mm256_loadu_ps(src);
            __m256 dst_v = _mm256_mul_ps(src_v, scalar_v);
            _mm256_storeu_ps(dst, dst_v);
            src += 8;
            dst += 8;
        }
        Scalar::smul(src, scalar, dst, rem);
    }

    inline void
    vadd(const float *src_a, const float *src_b, float *dst, unsigned length) {
        unsigned rem = length % 8;
        float *dst_end = dst + length - rem;
        while (dst < dst_end) {
            __m256 src_a_v = _mm256_loadu_ps(src_a);
            __m256 src_b_v = _mm256_loadu_ps(src_b);
            __m256 dst_v = _mm256_add_ps(src_a_v, src_b_v);
            _mm256_storeu_ps(dst, dst_v);
            src_a += 8;
            src_b += 8;
            dst += 8;
        }
        Scalar::vadd(src_a, src_b, dst, rem);
    }

    inline void
    vsma(const float *src, const float scalar, float *dst, unsigned length) {
        __m256 scalar_v = _mm256_set1_ps(scalar);
        unsigned rem = length % 8;
        float *dst_end = dst + length - rem;
        while (dst < dst_end) {
            __m256 src_v = _mm256_loadu_ps(src);
            __m256 dst_v = _mm256_loadu_ps(dst);
            dst_v = _mm256_fmadd_ps(src_v, scalar_v, dst_v);
            _mm256_storeu_ps(dst, dst_v);
            src += 8;
            dst += 8;
        }
        Scalar::vsma(src, scalar, dst, rem);
    }

    inline void vclamp(const float *src,
                       const float lo,
                       const float hi,
                       float *dst,
                       unsigned length) {
        __m256 lo_v = _mm256_set1_ps(lo);
        __m256 hi_v = _mm256_set1_ps(hi);
        unsigned rem = length % 8;
        float *dst_end = dst + length - rem;
        while (dst < dst_end) {
            __m256 src_v = _mm256_loadu_ps(src);
            __m256 dst_v = _mm256_max_ps(lo_v, _mm256_min_ps(hi_v, src_v));
            _mm256_storeu_ps(dst, dst_v);
            src += 8;
            dst += 8;
        }
        Scalar::vclamp(src, lo, hi, dst, rem);
    }
} // namespace Dynamo::Vectorize::AVX