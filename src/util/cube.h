#ifndef CUBE_H
#define CUBE_H

#include "math/vec2.h"
#include "math/vec3.h"

struct vec3 cubePositions[36];
struct vec3 cubeNormals[36];
struct vec2 cubeUV[36];

void cubeInit();

#endif // CUBE_H
