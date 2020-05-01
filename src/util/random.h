#ifndef DYNAMO_RANDOM_H_
#define DYNAMO_RANDOM_H_

#include <cstdlib>

namespace Dynamo {
    class Random {
    public:
        // Seed the random number generator
        static void seed(int s);

        // Get a random number between 0 and 1
        static float random();

        // Get a random number between min and max
        static float range(float min, float max);
    };
}

#endif