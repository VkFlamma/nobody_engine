#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "math/vec3.h"

struct Transform {
	struct vec3 position;
	struct vec3 scale;
	struct vec3 rotationAxis;
	struct vec3 rotationPoint;
	float rotationAngle;
};

void transformInit(struct Transform* t);
void calculateTransform(struct mat4* mat, struct Transform* transform);

#endif // TRANSFORM_H
