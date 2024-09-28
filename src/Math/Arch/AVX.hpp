#pragma once

#include <algorithm>
#include <immintrin.h>

namespace Dynamo::Vectorize::AVX {
    inline void
    smul(const float *src, const float scalar, float *dst, unsigned length) {
        unsigned i = 0;
        unsigned mul4 = length - (length % 8);
        __m256 scalar_v = _mm256_set1_ps(scalar);
        for (; i < mul4; i += 8) {
            __m256 a_v = _mm256_loadu_ps(src + i);
            _mm256_storeu_ps(dst + i, _mm256_mul_ps(a_v, scalar_v));
        }
        for (; i < length; i++) {
            dst[i] = src[i] * scalar;
        }
    }

    inline void
    vadd(const float *src_a, const float *src_b, float *dst, unsigned length) {
        unsigned i = 0;
        unsigned mul4 = length - (length % 8);
        for (; i < mul4; i += 8) {
            __m256 a_v = _mm256_loadu_ps(src_a + i);
            __m256 b_v = _mm256_loadu_ps(src_b + i);
            _mm256_storeu_ps(dst + i, _mm256_add_ps(a_v, b_v));
        }
        for (; i < length; i++) {
            dst[i] = src_a[i] + src_b[i];
        }
    }

    inline void
    vsma(const float *src, const float scalar, float *dst, unsigned length) {
        unsigned i = 0;
        unsigned mul4 = length - (length % 8);
        __m256 scalar_v = _mm256_set1_ps(scalar);
        for (; i < mul4; i += 8) {
            __m256 a_v = _mm256_loadu_ps(src + i);
            __m256 b_v = _mm256_loadu_ps(dst + i);
            __m256 c_v = _mm256_add_ps(_mm256_mul_ps(a_v, scalar_v), b_v);
            _mm256_storeu_ps(dst + i, c_v);
        }
        for (; i < length; i++) {
            dst[i] += src[i] * scalar;
        }
    }

    inline void vclip(const float *src,
                      const float lo,
                      const float hi,
                      float *dst,
                      unsigned length) {
        unsigned i = 0;
        unsigned mul4 = length - (length % 8);
        __m256 lo_v = _mm256_set1_ps(lo);
        __m256 hi_v = _mm256_set1_ps(hi);
        for (; i < mul4; i += 8) {
            __m256 a_v = _mm256_loadu_ps(src + i);
            _mm256_storeu_ps(dst + i,
                             _mm256_max_ps(lo_v, _mm256_min_ps(hi_v, a_v)));
        }
        for (; i < length; i++) {
            dst[i] = std::max(lo, std::min(hi, src[i]));
        }
    }
} // namespace Dynamo::Vectorize::AVX