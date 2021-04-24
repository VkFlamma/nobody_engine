#ifndef VULKAN_UTIL_H
#define VULKAN_UTIL_H

#include "vulkan_includes.h"

static const char* instanceExtensions[] = {
	VK_KHR_SURFACE_EXTENSION_NAME,
	VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
	VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
	VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
	VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME,
};

static const char* deviceExtensions[] = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME,
	VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME,
	VK_KHR_COPY_COMMANDS_2_EXTENSION_NAME,
	VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
	VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
	VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
};

VkShaderModule createShaderModule(struct VulkanDevice* vk, const char* shader_name);

void createBufferAndMemory(struct VulkanDevice* vk, VkBuffer* buffer, VkDeviceMemory* memory, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
VkDeviceAddress getBufferAddress(struct VulkanDevice* vk, VkBuffer buffer);
void destroyBufferAndMemory(struct VulkanDevice* vk, VkBuffer* buffer, VkDeviceMemory* memory);

VkFence createFence(struct VulkanDevice* vk);

void recompileSpirv();

#endif // VULKAN_UTIL_H
