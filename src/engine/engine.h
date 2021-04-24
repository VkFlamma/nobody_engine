#ifndef ENGINE_H
#define ENGINE_H

#include <time.h>

#include "entities/voxel.h"
#include "entities/particle.h"
#include "engine/animation.h"
#include "containers/storage.h"

struct Engine {
	struct AnimationEngine animEngine;
	struct Storage storage;

	int writeIndex;
	int readIndex;
	int bufferWriteIndex;
	int bufferReadIndex;

	struct timespec lastFrameTime;
	int delta;

	struct Voxel* debugWireframeVoxel;
	struct Voxel* activeVoxel;

	bool setCameraOnActiveObject;
};

struct Engine* engineInit(struct VulkanDevice* vulkanDevice);
void engineDestroy(struct Engine* engine);

void engineSyncedUpdate(struct Engine* engine, struct VulkanDevice* vulkanDevice, struct Window* window);
void engineUpdate(struct Engine* engine, struct VulkanDevice* vulkanDevice, struct Window* window);
void engineRender(struct Engine* engine, struct VulkanDevice* vulkanDevice);
void enginePresent(struct Engine* engine, struct VulkanDevice* vulkanDevice);

#endif // ENGINE_H
