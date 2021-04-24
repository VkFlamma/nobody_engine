#ifndef STORAGE_H
#define STORAGE_H

#include <stdint.h>

#include "vector.h"
#include "util/defines.h"

struct VulkanDevice;
struct Voxel;
struct Particle;

struct Storage {
	struct Vector voxels;
	struct Vector particles;
	struct Vector drawQueue[THREAD_COUNT];
};

void storageInit(struct Storage* storage);
void storageDestroy(struct Storage* storage);

void storageReload(struct Storage* storage, struct VulkanDevice* vk);
void storageUpdate(struct Storage* storage, struct VulkanDevice* vk, int deltaTime);
void storageCompute(struct Storage* storage, struct VulkanDevice* vk);
void storageRender(struct Storage* storage, struct VulkanDevice* vk);

int storageAddVoxel(struct Storage* storage);
int storageAddParticle(struct Storage* storage);

struct Voxel* storageGetVoxel(struct Storage* storage, int i);
struct Particle* storageGetParticle(struct Storage* storage, int i);

#endif // STORAGE_H
