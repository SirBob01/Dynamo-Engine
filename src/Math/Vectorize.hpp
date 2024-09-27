#pragma once

#ifdef __ARM_NEON
#include <Math/Arch/Neon.hpp>
namespace arch = Dynamo::Vectorize::Neon;
#else
#include <Math/Arch/Scalar.hpp>
namespace arch = Dynamo::Vectorize::Scalar;
#endif

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
    inline void
    vadd(const float *src_a, const float *src_b, float *dst, unsigned length) {
        arch::vadd(src_a, src_b, dst, length);
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
        arch::vsma(src, scalar, dst, length);
    }
} // namespace Dynamo::Vectorize