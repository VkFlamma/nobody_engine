#include "voxel.h"

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <assert.h>

#include "util/cube.h"
#include "math/mat4.h"
#include "vulkan/context.h"
#include "vulkan/vulkan_device.h"
#include "math/math_util.h"

void voxelUpdateVertices(struct Voxel* voxel, struct VoxelPipeline* pipeline) {
	struct VoxelVertex* v = voxel->vertices;
	for (int i = 0; i < voxel->count; ++i) {
		for (int j = 0; j < 36; ++j) {
			int index = i * 36 + j;
			v[index].x = cubePositions[j].x + voxel->voxlets[i].x;
			v[index].y = cubePositions[j].y + voxel->voxlets[i].y;
			v[index].z = cubePositions[j].z + voxel->voxlets[i].z;

			v[index].r = voxel->voxlets[i].r;
			v[index].g = voxel->voxlets[i].g;
			v[index].b = voxel->voxlets[i].b;
			v[index].a = voxel->voxlets[i].a;

			v[index].boneIndex = voxel->voxlets[i].boneIndex;
		}
	}

}

void voxelUpdateUniform(struct Voxel* voxel, struct VoxelPipeline* pipeline) {
	if (voxel->parent) {
		mat4_mul(&voxel->uniform.modelMatrix, &voxel->parent->transformation, &voxel->transformation);
	} else {
		voxel->uniform.modelMatrix = voxel->transformation;
	}
	for (int i = 0; i < MAX_BONES; ++i) {
		calculateTransform(&voxel->uniform.boneMatrices[i], &voxel->bones[i]);
	}
}

void voxelLoadModel(struct Voxel* voxel, const char* path) {
	voxel->draw = true;
	vec3_init(&voxel->bboxMin, FLT_MAX, FLT_MAX, FLT_MAX);
	vec3_init(&voxel->bboxMax, -FLT_MAX, -FLT_MAX, -FLT_MAX);

	FILE* fp = fopen(path, "r");

	fscanf(fp, "%f", &voxel->transform.scale.x);
	fscanf(fp, "%f", &voxel->transform.scale.y);
	fscanf(fp, "%f", &voxel->transform.scale.z);
	fscanf(fp, "%f", &voxel->transform.position.x);
	fscanf(fp, "%f", &voxel->transform.position.y);
	fscanf(fp, "%f", &voxel->transform.position.z);
	fscanf(fp, "%f", &voxel->transform.rotationAxis.x);
	fscanf(fp, "%f", &voxel->transform.rotationAxis.y);
	fscanf(fp, "%f", &voxel->transform.rotationAxis.z);
	fscanf(fp, "%f", &voxel->transform.rotationAngle);

	fscanf(fp, "%i", &voxel->bonesCount);
	assert(voxel->bonesCount <= MAX_BONES);
	for (int i = 0; i < voxel->bonesCount; ++i) {
		fscanf(fp, "%f", &voxel->bones[i].scale.x);
		fscanf(fp, "%f", &voxel->bones[i].scale.y);
		fscanf(fp, "%f", &voxel->bones[i].scale.z);
		fscanf(fp, "%f", &voxel->bones[i].position.x);
		fscanf(fp, "%f", &voxel->bones[i].position.y);
		fscanf(fp, "%f", &voxel->bones[i].position.z);
		fscanf(fp, "%f", &voxel->bones[i].rotationAxis.x);
		fscanf(fp, "%f", &voxel->bones[i].rotationAxis.y);
		fscanf(fp, "%f", &voxel->bones[i].rotationAxis.z);
		fscanf(fp, "%f", &voxel->bones[i].rotationAngle);
	}

	fscanf(fp, "%i", &voxel->count);

	voxel->voxlets = malloc(sizeof(struct Voxlet) * voxel->count);
	voxel->vertices = malloc(sizeof(struct VoxelVertex) * voxel->count * 36);

	for (int i = 0; i < voxel->count; ++i) {
		if (fscanf(fp, "%f", &voxel->voxlets[i].x) == EOF) break;
		fscanf(fp, "%f", &voxel->voxlets[i].y);
		fscanf(fp, "%f", &voxel->voxlets[i].z);
		fscanf(fp, "%f", &voxel->voxlets[i].r);
		fscanf(fp, "%f", &voxel->voxlets[i].g);
		fscanf(fp, "%f", &voxel->voxlets[i].b);
		fscanf(fp, "%f", &voxel->voxlets[i].a);
		fscanf(fp, "%u", &voxel->voxlets[i].boneIndex);
		int bi = voxel->voxlets[i].boneIndex;
		assert(bi < voxel->bonesCount);

		float bx = voxel->bones[bi].position.x;
		float by = voxel->bones[bi].position.y;
		float bz = voxel->bones[bi].position.z;

		voxel->bboxMin.x = float_min(voxel->bboxMin.x, voxel->voxlets[i].x + bx);
		voxel->bboxMin.y = float_min(voxel->bboxMin.y, voxel->voxlets[i].y + by);
		voxel->bboxMin.z = float_min(voxel->bboxMin.z, voxel->voxlets[i].z + bz);
		voxel->bboxMax.x = float_max(voxel->bboxMax.x, voxel->voxlets[i].x + bx);
		voxel->bboxMax.y = float_max(voxel->bboxMax.y, voxel->voxlets[i].y + by);
		voxel->bboxMax.z = float_max(voxel->bboxMax.z, voxel->voxlets[i].z + bz);
	}
	++voxel->bboxMax.x;
	++voxel->bboxMax.y;
	++voxel->bboxMax.z;

	fclose(fp);
}

void voxelInit(struct Voxel* voxel, struct VoxelPipeline* pipeline, const char* path, int id) {
	voxel->parent = NULL;
	voxel->id = id;
	voxel->specialization = VOXEL_GRAPHICS_DEFAULT;
	voxelLoadModel(voxel, path);
	voxelUpdateVertices(voxel, pipeline);
	voxelUpdateUniform(voxel, pipeline);
}

void voxelCalculateTransformation(struct Voxel* voxel) {
	calculateTransform(&voxel->transformation, &voxel->transform);
}

bool voxelUpdate(struct Voxel* voxel, struct VoxelPipeline* pipeline) {
	if (!voxel->draw) {
		return false;
	}

	voxelUpdateUniform(voxel, pipeline);

	struct RenderContext* renderCtx = voxelPipelineGetRenderContext(pipeline);
	renderCtx->specialization = voxel->specialization;

	voxelPipelineAddToQueue(pipeline, voxel->count * 36, voxel->vertices, &voxel->uniform);

	return true;
}

void voxelDestroy(struct Voxel* voxel) {
	free(voxel->voxlets);
	free(voxel->vertices);
}

void voxelReload(struct Voxel* voxel, struct VoxelPipeline* pipeline, const char* path) {
	voxelDestroy(voxel);
	voxelLoadModel(voxel, path);
	voxelUpdateVertices(voxel, pipeline);
	voxelUpdateUniform(voxel, pipeline);
}

void voxelUseWireframe(struct Voxel* voxel) {
	voxel->specialization = VOXEL_GRAPHICS_WIREFRAME;
}
