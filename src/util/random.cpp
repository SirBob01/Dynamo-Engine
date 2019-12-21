#include "random.h"

namespace Dynamo {
    void Random::seed(int s) {
        std::srand(s);
    }

    float Random::random() {
        return static_cast<float>(std::rand()) / RAND_MAX;
    }

    float Random::range(int min, int max) {
        return min + ((max - min) * random());
    }
}
