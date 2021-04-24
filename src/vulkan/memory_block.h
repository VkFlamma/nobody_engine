#ifndef MEMORY_BLOCK_H
#define MEMORY_BLOCK_H

#include <stdbool.h>

#include "vulkan_includes.h"
#include "containers/vector.h"

struct MemoryBlock {
	VkDeviceMemory memory;
	uint64_t offset;
	uint64_t size;
	uint8_t* mapped;
	bool free;
};

struct MemoryBlock* findBlock(struct Vector* blocks, VkDeviceSize size, VkDeviceSize alignment);
void freeBlock(struct Vector* blocks, struct MemoryBlock* block);

#endif // MEMORY_BLOCK_H
