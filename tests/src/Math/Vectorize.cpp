#include <Dynamo.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Vectorize Instruction Set", "[Vectorize]") {
#if defined(DYNAMO_ARCH_NEON)
    Dynamo::Log::info("Vectorize Neon");
#elif defined(DYNAMO_ARCH_AVX)
    Dynamo::Log::info("Vectorize AVX");
#elif defined(DYNAMO_ARCH_SSE)
    Dynamo::Log::info("Vectorize SSE");
#else
    Dynamo::Log::info("Vectorize SIMD not available");
#endif
}
