#ifndef STAGING_BUFFER_H
#define STAGING_BUFFER_H

#include <stdbool.h>

#include "vulkan_includes.h"
#include "containers/vector.h"
#include "util/defines.h"

struct VulkanDevice;

struct StagingBuffer {
	struct VulkanDevice* vk;
	VkBuffer buffer;
	VkDeviceMemory memory;
	uint8_t* mapped;
	uint32_t offset;

	struct Vector textureSubmits[THREAD_COUNT];
};

void stagingBufferInit(struct StagingBuffer* stagingBuffer, struct VulkanDevice* vk);
void stagingBufferDestroy(struct StagingBuffer* stagingBuffer);

uint32_t stagingBufferGetOffset(struct StagingBuffer* stagingBuffer);
uint8_t* stagingBufferGetPointer(struct StagingBuffer* stagingBuffer, uint32_t offset);
uint32_t stagingBufferGetAvailableSize(struct StagingBuffer* stagingBuffer);

void stagingBufferSubmitTexture(struct StagingBuffer* stagingBuffer, struct Texture* texture, uint32_t imageSize);
void stagingBufferSubmit(struct StagingBuffer* stagingBuffer, VkCommandBuffer commandBuffer);
void stagingBufferLoad(struct StagingBuffer* stagingBuffer, VkCommandBuffer commandBuffer);

#endif // STAGING_BUFFER_H
