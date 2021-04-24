#include "transform.h"

#include "math/mat4.h"
#include "math/quat.h"
#include "math/math_util.h"

void calculateRotation(struct Transform* transform, struct mat4* rotation) {
	float half_angle = radians(transform->rotationAngle) * 0.5f;
	float sin_half_angle = (float)sin(half_angle);
	float cos_half_angle = (float)cos(half_angle);
	struct quat q;
	quat_init(&q, transform->rotationAxis.x * sin_half_angle,
		transform->rotationAxis.y * sin_half_angle, transform->rotationAxis.z * sin_half_angle, cos_half_angle);
	quat_normalize(&q);

	rotation->m[0] = 1.0f - 2.0f * q.y * q.y - 2.0f * q.z * q.z;
	rotation->m[1] = 2.0f * q.x * q.y - 2.0f * q.z * q.w;
	rotation->m[2] = 2.0f * q.x * q.z + 2.0f * q.y * q.w;
	rotation->m[4] = 2.0f * q.x * q.y + 2.0f * q.z * q.w;
	rotation->m[5] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.z * q.z;
	rotation->m[6] = 2.0f * q.y * q.z - 2.0f * q.x * q.w;
	rotation->m[8] = 2.0f * q.x * q.z - 2.0f * q.y * q.w;
	rotation->m[9] = 2.0f * q.y * q.z + 2.0f * q.x * q.w;
	rotation->m[10] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.y * q.y;
}

void transformInit(struct Transform* t) {
	vec3_init(&t->position, 0.0f, 0.0f, 0.0f);
	vec3_init(&t->scale, 1.0, 1.0f, 1.0f);
	vec3_init(&t->rotationAxis, 0.0f, 1.0f, 0.0f);
	vec3_init(&t->rotationPoint, 0.0f, 0.0f, 0.0f);
	t->rotationAngle = 0.0f;
}

void calculateTransform(struct mat4* mat, struct Transform* transform) {
	struct mat4 scale, rotation, translation;
	mat4_identity(&scale);
	mat4_identity(&rotation);
	mat4_identity(&translation);
	scale.m[0] = transform->scale.x;
	scale.m[5] = transform->scale.y;
	scale.m[10] = transform->scale.z;

	calculateRotation(transform, &rotation);

	translation.m[12] = transform->position.x;
	translation.m[13] = transform->position.y;
	translation.m[14] = transform->position.z;

	mat4_mul(mat, &rotation, &scale);
	mat4_mul(mat, &translation, mat);
}
