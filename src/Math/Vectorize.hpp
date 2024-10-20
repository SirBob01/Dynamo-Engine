#pragma once

#if defined(__ARM_NEON)
#define DYNAMO_ARCH_NEON
#endif
#if (defined(__i386__) || defined(__x86_64__)) && defined(__AVX__)
#define DYNAMO_ARCH_AVX
#endif
#if (defined(__i386__) || defined(__x86_64__)) && defined(__SSE__)
#define DYNAMO_ARCH_SSE
#endif

#if defined(DYNAMO_ARCH_NEON)
#include <Math/Arch/Neon.hpp>
namespace arch = Dynamo::Vectorize::Neon;
#elif defined(DYNAMO_ARCH_AVX)
#include <Math/Arch/AVX.hpp>
namespace arch = Dynamo::Vectorize::AVX;
#elif defined(DYNAMO_ARCH_SSE)
#include <Math/Arch/SSE.hpp>
namespace arch = Dynamo::Vectorize::SSE;
#else
#include <Math/Arch/Scalar.hpp>
namespace arch = Dynamo::Vectorize::Scalar;
#endif

namespace Dynamo::Vectorize {
    /**
     * @brief dst[i] = src_a[i] * scalar
     *
     * @param src_a
     * @param scalar
     * @param dst
     * @param length
     */
    inline void smul(const float *src_a, const float scalar, float *dst, unsigned length) {
        arch::smul(src_a, scalar, dst, length);
    }

    /**
     * @brief dst[i] = src_a[i] + src_b[i]
     *
     * @param src_a
     * @param src_b
     * @param dst
     * @param length
     */
    inline void vadd(const float *src_a, const float *src_b, float *dst, unsigned length) {
        arch::vadd(src_a, src_b, dst, length);
    }

    /**
     * @brief dst[i] = src_a[i] - src_b[i]
     *
     * @param src_a
     * @param src_b
     * @param dst
     * @param length
     */
    inline void vsub(const float *src_a, const float *src_b, float *dst, unsigned length) {
        arch::vsub(src_a, src_b, dst, length);
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
    inline void vsma(const float *src, const float scalar, float *dst, unsigned length) {
        arch::vsma(src, scalar, dst, length);
    }

    /**
     * @brief dst[i] = min(hi, max(lo, src[i]))
     *
     * @param src
     * @param min
     * @param max
     * @param dst
     * @param length
     */
    inline void vclamp(const float *src, const float lo, const float hi, float *dst, unsigned length) {
        arch::vclamp(src, lo, hi, dst, length);
    }
} // namespace Dynamo::Vectorize