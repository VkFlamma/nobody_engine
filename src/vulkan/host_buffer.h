#ifndef HOST_BUFFER_H
#define HOST_BUFFER_H

#include "vulkan_includes.h"
#include "memory_allocator.h"

struct HostBuffer {
	struct MemoryBlock memoryBlock;
	VkBuffer buffer;
	VkDeviceAddress deviceAddress;
	int count;
	int maxCount;
	int stride;
	int size;
	int totalSize;
};

void hostBufferInit(struct HostBuffer* hostBuffer, struct VulkanDevice* vk, int stride, int maxCount, VkBufferUsageFlags usage);
void hostBufferDestroy(struct HostBuffer* hostBuffer, struct VulkanDevice* vk);

void hostBufferWrite(struct HostBuffer* hostBuffer, int bufferWriteIndex, int first, int count, void* data);

#endif // HOST_BUFFER_H
