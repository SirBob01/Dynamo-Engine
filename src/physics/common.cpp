#include "common.h"

namespace Dynamo::Physics {
    float max(float a, float b) {
        if(a > b) {
            return a;
        }
        return b;
    }

    float min(float a, float b) {
        if(a < b) {
            return a;
        }
        return b;
    }

    float wrap_val(float v, float min, float max) {
        v = fmod(v - min, max - min) + min;
        return v;
    }
}