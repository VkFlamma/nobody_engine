#ifndef VULKAN_DEVICE_H
#define VULKAN_DEVICE_H

#include "vulkan_includes.h"
#include "vulkan_util.h"
#include "swapchain.h"
#include "framebuffer.h"
#include "pipelines/voxel_pipeline.h"
#include "pipelines/particle_pipeline.h"
#include "pipelines/gui_pipeline.h"
#include "view.h"
#include "staging_buffer.h"
#include "texture_allocator.h"
#include "memory_allocator.h"

struct Window;

struct PhysicalDevice {
	VkPhysicalDevice physicalDevice;
	VkPhysicalDeviceProperties properties;
	VkPhysicalDeviceFeatures features;
	VkPhysicalDeviceMemoryProperties memoryProperties;
};

struct QueueFamilies {
	uint32_t index;
	VkQueue queue;
};

struct Synchronization {
	VkSemaphore imageAvailableSemaphore[IMAGE_COUNT];
	VkSemaphore renderFinishedSemaphore[IMAGE_COUNT];
	VkSemaphore updateFinishedSemaphore[IMAGE_COUNT];
	VkSemaphore computeFinishedSemaphore[IMAGE_COUNT];
	VkFence renderFence[IMAGE_COUNT];
	VkFence updateFence[IMAGE_COUNT];
	VkFence computeFence[IMAGE_COUNT];
};

struct CommandBuffers {
	VkCommandPool commandPools[QUEUE_COUNT];
	VkCommandBuffer renderCommandBuffers[IMAGE_COUNT];
	VkCommandBuffer updateCommandBuffers[IMAGE_COUNT];
	VkCommandBuffer computeCommandBuffers[IMAGE_COUNT];
	VkCommandBuffer editorCommandBuffers[IMAGE_COUNT];
};

struct VulkanDevice {
	VkInstance instance;
	VkSurfaceKHR surface;
	struct PhysicalDevice physicalDevice;
	struct QueueFamilies queues[QUEUE_COUNT];
	VkDevice device;
	struct Swapchain swapchain;
	struct CommandBuffers commandBuffers;
	struct Synchronization synchronization;
	struct StagingBuffer stagingBuffer;
	struct Framebuffer framebuffer;
	struct VoxelPipeline voxelPipeline;
	struct ParticlePipeline particlesPipeline;
	struct GuiPipeline guiPipeline;
	VkQueryPool queryPool;

	uint32_t imageIndex;
	int readIndex;
	int writeIndex;
	int bufferWriteIndex;
	int bufferReadIndex;

	struct View view;
	struct MemoryAllocator memoryAllocator;
	struct TextureAllocator textureAllocator;

	void* allocationBuffer;
};

struct VulkanDevice* vulkanDeviceInit(struct Window* window);
void vulkanDeviceDestroy(struct VulkanDevice* vk);

void vulkanDeviceWaitFrame(struct VulkanDevice* vk);
void vulkanDeviceUpdateBegin(struct VulkanDevice* vk);
void vulkanDeviceUpdateEnd(struct VulkanDevice* vk);
void vulkanDeviceRenderBegin(struct VulkanDevice* vk);
void vulkanDeviceRenderEnd(struct VulkanDevice* vk);
void vulkanDeviceEditorBegin(struct VulkanDevice* vk);
void vulkanDeviceEditorEnd(struct VulkanDevice* vk);
void vulkanDevicePresentFrame(struct VulkanDevice* vk);

uint32_t findMemoryType(struct VulkanDevice* vk, uint32_t typeFilter, VkMemoryPropertyFlags properties);
VkFormat findDepthFormat(struct VulkanDevice* vk);

void renderAllocateCommand(struct VulkanDevice* vk, VkCommandBuffer* commandBuffer);
void renderBeginSingleTimeCommand(VkCommandBuffer commandBuffer);
void renderEndSingleTimeCommand(VkCommandBuffer commandBuffer);

void updateBeginSingleTimeCommand(VkCommandBuffer commandBuffer);
void updateEndSingleTimeCommand(VkCommandBuffer commandBuffer);

void reloadPipelines(struct VulkanDevice* vk);
void* getAllocationBuffer(struct VulkanDevice* vk, size_t size);

#endif // VULKAN_DEVICE_H
