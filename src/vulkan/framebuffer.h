#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "vulkan_includes.h"
#include "vulkan_util.h"
#include "texture.h"

struct VulkanDevice;

struct Framebuffer {
	VkRenderPass renderPass;
	VkRenderPass guiRenderPass;
	VkExtent2D extent;
	VkFramebuffer framebuffers[IMAGE_COUNT];
	struct Texture depthTexture;
	uint32_t width;
	uint32_t height;
};

void framebufferInit(struct Framebuffer* framebuffer, struct VulkanDevice* vk);
void framebufferDestroy(struct Framebuffer* framebuffer, struct VulkanDevice* vk);

void framebufferBeginRenderPass(struct Framebuffer* framebuffer, struct VulkanDevice* vk, VkCommandBuffer commandBuffer);
void framebufferBeginGuiRenderPass(struct Framebuffer* framebuffer, struct VulkanDevice* vk, VkCommandBuffer commandBuffer);
void framebufferEndRenderPass(struct Framebuffer* framebuffer, VkCommandBuffer commandBuffer);

#endif // FRAMEBUFFER_H
