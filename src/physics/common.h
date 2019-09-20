#ifndef GAME_PHYSICS_COMMON
#define GAME_PHYSICS_COMMON
#define PI 3.141592653589793

#include <math.h>

float Physics_max(float a, float b);

float Physics_min(float a, float b);

float Physics_abs(float v);

float Physics_wrap_val(float v, float min, float max);

#endif