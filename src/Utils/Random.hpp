#pragma once

#include <random>

namespace Dynamo::Random {
    static std::default_random_engine rng;
    static std::uniform_real_distribution<float> uniform(0, 1);

    /**
     * @brief Seed the random number generator.
     *
     * @param s Seed value.
     */
    inline void seed(long long s) { rng.seed(s); }

    /**
     * @brief Get a random number in the range [0, 1).
     *
     * @return float
     */
    inline float random() { return uniform(rng); }

    /**
     * @brief Get a random number in the range [min, max).
     *
     * @param min Minimum value inclusive.
     * @param max Maximum value exclusive.
     * @return float
     */
    inline float range(float min, float max) {
        return min + ((max - min) * random());
    }
} // namespace Dynamo::Random
