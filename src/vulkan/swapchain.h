#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#include "vulkan_includes.h"
#include "vulkan_util.h"

struct Swapchain {
	VkSwapchainKHR swapchain;
	VkExtent2D extent;
	VkSurfaceFormatKHR surfaceFormat;
	VkPresentModeKHR presentMode;
	VkImage images[IMAGE_COUNT];
	VkImageView imageViews[IMAGE_COUNT];
};

void swapchainInit(struct Swapchain* swapchain, struct VulkanDevice* vk);
void swapchainDestroy(struct Swapchain* swapchain, struct VulkanDevice* vk);

#endif // SWAPCHAIN_H
