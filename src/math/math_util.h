#ifndef MATH_UTIL
#define MATH_UTIL

#define _USE_MATH_DEFINES
#include <math.h>

#include "util/env_defines.h"

FORCE_INLINE float float_min(float a, float b) {
	return a < b ? a : b;
}

FORCE_INLINE float float_max(float a, float b) {
	return a > b ? a : b;
}

FORCE_INLINE float float_abs(float a) {
	return a >= 0.0f ? a : -a;
}

FORCE_INLINE float radians(float degrees) {
	return degrees * 0.01745329251994329576923690768489f;
}

FORCE_INLINE float degrees(float radians) {
	return radians * 57.295779513082320876798154814105f;
}

#endif // MATH_UTIL
