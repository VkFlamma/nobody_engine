#include "storage.h"

#include <stdlib.h>

#include "vulkan/vulkan_device.h"
#include "entities/voxel.h"
#include "entities/particle.h"
#include "util/values.h"

void storageInit(struct Storage* storage) {
	int initialSize = 256;
	vectorInit(&storage->voxels, initialSize, sizeof(struct Voxel));
	vectorInit(&storage->particles, initialSize, sizeof(struct Particle));

	for (int i = 0; i < THREAD_COUNT; ++i) {
		vectorInit(&storage->drawQueue[i], initialSize, sizeof(uint32_t));
	}
}

void storageDestroy(struct Storage* storage) {
	for (int i = 0; i < storage->voxels.count; ++i) {
		voxelDestroy(vectorGet(&storage->voxels, i));
	}
	for (int i = 0; i < storage->particles.count; ++i) {
		particleDestroy(vectorGet(&storage->particles, i));
	}
	vectorDestroy(&storage->voxels);
	vectorDestroy(&storage->particles);
	for (int i = 0; i < THREAD_COUNT; ++i) {
		vectorDestroy(&storage->drawQueue[i]);
	}
}

void storageReload(struct Storage* storage, struct VulkanDevice* vk) {
	/*voxelReload(vectorGet(&storage->voxels, i), &vk->voxelPipeline, testVoxelPath);
	voxelReload(vectorGet(&storage->voxels, i), &vk->voxelPipeline, treeVoxelPath);*/

	for (int i = 0; i < storage->particles.count; ++i) {
		particleReload(vectorGet(&storage->particles, i), &vk->particlesPipeline);
	}
}

void insertIntoDrawQueue(struct Storage* storage, struct VulkanDevice* vk, int pipeline, int pipelineIndex) {
	uint32_t v = (((uint32_t)pipeline) << 16) | ((uint32_t)pipelineIndex);
	uint32_t* q = vectorAddAndGet(&storage->drawQueue[vk->writeIndex]);
	*q = v;
}

void storageUpdate(struct Storage* storage, struct VulkanDevice* vk, int deltaTime) {
	storage->drawQueue[vk->writeIndex].count = 0;
	vk->voxelPipeline.queueIndex[vk->writeIndex] = 0;
	vk->voxelPipeline.firstVertex = 0;
	vk->particlesPipeline.queueIndex[vk->writeIndex] = 0;
	vk->particlesPipeline.firstInstance = 0;

	for (int i = 0; i < storage->voxels.count; ++i) {
		voxelCalculateTransformation(vectorGet(&storage->voxels, i));
	}
	for (int i = 0; i < storage->voxels.count; ++i) {
		voxelUpdate(vectorGet(&storage->voxels, i), &vk->voxelPipeline);
		/*bool draw = voxelUpdate(vectorGet(&storage->voxels, i), &vk->voxelPipeline);

		if (draw) {
			insertIntoDrawQueue(storage, vk, 0, i);
		}*/
	}
	for (int i = 0; i < storage->particles.count; ++i) {
		particleUpdate(vectorGet(&storage->particles, i), &vk->particlesPipeline, deltaTime);
		/*bool draw = particleUpdate(vectorGet(&storage->particles, i), &vk->particlesPipeline, deltaTime);

		if (draw) {
			insertIntoDrawQueue(storage, vk, 1, i);
		}*/
	}
}

void storageCompute(struct Storage* storage, struct VulkanDevice* vk) {
	particlePipelineCompute(&vk->particlesPipeline);
}

void storageRender(struct Storage* storage, struct VulkanDevice* vk) {
	voxelPipelineRender(&vk->voxelPipeline);
	particlePipelineRender(&vk->particlesPipeline);
}

int storageAddVoxel(struct Storage* storage) {
	return vectorAdd(&storage->voxels);
}

int storageAddParticle(struct Storage* storage) {
	return vectorAdd(&storage->particles);
}

struct Voxel* storageGetVoxel(struct Storage* storage, int i) {
	return vectorGet(&storage->voxels, i);
}
struct Particle* storageGetParticle(struct Storage* storage, int i) {
	return vectorGet(&storage->particles, i);
}