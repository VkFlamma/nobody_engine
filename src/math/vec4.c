#include "math/vec4.h"

#include <math.h>

void vec4_init(struct vec4* v, float x, float y, float z, float w) {
	v->x = x;
	v->y = y;
	v->z = z;
	v->w = w;
}

float vec4_length(struct vec4* v) {
	return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w);
}
