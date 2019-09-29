#include "common.h"

float Physics::max(float a, float b) {
	if(a > b) {
		return a;
	}
	return b;
}

float Physics::min(float a, float b) {
	if(a < b) {
		return a;
	}
	return b;
}

float Physics::wrap_val(float v, float min, float max) {
	v = fmod(v - min, max - min) + min;
	return v;
}