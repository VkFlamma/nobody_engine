#include "math/quat.h"

#include <math.h>
#include <assert.h>

void quat_init(struct quat* q, float x, float y, float z, float w) {
	q->x = x;
	q->y = y;
	q->z = z;
	q->w = w;
}

float quat_dot_self(struct quat* q) {
	return q->x * q->x + q->y * q->y + q->z * q->z + q->w * q->w;
}

float quat_length(struct quat* d) {
	return sqrtf(quat_dot_self(d));
}

void quat_normalize(struct quat* q) {
	float norm = quat_length(q);
	q->x /= norm;
	q->y /= norm;
	q->z /= norm;
	q->w /= norm;
}

void quat_mul(struct quat* d, struct quat* l, struct quat* r) {
	assert(d != l && d != r);
	d->x = l->w * r->x + l->x * r->w + l->y * r->z - l->z * r->y;
	d->y = l->w * r->y - l->x * r->z + l->y * r->w + l->z * r->x;
	d->z = l->w * r->z + l->x * r->y - l->y * r->x + l->z * r->w;
	d->w = l->w * r->w - l->x * r->x - l->y * r->y - l->z * r->z;
}

void quat_get_matrix(struct quat* q, struct mat4* m) {
	m->m[0] = 1.0f - 2.0f * q->y * q->y - 2.0f * q->z * q->z;
	m->m[1] = 2.0f * q->x * q->y - 2.0f * q->z * q->w;
	m->m[2] = 2.0f * q->x * q->z + 2.0f * q->y * q->w;
	m->m[3] = 0.0f;
	m->m[4] = 2.0f * q->x * q->y + 2.0f * q->z * q->w;
	m->m[5] = 1.0f - 2.0f * q->x * q->x - 2.0f * q->z * q->z;
	m->m[6] = 2.0f * q->y * q->z - 2.0f * q->x * q->w;
	m->m[7] = 0.0f;
	m->m[8] = 2.0f * q->x * q->z - 2.0f * q->y * q->w;
	m->m[9] = 2.0f * q->y * q->z + 2.0f * q->x * q->w;
	m->m[10] = 1.0f - 2.0f * q->x * q->x - 2.0f * q->y * q->y;
	m->m[11] = 0.0f;
	m->m[12] = 0.0f;
	m->m[13] = 0.0f;
	m->m[14] = 0.0f;
	m->m[15] = 1.0f;
}
