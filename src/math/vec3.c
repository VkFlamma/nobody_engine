#include "math/vec3.h"

#include <math.h>

void vec3_init(struct vec3* d, float x, float y, float z) {
	d->x = x;
	d->y = y;
	d->z = z;
}

float vec3_dot_self(const struct vec3* a) {
	return a->x * a->x + a->y * a->y + a->z * a->z;
}

float vec3_dot(const struct vec3* a, const struct vec3* b) {
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

float vec3_norm(const struct vec3* d) {
	return sqrtf(vec3_dot_self(d));
}

void vec3_normalize(struct vec3* d) {
	float norm = vec3_norm(d);
	d->x /= norm;
	d->y /= norm;
	d->z /= norm;
}

void vec3_add(struct vec3* d, const struct vec3* a, const struct vec3* b) {
	d->x = a->x + b->x;
	d->y = a->y + b->y;
	d->z = a->z + b->z;
}

void vec3_sub(struct vec3* d, const struct vec3* a, const struct vec3* b) {
	d->x = a->x - b->x;
	d->y = a->y - b->y;
	d->z = a->z - b->z;
}

void vec3_cross(struct vec3* d, const struct vec3* a, const struct vec3* b) {
	d->x = a->y * b->z - a->z * b->y;
	d->y = a->z * b->x - a->x * b->z;
	d->z = a->x * b->y - a->y * b->x;
}
