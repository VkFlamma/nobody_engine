#include "swapchain.h"

#include <stdlib.h>
#include <assert.h>

#include "vulkan_debug.h"
#include "vulkan_device.h"

void createSwapchain(struct Swapchain* swapchain, struct VulkanDevice* vk) {
	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	VK_ASSERT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vk->physicalDevice.physicalDevice, vk->surface, &surfaceCapabilities));

	uint32_t maxImageCount = surfaceCapabilities.maxImageCount;
	uint32_t minImageCount = surfaceCapabilities.minImageCount;
	assert(IMAGE_COUNT <= maxImageCount);
	assert(IMAGE_COUNT >= minImageCount);

	swapchain->extent = surfaceCapabilities.currentExtent;
	if (swapchain->extent.width == 0 || swapchain->extent.height == 0) {
		return;
	}

	uint32_t surfaceFormatCount;
	VK_ASSERT(vkGetPhysicalDeviceSurfaceFormatsKHR(vk->physicalDevice.physicalDevice, vk->surface, &surfaceFormatCount, NULL));
	VkSurfaceFormatKHR* surfaceFormats = getAllocationBuffer(vk, sizeof(VkSurfaceFormatKHR) * surfaceFormatCount);
	VK_ASSERT(vkGetPhysicalDeviceSurfaceFormatsKHR(vk->physicalDevice.physicalDevice, vk->surface, &surfaceFormatCount, surfaceFormats));

	if (surfaceFormatCount == 1 && surfaceFormats[0].format == VK_FORMAT_UNDEFINED) {
		swapchain->surfaceFormat.format = VK_FORMAT_B8G8R8A8_UNORM;
		swapchain->surfaceFormat.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	} else {
		swapchain->surfaceFormat = surfaceFormats[0];
		for (uint32_t i = 0; i < surfaceFormatCount; ++i) {
			if (surfaceFormats[i].format == VK_FORMAT_B8G8R8A8_UNORM && surfaceFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				swapchain->surfaceFormat = surfaceFormats[i];
				break;
			}
		}
	}

	uint32_t presentModeCount;
	VkPhysicalDeviceSurfaceInfo2KHR physicalDeviceSurfaceInfo;
	physicalDeviceSurfaceInfo.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR;
	physicalDeviceSurfaceInfo.pNext = NULL;
	physicalDeviceSurfaceInfo.surface = vk->surface;
	vkGetPhysicalDeviceSurfacePresentModes2EXT(vk->physicalDevice.physicalDevice, &physicalDeviceSurfaceInfo, &presentModeCount, NULL);

	VkPresentModeKHR* presentModes = getAllocationBuffer(vk, sizeof(VkPresentModeKHR) * presentModeCount);
	vkGetPhysicalDeviceSurfacePresentModes2EXT(vk->physicalDevice.physicalDevice, &physicalDeviceSurfaceInfo, &presentModeCount, presentModes);

	swapchain->presentMode = presentModes[0];
	for (uint32_t i = 0; i < presentModeCount; ++i) {
		if (presentModes[i] == VK_PRESENT_MODE_FIFO_KHR) {
			swapchain->presentMode = presentModes[i];
			break;
		}
	}

	VkSwapchainKHR oldSwapchain = swapchain->swapchain;

	VkSwapchainCreateInfoKHR swapchainCreateInfo;
	swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCreateInfo.pNext = NULL;
	swapchainCreateInfo.flags = 0;
	swapchainCreateInfo.surface = vk->surface;
	swapchainCreateInfo.minImageCount = IMAGE_COUNT;
	swapchainCreateInfo.imageFormat = swapchain->surfaceFormat.format;
	swapchainCreateInfo.imageColorSpace = swapchain->surfaceFormat.colorSpace;
	swapchainCreateInfo.imageExtent = swapchain->extent;
	swapchainCreateInfo.imageArrayLayers = 1;
	swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapchainCreateInfo.queueFamilyIndexCount = 0;
	swapchainCreateInfo.pQueueFamilyIndices = NULL;
	swapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
	swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainCreateInfo.presentMode = swapchain->presentMode;
	swapchainCreateInfo.clipped = VK_FALSE;
	swapchainCreateInfo.oldSwapchain = oldSwapchain;

	VK_ASSERT(vkCreateSwapchainKHR(vk->device, &swapchainCreateInfo, NULL, &swapchain->swapchain));

	if (oldSwapchain != VK_NULL_HANDLE) {
		vkDestroySwapchainKHR(vk->device, oldSwapchain, NULL);
	}
}

void createImages(struct Swapchain* swapchain, struct VulkanDevice* vk) {
	uint32_t imageCount;
	vkGetSwapchainImagesKHR(vk->device, swapchain->swapchain, &imageCount, NULL);
	if (imageCount > IMAGE_COUNT) {
		imageCount = IMAGE_COUNT;
	}
	vkGetSwapchainImagesKHR(vk->device, swapchain->swapchain, &imageCount, swapchain->images);

	VkImageViewCreateInfo imageViewCreateInfo;
	imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewCreateInfo.pNext = NULL;
	imageViewCreateInfo.flags = 0;
	imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imageViewCreateInfo.format = swapchain->surfaceFormat.format;
	imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R;
	imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_G;
	imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_B;
	imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_A;
	imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
	imageViewCreateInfo.subresourceRange.levelCount = 1;
	imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
	imageViewCreateInfo.subresourceRange.layerCount = 1;

	for (uint32_t i = 0; i < IMAGE_COUNT; ++i) {
		imageViewCreateInfo.image = swapchain->images[i];
		VK_ASSERT(vkCreateImageView(vk->device, &imageViewCreateInfo, NULL, &swapchain->imageViews[i]));
	}
}

void swapchainInit(struct Swapchain* swapchain, struct VulkanDevice* vk) {
	swapchain->swapchain = VK_NULL_HANDLE;
	createSwapchain(swapchain, vk);
	createImages(swapchain, vk);
}

void swapchainDestroy(struct Swapchain* swapchain, struct VulkanDevice* vk) {
	for (uint32_t i = 0; i < IMAGE_COUNT; ++i) {
		vkDestroyImageView(vk->device, swapchain->imageViews[i], NULL);
	}
	vkDestroySwapchainKHR(vk->device, swapchain->swapchain, NULL);
}
