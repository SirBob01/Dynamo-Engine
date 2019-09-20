#include "common.h"

float Physics_max(float a, float b) {
	if(a > b) {
		return a;
	}
	return b;
}

float Physics_abs(float v) {
	if(v < 0) {
		v = -v;
	}
	return v;
}

float Physics_min(float a, float b) {
	if(a < b) {
		return a;
	}
	return b;
}

float Physics_wrap_val(float v, float min, float max) {
	v = fmod(v - min, max - min) + min;
	return v;
}