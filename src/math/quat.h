#ifndef QUAT_H
#define QUAT_H

#include "math/mat4.h"

struct quat {
	float x;
	float y;
	float z;
	float w;
};

void quat_init(struct quat* q, float x, float y, float z, float w);

float quat_length(struct quat* d);

void quat_normalize(struct quat* q);

void quat_mul(struct quat* d, struct quat* l, struct quat* r);

void quat_get_matrix(struct quat* q, struct mat4* m);

#endif // QUAT_H
