#ifndef TEXTURE_ALLOCATOR_H
#define TEXTURE_ALLOCATOR_H

#include "vulkan_includes.h"
#include "containers/vector.h"

struct TextureAllocator {
	VkDeviceMemory memory;
	uint32_t memoryType;
	VkImageUsageFlags imageUsage;
	VkMemoryPropertyFlags memoryProperties;
	VkDeviceSize size;
	struct Vector blocks;
	VkDescriptorPool descriptorPool;
	VkDescriptorSetLayout descriptorSetLayout;
};

void textureAllocatorInit(struct TextureAllocator* textureAllocator, struct VulkanDevice* vk);
void textureAllocatorDestroy(struct TextureAllocator* textureAllocator, struct VulkanDevice* vk);

struct MemoryBlock requestImageMemory(struct TextureAllocator* textureAllocator, struct VulkanDevice* vk, VkImage image);
void freeImageMemory(struct TextureAllocator* textureAllocator, struct MemoryBlock* block);

#endif // TEXTURE_ALLOCATOR_H
