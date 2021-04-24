#ifndef VOXEL_H
#define VOXEL_H

#include <stdint.h>
#include <stdbool.h>

#include "pipelines/voxel_pipeline.h"
#include "math/transform.h"

struct Voxlet {
	float x;
	float y;
	float z;
	float r;
	float g;
	float b;
	float a;
	int boneIndex;
};

struct Voxel {
	struct Voxel* parent;
	struct Voxlet* voxlets;
	struct VoxelVertex* vertices;
	struct VoxelUniform uniform;
	struct Transform transform;
	struct Transform bones[MAX_BONES];
	struct mat4 transformation;
	struct vec3 bboxMin;
	struct vec3 bboxMax;
	int count;
	int bonesCount;
	int id;
	int specialization;
	bool draw;
};

void voxelInit(struct Voxel* voxel, struct VoxelPipeline* pipeline, const char* path, int id);
void voxelCalculateTransformation(struct Voxel* voxel);
bool voxelUpdate(struct Voxel* voxel, struct VoxelPipeline* pipeline);
void voxelDestroy(struct Voxel* voxel);
void voxelReload(struct Voxel* voxel, struct VoxelPipeline* pipeline, const char* path);

void voxelUseWireframe(struct Voxel* voxel);

#endif // VOXEL_H
