#include "random.h"

namespace Dynamo::Random {
    void seed(int s) {
        std::srand(s);
    }

    float random() {
        return static_cast<float>(std::rand()) / RAND_MAX;
    }

    float range(float min, float max) {
        return min + ((max - min) * random());
    }
}
