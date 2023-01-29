#pragma once

#include <chrono>
#include <functional>
#include <random>

#include "../Types.hpp"

namespace Dynamo::Random {
    static std::default_random_engine rng;
    static std::uniform_real_distribution<f32> uniform(0, 1);

    /**
     * @brief Seed the random number generator
     *
     * @param s Seed value
     */
    inline void seed(i32 s) { rng.seed(s); }

    /**
     * @brief Get a random number in the range [0, 1)
     *
     * @return f32
     */
    inline f32 random() { return uniform(rng); }

    /**
     * @brief Get a random number in the range [min, max)
     *
     * @param min
     * @param max
     * @return f32
     */
    inline f32 range(f32 min, f32 max) {
        return min + ((max - min) * random());
    }
} // namespace Dynamo::Random
