#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include "vulkan_includes.h"
#include "containers/vector.h"
#include "memory_block.h"

struct VulkanDevice;

struct MemoryBuffer {
	VkDeviceMemory memory;
	uint32_t memoryType;
	VkBufferUsageFlags bufferUsage;
	VkMemoryPropertyFlags memoryProperties;
	VkDeviceSize size;
	struct Vector blocks;
};

struct MemoryAllocator {
	struct MemoryBuffer hostMemory;
	struct MemoryBuffer deviceMemory;
};

VkBuffer createBuffer(struct VulkanDevice* vk, uint64_t size, VkBufferUsageFlags usage);

void memoryAllocatorInit(struct MemoryAllocator* memoryAllocator, struct VulkanDevice* vk);
void memoryAllocatorDestroy(struct MemoryAllocator* memoryAllocator, struct VulkanDevice* vk);

struct MemoryBlock requestBufferMemory(struct MemoryAllocator* memoryAllocator, struct VulkanDevice* vk, VkBuffer buffer, VkMemoryPropertyFlags properties, uint64_t size);
void freeBufferMemory(struct MemoryAllocator* memoryAllocator, struct MemoryBlock* block);

#endif // MEMORY_ALLOCATOR_H
