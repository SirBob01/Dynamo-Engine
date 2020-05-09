#ifndef DYNAMO_RANDOM_H_
#define DYNAMO_RANDOM_H_

#include <cstdlib>

namespace Dynamo::Random {
    // Seed the random number generator
    void seed(int s);

    // Get a random number between 0 and 1
    float random();

    // Get a random number between min and max
    float range(float min, float max);
}

#endif