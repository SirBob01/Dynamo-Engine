#ifndef DYNAMO_PHYSICS_COMMON_H_
#define DYNAMO_PHYSICS_COMMON_H_
#define PI 3.141592653589793

#include <math.h>

namespace Dynamo::Physics {
    float max(float a, float b);

    float min(float a, float b);

    float wrap_val(float v, float min, float max);
}

#endif