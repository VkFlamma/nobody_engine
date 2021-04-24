#ifndef TEXTURE_H
#define TEXTURE_H

#include "vulkan_includes.h"
#include "memory_block.h"

struct VulkanDevice;

struct Texture {
	struct MemoryBlock memoryBlock;
	VkImage image;
	VkImageView view;
	VkSampler sampler;
	VkImageLayout layout;
	VkFormat format;
	VkImageUsageFlags usage;
	VkFilter filter;
	VkExtent2D extent;
	VkDescriptorSet descriptorSet;
	uint32_t stagingBufferOffset;
};

void transitionImageLayout(struct Texture* texture, VkCommandBuffer commandBuffer, VkImageLayout newLayout);
void transitionImageLayoutTransfer(struct Texture* texture, VkCommandBuffer commandBuffer, VkImageLayout newLayout, uint32_t srcQueue, uint32_t dstQueue);

void textureInitColor(struct Texture* texture, struct VulkanDevice* vk, const char* file);
void textureInitDepth(struct Texture* texture, struct VulkanDevice* vk, VkExtent2D extent);
void textureDestroy(struct Texture* texture, struct VulkanDevice* vk);

#endif // TEXTURE_H
