#include "cube.h"

void cubeInit() {
	vec3_init(&cubePositions[0], 1.0f, 1.0f, 1.0f);
	vec3_init(&cubePositions[1], 1.0f, 0.0f, 1.0f);
	vec3_init(&cubePositions[2], 0.0f, 0.0f, 1.0f);
	vec3_init(&cubePositions[3], 0.0f, 1.0f, 1.0f);
	vec3_init(&cubePositions[4], 1.0f, 1.0f, 1.0f);
	vec3_init(&cubePositions[5], 0.0f, 0.0f, 1.0f);

	vec3_init(&cubePositions[6], 0.0f, 0.0f, 0.0f);
	vec3_init(&cubePositions[7], 1.0f, 0.0f, 0.0f);
	vec3_init(&cubePositions[8], 1.0f, 1.0f, 0.0f);
	vec3_init(&cubePositions[9], 0.0f, 0.0f, 0.0f);
	vec3_init(&cubePositions[10], 1.0f, 1.0f, 0.0f);
	vec3_init(&cubePositions[11], 0.0f, 1.0f, 0.0f);

	vec3_init(&cubePositions[12], 0.0f, 1.0f, 0.0f);
	vec3_init(&cubePositions[13], 1.0f, 1.0f, 0.0f);
	vec3_init(&cubePositions[14], 1.0f, 1.0f, 1.0f);
	vec3_init(&cubePositions[15], 0.0f, 1.0f, 0.0f);
	vec3_init(&cubePositions[16], 1.0f, 1.0f, 1.0f);
	vec3_init(&cubePositions[17], 0.0f, 1.0f, 1.0f);

	vec3_init(&cubePositions[18], 1.0f, 0.0f, 1.0f);
	vec3_init(&cubePositions[19], 1.0f, 0.0f, 0.0f);
	vec3_init(&cubePositions[20], 0.0f, 0.0f, 0.0f);
	vec3_init(&cubePositions[21], 0.0f, 0.0f, 1.0f);
	vec3_init(&cubePositions[22], 1.0f, 0.0f, 1.0f);
	vec3_init(&cubePositions[23], 0.0f, 0.0f, 0.0f);

	vec3_init(&cubePositions[24], 1.0f, 1.0f, 1.0f);
	vec3_init(&cubePositions[25], 1.0f, 1.0f, 0.0f);
	vec3_init(&cubePositions[26], 1.0f, 0.0f, 0.0f);
	vec3_init(&cubePositions[27], 1.0f, 0.0f, 1.0f);
	vec3_init(&cubePositions[28], 1.0f, 1.0f, 1.0f);
	vec3_init(&cubePositions[29], 1.0f, 0.0f, 0.0f);

	vec3_init(&cubePositions[30], 0.0f, 0.0f, 0.0f);
	vec3_init(&cubePositions[31], 0.0f, 1.0f, 0.0f);
	vec3_init(&cubePositions[32], 0.0f, 1.0f, 1.0f);
	vec3_init(&cubePositions[33], 0.0f, 0.0f, 0.0f);
	vec3_init(&cubePositions[34], 0.0f, 1.0f, 1.0f);
	vec3_init(&cubePositions[35], 0.0f, 0.0f, 1.0f);

	vec3_init(&cubeNormals[0], 0.0f, 0.0f, 1.0f);
	vec3_init(&cubeNormals[1], 0.0f, 0.0f, 1.0f);
	vec3_init(&cubeNormals[2], 0.0f, 0.0f, 1.0f);
	vec3_init(&cubeNormals[3], 0.0f, 0.0f, 1.0f);
	vec3_init(&cubeNormals[4], 0.0f, 0.0f, 1.0f);
	vec3_init(&cubeNormals[5], 0.0f, 0.0f, 1.0f);

	vec3_init(&cubeNormals[6], 0.0f, 0.0f, -1.0f);
	vec3_init(&cubeNormals[7], 0.0f, 0.0f, -1.0f);
	vec3_init(&cubeNormals[8], 0.0f, 0.0f, -1.0f);
	vec3_init(&cubeNormals[9], 0.0f, 0.0f, -1.0f);
	vec3_init(&cubeNormals[10], 0.0f, 0.0f, -1.0f);
	vec3_init(&cubeNormals[11], 0.0f, 0.0f, -1.0f);

	vec3_init(&cubeNormals[12], 0.0f, 1.0f, 0.0f);
	vec3_init(&cubeNormals[13], 0.0f, 1.0f, 0.0f);
	vec3_init(&cubeNormals[14], 0.0f, 1.0f, 0.0f);
	vec3_init(&cubeNormals[15], 0.0f, 1.0f, 0.0f);
	vec3_init(&cubeNormals[16], 0.0f, 1.0f, 0.0f);
	vec3_init(&cubeNormals[17], 0.0f, 1.0f, 0.0f);

	vec3_init(&cubeNormals[18], 0.0f, -1.0f, 0.0f);
	vec3_init(&cubeNormals[19], 0.0f, -1.0f, 0.0f);
	vec3_init(&cubeNormals[20], 0.0f, -1.0f, 0.0f);
	vec3_init(&cubeNormals[21], 0.0f, -1.0f, 0.0f);
	vec3_init(&cubeNormals[22], 0.0f, -1.0f, 0.0f);
	vec3_init(&cubeNormals[23], 0.0f, -1.0f, 0.0f);

	vec3_init(&cubeNormals[24], 1.0f, 0.0f, 0.0f);
	vec3_init(&cubeNormals[25], 1.0f, 0.0f, 0.0f);
	vec3_init(&cubeNormals[26], 1.0f, 0.0f, 0.0f);
	vec3_init(&cubeNormals[27], 1.0f, 0.0f, 0.0f);
	vec3_init(&cubeNormals[28], 1.0f, 0.0f, 0.0f);
	vec3_init(&cubeNormals[29], 1.0f, 0.0f, 0.0f);

	vec3_init(&cubeNormals[30], -1.0f, 0.0f, 0.0f);
	vec3_init(&cubeNormals[31], -1.0f, 0.0f, 0.0f);
	vec3_init(&cubeNormals[32], -1.0f, 0.0f, 0.0f);
	vec3_init(&cubeNormals[33], -1.0f, 0.0f, 0.0f);
	vec3_init(&cubeNormals[34], -1.0f, 0.0f, 0.0f);
	vec3_init(&cubeNormals[35], -1.0f, 0.0f, 0.0f);
	
	vec2_init(&cubeUV[0], 1.0f, 1.0f);
	vec2_init(&cubeUV[1], 1.0f, 0.0f);
	vec2_init(&cubeUV[2], 0.0f, 0.0f);
	vec2_init(&cubeUV[3], 0.0f, 1.0f);
	vec2_init(&cubeUV[4], 1.0f, 1.0f);
	vec2_init(&cubeUV[5], 0.0f, 0.0f);

	vec2_init(&cubeUV[6], 0.0f, 0.0f);
	vec2_init(&cubeUV[7], 1.0f, 0.0f);
	vec2_init(&cubeUV[8], 1.0f, 1.0f);
	vec2_init(&cubeUV[9], 0.0f, 0.0f);
	vec2_init(&cubeUV[10], 1.0f, 1.0f);
	vec2_init(&cubeUV[11], 0.0f, 1.0f);

	vec2_init(&cubeUV[12], 0.0f, 0.0f);
	vec2_init(&cubeUV[13], 1.0f, 0.0f);
	vec2_init(&cubeUV[14], 1.0f, 1.0f);
	vec2_init(&cubeUV[15], 0.0f, 0.0f);
	vec2_init(&cubeUV[16], 1.0f, 1.0f);
	vec2_init(&cubeUV[17], 0.0f, 1.0f);

	vec2_init(&cubeUV[18], 1.0f, 1.0f);
	vec2_init(&cubeUV[19], 1.0f, 0.0f);
	vec2_init(&cubeUV[20], 0.0f, 0.0f);
	vec2_init(&cubeUV[21], 0.0f, 1.0f);
	vec2_init(&cubeUV[22], 1.0f, 1.0f);
	vec2_init(&cubeUV[23], 0.0f, 0.0f);

	vec2_init(&cubeUV[24], 1.0f, 1.0f);
	vec2_init(&cubeUV[25], 1.0f, 0.0f);
	vec2_init(&cubeUV[26], 0.0f, 0.0f);
	vec2_init(&cubeUV[27], 0.0f, 1.0f);
	vec2_init(&cubeUV[28], 1.0f, 1.0f);
	vec2_init(&cubeUV[29], 0.0f, 0.0f);

	vec2_init(&cubeUV[30], 0.0f, 0.0f);
	vec2_init(&cubeUV[31], 1.0f, 0.0f);
	vec2_init(&cubeUV[32], 1.0f, 1.0f);
	vec2_init(&cubeUV[33], 0.0f, 0.0f);
	vec2_init(&cubeUV[34], 1.0f, 1.0f);
	vec2_init(&cubeUV[35], 0.0f, 1.0f);
}