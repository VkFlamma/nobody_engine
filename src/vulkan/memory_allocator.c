#include "memory_allocator.h"

#include <assert.h>

#include "vulkan_device.h"
#include "vulkan_debug.h"
#include "util/defines.h"

VkBuffer createBuffer(struct VulkanDevice* vk, uint64_t size, VkBufferUsageFlags usage) {
	VkBufferCreateInfo bufferCreateInfo;
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.pNext = NULL;
	bufferCreateInfo.flags = 0;
	bufferCreateInfo.size = size;
	bufferCreateInfo.usage = usage;
	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	bufferCreateInfo.queueFamilyIndexCount = 0;
	bufferCreateInfo.pQueueFamilyIndices = NULL;

	VkBuffer buffer;
	VK_ASSERT(vkCreateBuffer(vk->device, &bufferCreateInfo, NULL, &buffer));
	return buffer;
}

VkMemoryRequirements getBufferMemoryRequirements(struct VulkanDevice* vk, VkBuffer buffer) {
	VkBufferMemoryRequirementsInfo2 bufferMemoryRequirementsInfo;
	bufferMemoryRequirementsInfo.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_REQUIREMENTS_INFO_2;
	bufferMemoryRequirementsInfo.pNext = NULL;
	bufferMemoryRequirementsInfo.buffer = buffer;

	VkMemoryDedicatedRequirements memoryDedicatedRequirements;
	memoryDedicatedRequirements.sType = VK_STRUCTURE_TYPE_MEMORY_DEDICATED_REQUIREMENTS;
	memoryDedicatedRequirements.pNext = NULL;
	memoryDedicatedRequirements.prefersDedicatedAllocation = VK_FALSE;
	memoryDedicatedRequirements.requiresDedicatedAllocation = VK_FALSE;

	VkMemoryRequirements2 memoryRequirements;
	memoryRequirements.sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2;
	memoryRequirements.pNext = &memoryDedicatedRequirements;
	vkGetBufferMemoryRequirements2(vk->device, &bufferMemoryRequirementsInfo, &memoryRequirements);
	return memoryRequirements.memoryRequirements;
}

void allocateMemory(struct VulkanDevice* vk, struct MemoryBuffer* memory) {
	VkBuffer dummyBuffer = createBuffer(vk, HOST_BUFFER_ALLOCATION_SIZE, memory->bufferUsage);
	VkMemoryRequirements memoryRequirements = getBufferMemoryRequirements(vk, dummyBuffer);
	vkDestroyBuffer(vk->device, dummyBuffer, NULL);

	memory->memoryType = findMemoryType(vk, memoryRequirements.memoryTypeBits, memory->memoryProperties);
	assert(memory->memoryType != UINT32_MAX);

	VkMemoryPriorityAllocateInfoEXT memoryPriority;
	memoryPriority.sType = VK_STRUCTURE_TYPE_MEMORY_PRIORITY_ALLOCATE_INFO_EXT;
	memoryPriority.pNext = NULL;
	memoryPriority.priority = 1.0f;

	VkMemoryAllocateFlagsInfo memoryAllocateFlagsInfo;
	memoryAllocateFlagsInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO;
	memoryAllocateFlagsInfo.pNext = &memoryPriority;
	memoryAllocateFlagsInfo.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT;
	memoryAllocateFlagsInfo.deviceMask = 0;

	VkMemoryAllocateInfo memoryAllocateInfo;
	memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocateInfo.pNext = &memoryAllocateFlagsInfo;
	memoryAllocateInfo.allocationSize = memory->size;
	memoryAllocateInfo.memoryTypeIndex = memory->memoryType;

	VK_ASSERT(vkAllocateMemory(vk->device, &memoryAllocateInfo, NULL, &memory->memory));
}

void memoryAllocatorInit(struct MemoryAllocator* memoryAllocator, struct VulkanDevice* vk) {
	memoryAllocator->hostMemory.bufferUsage = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
	memoryAllocator->hostMemory.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	memoryAllocator->hostMemory.size = HOST_BUFFER_ALLOCATION_SIZE;
	allocateMemory(vk, &memoryAllocator->hostMemory);
	vectorInit(&memoryAllocator->hostMemory.blocks, 256, sizeof(struct MemoryBlock));
	struct MemoryBlock* hostBlock = vectorAddAndGet(&memoryAllocator->hostMemory.blocks);
	hostBlock->memory = memoryAllocator->hostMemory.memory;
	hostBlock->offset = 0;
	hostBlock->size = memoryAllocator->hostMemory.size;
	vkMapMemory(vk->device, hostBlock->memory, 0, memoryAllocator->hostMemory.size, 0, (void**)&hostBlock->mapped);
	hostBlock->free = true;

	memoryAllocator->deviceMemory.bufferUsage = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
	memoryAllocator->deviceMemory.memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	memoryAllocator->deviceMemory.size = DEVICE_BUFFER_ALLOCATION_SIZE;
	allocateMemory(vk, &memoryAllocator->deviceMemory);
	vectorInit(&memoryAllocator->deviceMemory.blocks, 256, sizeof(struct MemoryBlock));
	struct MemoryBlock* deviceBlock = vectorAddAndGet(&memoryAllocator->deviceMemory.blocks);
	deviceBlock->memory = memoryAllocator->deviceMemory.memory;
	deviceBlock->offset = 0;
	deviceBlock->size = memoryAllocator->deviceMemory.size;
	deviceBlock->mapped = NULL;
	deviceBlock->free = true;
}

void memoryAllocatorDestroy(struct MemoryAllocator* memoryAllocator, struct VulkanDevice* vk) {
#ifdef MEMORY_CHECKS
	assert(memoryAllocator->hostMemory.blocks.count == 1);
	struct MemoryBlock* hostBlock = vectorGet(&memoryAllocator->hostMemory.blocks, 0);
	assert(hostBlock->free);
	assert(memoryAllocator->deviceMemory.blocks.count == 1);
	struct MemoryBlock* deviceBlock = vectorGet(&memoryAllocator->deviceMemory.blocks, 0);
	assert(deviceBlock->free);
#endif
	vectorDestroy(&memoryAllocator->hostMemory.blocks);
	vectorDestroy(&memoryAllocator->deviceMemory.blocks);
	vkFreeMemory(vk->device, memoryAllocator->hostMemory.memory, NULL);
	vkFreeMemory(vk->device, memoryAllocator->deviceMemory.memory, NULL);
}

void bindBufferMemory(struct VulkanDevice* vk, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize offset) {
	VkBindBufferMemoryInfo bindBufferMemoryInfo;
	bindBufferMemoryInfo.sType = VK_STRUCTURE_TYPE_BIND_BUFFER_MEMORY_INFO;
	bindBufferMemoryInfo.pNext = NULL;
	bindBufferMemoryInfo.buffer = buffer;
	bindBufferMemoryInfo.memory = memory;
	bindBufferMemoryInfo.memoryOffset = offset;
	VK_ASSERT(vkBindBufferMemory2(vk->device, 1, &bindBufferMemoryInfo));
}

struct MemoryBlock requestBufferMemory(struct MemoryAllocator* memoryAllocator, struct VulkanDevice* vk, VkBuffer buffer, VkMemoryPropertyFlags properties, uint64_t size) {
	struct MemoryBlock* block = NULL;
	VkMemoryRequirements memoryRequirements = getBufferMemoryRequirements(vk, buffer);
	uint32_t memoryType = findMemoryType(vk, memoryRequirements.memoryTypeBits, properties);
	if (memoryAllocator->hostMemory.memoryType == memoryType) {
		block = findBlock(&memoryAllocator->hostMemory.blocks, memoryRequirements.size, memoryRequirements.alignment);
	} else if (memoryAllocator->deviceMemory.memoryType == memoryType) {
		block = findBlock(&memoryAllocator->deviceMemory.blocks, memoryRequirements.size, memoryRequirements.alignment);
	}
#ifdef MEMORY_CHECKS
	assert(block);
#endif
	bindBufferMemory(vk, buffer, block->memory, block->offset);
	return *block;
}

void freeBufferMemory(struct MemoryAllocator* memoryAllocator, struct MemoryBlock* block) {
	if (memoryAllocator->hostMemory.memoryType) {
		freeBlock(&memoryAllocator->hostMemory.blocks, block);
	} else if (memoryAllocator->deviceMemory.memoryType) {
		freeBlock(&memoryAllocator->hostMemory.blocks, block);
	} else {
#ifdef MEMORY_CHECKS
		assert(0);
#endif
	}
}
