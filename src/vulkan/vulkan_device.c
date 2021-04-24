#include "vulkan_device.h"

#include <assert.h>
#include <stdlib.h>

#include "vulkan_debug.h"
#include "vulkan_print.h"
#include "window/window.h"

void createInstance(struct VulkanDevice* vk) {
	loadInstance();

	VkApplicationInfo applicationInfo;
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pNext = NULL;
	applicationInfo.pApplicationName = "Nobody";
	applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	applicationInfo.pEngineName = "Nobody Engine";
	applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	applicationInfo.apiVersion = VK_API_VERSION_1_2;

	VkInstanceCreateInfo instanceCreateInfo;
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pNext = NULL;
	instanceCreateInfo.flags = 0;
	instanceCreateInfo.pApplicationInfo = &applicationInfo;
#ifdef USE_VALIDATION_LAYERS
	instanceCreateInfo.enabledLayerCount = 1;
	instanceCreateInfo.ppEnabledLayerNames = validationLayers;
#else
	instanceCreateInfo.enabledLayerCount = 0;
	instanceCreateInfo.ppEnabledLayerNames = NULL;
#endif
	instanceCreateInfo.enabledExtensionCount = ARRAY_SIZE(instanceExtensions);
	instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions;

	VkValidationFeaturesEXT validationFeatures;
	validationFeatures.sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT;
	validationFeatures.pNext = NULL;
#ifdef USE_VALIDATION_LAYERS
	validationFeatures.enabledValidationFeatureCount = ARRAY_SIZE(validationFeaturesEnable);
	validationFeatures.pEnabledValidationFeatures = validationFeaturesEnable;
	validationFeatures.disabledValidationFeatureCount = 0;
	validationFeatures.pDisabledValidationFeatures = NULL;
#else
	validationFeatures.enabledValidationFeatureCount = 0;
	validationFeatures.pEnabledValidationFeatures = NULL;
	validationFeatures.disabledValidationFeatureCount = ARRAY_SIZE(validationFeaturesDisable);
	validationFeatures.pDisabledValidationFeatures = validationFeaturesDisable;
#endif

	VkValidationCheckEXT validationCheckAll = VK_VALIDATION_CHECK_ALL_EXT;
	VkValidationFlagsEXT validationFlags;
	validationFlags.sType = VK_STRUCTURE_TYPE_VALIDATION_FLAGS_EXT;
	validationFlags.pNext = NULL;
	validationFlags.disabledValidationCheckCount = 1;
	validationFlags.pDisabledValidationChecks = &validationCheckAll;

	instanceCreateInfo.pNext = &validationFeatures;
#ifndef USE_VALIDATION_LAYERS
	validationFeatures.pNext = &validationFlags;
#endif
	VK_ASSERT(vkCreateInstance(&instanceCreateInfo, NULL, &vk->instance));

	loadVulkanInstanceFunctions(vk->instance);

	printInstanceExtensions(vk);
}

void createSurface(struct VulkanDevice* vk, struct Window* window) {
	VkWin32SurfaceCreateInfoKHR surfaceCreateInfo;
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.pNext = NULL;
	surfaceCreateInfo.flags = 0;
	surfaceCreateInfo.hinstance = window->hInstance;
	surfaceCreateInfo.hwnd = window->hwnd;

	VK_ASSERT(vkCreateWin32SurfaceKHR(vk->instance, &surfaceCreateInfo, NULL, &vk->surface));
}

uint32_t findQueueFamily(struct VulkanDevice* vk, VkPhysicalDevice physicalDevice, uint32_t count, VkQueueFamilyProperties2* properties, uint32_t index) {
	uint32_t family = UINT32_MAX;
	for (uint32_t i = 0; i < count; ++i) {
		if (index == GRAPHICS_INDEX) {
			if (properties[i].queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				VkBool32 presentSupported;
				vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, vk->surface, &presentSupported);
				if (presentSupported) {
					family = i;
					break;
				}
			}
		} else if (index == COMPUTE_INDEX) {
			if (properties[i].queueFamilyProperties.queueFlags & VK_QUEUE_COMPUTE_BIT) {
				family = i;
				if ((properties[i].queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) {
					break;
				}
			}
		} else if (index == TRANSFER_INDEX) {
			if (properties[i].queueFamilyProperties.queueFlags & VK_QUEUE_TRANSFER_BIT) {
				family = i;
				if ((properties[i].queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0 &&
					(properties[i].queueFamilyProperties.queueFlags & VK_QUEUE_COMPUTE_BIT) == 0) {
					break;
				}
			}
		}
	}

	assert(family != UINT32_MAX);
	return family;
}

void selectPhysicalDevice(struct VulkanDevice* vk) {
	uint32_t physicalDeviceCount;
	VK_ASSERT(vkEnumeratePhysicalDevices(vk->instance, &physicalDeviceCount, NULL));
	VkPhysicalDevice* physicalDevices = getAllocationBuffer(vk, sizeof(VkPhysicalDevice) * physicalDeviceCount);
	VK_ASSERT(vkEnumeratePhysicalDevices(vk->instance, &physicalDeviceCount, physicalDevices));

	VkPhysicalDevice pd = VK_NULL_HANDLE;
	for (uint32_t i = 0; i < physicalDeviceCount; ++i) {
		pd = physicalDevices[i];
		break;
	}
	assert(pd != VK_NULL_HANDLE);
	vk->physicalDevice.physicalDevice = pd;

	uint32_t queueFamilyPropertyCount;
	vkGetPhysicalDeviceQueueFamilyProperties2(vk->physicalDevice.physicalDevice, &queueFamilyPropertyCount, NULL);
	VkQueueFamilyProperties2* queueFamilyProperties = getAllocationBuffer(vk, sizeof(VkQueueFamilyProperties2) * queueFamilyPropertyCount);
	for (uint32_t i = 0; i < queueFamilyPropertyCount; ++i) {
		queueFamilyProperties[i].sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2;
		queueFamilyProperties[i].pNext = NULL;
		queueFamilyProperties[i].queueFamilyProperties.queueFlags = 0;
		queueFamilyProperties[i].queueFamilyProperties.queueCount = 0;
		queueFamilyProperties[i].queueFamilyProperties.timestampValidBits = 0;
		queueFamilyProperties[i].queueFamilyProperties.minImageTransferGranularity.width = 0;
		queueFamilyProperties[i].queueFamilyProperties.minImageTransferGranularity.height = 0;
		queueFamilyProperties[i].queueFamilyProperties.minImageTransferGranularity.depth = 0;
	}
	vkGetPhysicalDeviceQueueFamilyProperties2(vk->physicalDevice.physicalDevice, &queueFamilyPropertyCount, queueFamilyProperties);

	vk->queues[GRAPHICS_INDEX].index = findQueueFamily(vk, pd, queueFamilyPropertyCount, queueFamilyProperties, GRAPHICS_INDEX);
	vk->queues[COMPUTE_INDEX].index = findQueueFamily(vk, pd, queueFamilyPropertyCount, queueFamilyProperties, COMPUTE_INDEX);
	vk->queues[TRANSFER_INDEX].index = findQueueFamily(vk, pd, queueFamilyPropertyCount, queueFamilyProperties, TRANSFER_INDEX);
	assert(vk->queues[GRAPHICS_INDEX].index != vk->queues[COMPUTE_INDEX].index);
	assert(vk->queues[GRAPHICS_INDEX].index != vk->queues[TRANSFER_INDEX].index);
	assert(vk->queues[COMPUTE_INDEX].index != vk->queues[TRANSFER_INDEX].index);

	VkPhysicalDeviceFloatControlsProperties floatControls;
	floatControls.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FLOAT_CONTROLS_PROPERTIES;
	floatControls.pNext = NULL;

	VkPhysicalDevicePCIBusInfoPropertiesEXT busInfoProperties;
	busInfoProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PCI_BUS_INFO_PROPERTIES_EXT;
	busInfoProperties.pNext = &floatControls;

	VkPhysicalDeviceDriverProperties driverProperties;
	driverProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DRIVER_PROPERTIES;
	driverProperties.pNext = &busInfoProperties;

	VkPhysicalDeviceProperties2 physicalDeviceProperties2;
	physicalDeviceProperties2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
	physicalDeviceProperties2.pNext = &driverProperties;
	vkGetPhysicalDeviceProperties2(pd, &physicalDeviceProperties2);
	vk->physicalDevice.properties = physicalDeviceProperties2.properties;

	VkPhysicalDeviceFeatures2 physicalDeviceFeatures;
	physicalDeviceFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
	physicalDeviceFeatures.pNext = NULL;
	vkGetPhysicalDeviceFeatures2(pd, &physicalDeviceFeatures);
	vk->physicalDevice.features = physicalDeviceFeatures.features;

	VkPhysicalDeviceMemoryProperties2 memoryProperties;
	memoryProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2;
	memoryProperties.pNext = NULL;
	vkGetPhysicalDeviceMemoryProperties2(pd, &memoryProperties);
	vk->physicalDevice.memoryProperties = memoryProperties.memoryProperties;

	printDeviceExtensions(vk);
	printPhysicalDeviceInfo(vk);
	printDriverInfo(&driverProperties);
	printPCIBusInfo(&busInfoProperties);
}

void createDevice(struct VulkanDevice* vk) {
	float queuePriorities[] = { 1.0f, 0.9f, 0.8f };

	VkDeviceQueueCreateInfo queueCreateInfos[QUEUE_COUNT];
	for (uint32_t i = 0; i < QUEUE_COUNT; ++i) {
		queueCreateInfos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfos[i].pNext = NULL;
		queueCreateInfos[i].flags = 0;
		queueCreateInfos[i].queueFamilyIndex = vk->queues[i].index;
		queueCreateInfos[i].queueCount = 1;
		queueCreateInfos[i].pQueuePriorities = &queuePriorities[i];
	}

	VkPhysicalDeviceSynchronization2FeaturesKHR synchronization2Features;
	synchronization2Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES_KHR;
	synchronization2Features.pNext = NULL;
	synchronization2Features.synchronization2 = VK_TRUE;

	VkPhysicalDeviceVulkan12Features deviceEnabledFeatures12 = { 0 };
	deviceEnabledFeatures12.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
	deviceEnabledFeatures12.pNext = &synchronization2Features;
	deviceEnabledFeatures12.timelineSemaphore = VK_TRUE;
	deviceEnabledFeatures12.bufferDeviceAddress = VK_TRUE;

	VkPhysicalDeviceFeatures2 deviceEnabledFeatures = { 0 };
	deviceEnabledFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR;
	deviceEnabledFeatures.pNext = &deviceEnabledFeatures12;

	deviceEnabledFeatures.features.fillModeNonSolid = VK_TRUE;
	deviceEnabledFeatures.features.depthBounds = VK_TRUE;
	deviceEnabledFeatures.features.robustBufferAccess = VK_FALSE;

	VkDeviceCreateInfo deviceCreateInfo;
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.pNext = &deviceEnabledFeatures;
	deviceCreateInfo.flags = 0;
	deviceCreateInfo.queueCreateInfoCount = ARRAY_SIZE(queueCreateInfos);
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos;
	deviceCreateInfo.enabledLayerCount = 0;
	deviceCreateInfo.ppEnabledLayerNames = NULL;
	deviceCreateInfo.enabledExtensionCount = ARRAY_SIZE(deviceExtensions);
	deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions;
	deviceCreateInfo.pEnabledFeatures = NULL;

	VK_ASSERT(vkCreateDevice(vk->physicalDevice.physicalDevice, &deviceCreateInfo, NULL, &vk->device));

	loadVulkanDeviceFunctions(vk->device);

	for (int i = 0; i < QUEUE_COUNT; ++i) {
		VkDeviceQueueInfo2 graphicsQueueInfo;
		graphicsQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2;
		graphicsQueueInfo.pNext = NULL;
		graphicsQueueInfo.flags = 0;
		graphicsQueueInfo.queueFamilyIndex = vk->queues[i].index;
		graphicsQueueInfo.queueIndex = 0;
		vkGetDeviceQueue2(vk->device, &graphicsQueueInfo, &vk->queues[i].queue);
	}
}

void createPools(struct VulkanDevice* vk) {
	for (int i = 0; i < QUEUE_COUNT; ++i) {
		VkCommandPoolCreateInfo commandPoolCreateInfo;
		commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolCreateInfo.pNext = NULL;
		commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		commandPoolCreateInfo.queueFamilyIndex = vk->queues[i].index;
		VK_ASSERT(vkCreateCommandPool(vk->device, &commandPoolCreateInfo, NULL, &vk->commandBuffers.commandPools[i]));
	}

	VkCommandBufferAllocateInfo graphicsCommandBufferAllocateInfo;
	graphicsCommandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	graphicsCommandBufferAllocateInfo.pNext = NULL;
	graphicsCommandBufferAllocateInfo.commandPool = vk->commandBuffers.commandPools[GRAPHICS_INDEX];
	graphicsCommandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	graphicsCommandBufferAllocateInfo.commandBufferCount = IMAGE_COUNT;

	VK_ASSERT(vkAllocateCommandBuffers(vk->device, &graphicsCommandBufferAllocateInfo, vk->commandBuffers.renderCommandBuffers));
	VK_ASSERT(vkAllocateCommandBuffers(vk->device, &graphicsCommandBufferAllocateInfo, vk->commandBuffers.editorCommandBuffers));

	VkCommandBufferAllocateInfo updateCommandBufferAllocateInfo;
	updateCommandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	updateCommandBufferAllocateInfo.pNext = NULL;
	updateCommandBufferAllocateInfo.commandPool = vk->commandBuffers.commandPools[TRANSFER_INDEX];
	updateCommandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	updateCommandBufferAllocateInfo.commandBufferCount = IMAGE_COUNT;

	VK_ASSERT(vkAllocateCommandBuffers(vk->device, &updateCommandBufferAllocateInfo, vk->commandBuffers.updateCommandBuffers));

	VkCommandBufferAllocateInfo computeCommandBufferAllocateInfo;
	computeCommandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	computeCommandBufferAllocateInfo.pNext = NULL;
	computeCommandBufferAllocateInfo.commandPool = vk->commandBuffers.commandPools[COMPUTE_INDEX];
	computeCommandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	computeCommandBufferAllocateInfo.commandBufferCount = IMAGE_COUNT;

	VK_ASSERT(vkAllocateCommandBuffers(vk->device, &computeCommandBufferAllocateInfo, vk->commandBuffers.computeCommandBuffers));

	VkQueryPoolCreateInfo queryPoolCreateInfo;
	queryPoolCreateInfo.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
	queryPoolCreateInfo.pNext = NULL;
	queryPoolCreateInfo.flags = 0;
	queryPoolCreateInfo.queryType = VK_QUERY_TYPE_TIMESTAMP;
	queryPoolCreateInfo.queryCount = 1;
	queryPoolCreateInfo.pipelineStatistics = 0;

	vkCreateQueryPool(vk->device, &queryPoolCreateInfo, NULL, &vk->queryPool);
}

VkSemaphore createSemaphore(struct VulkanDevice* vk, bool timeline) {
	VkSemaphoreTypeCreateInfoKHR semaphoreTypeCreateInfo;
	semaphoreTypeCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO;
	semaphoreTypeCreateInfo.pNext = NULL;
	if (timeline) {
		semaphoreTypeCreateInfo.semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE;
	} else {
		semaphoreTypeCreateInfo.semaphoreType = VK_SEMAPHORE_TYPE_BINARY;
	}
	semaphoreTypeCreateInfo.initialValue = 0;

	VkSemaphoreCreateInfo semaphoreCreateInfo;
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphoreCreateInfo.pNext = &semaphoreTypeCreateInfo;
	semaphoreCreateInfo.flags = 0;

	VkSemaphore semaphore;
	VK_ASSERT(vkCreateSemaphore(vk->device, &semaphoreCreateInfo, NULL, &semaphore));
	return semaphore;
}

void createSyncObjects(struct VulkanDevice* vk) {
	for (int i = 0; i < IMAGE_COUNT; ++i) {
		vk->synchronization.imageAvailableSemaphore[i] = createSemaphore(vk, false);
		vk->synchronization.renderFinishedSemaphore[i] = createSemaphore(vk, false);
		vk->synchronization.updateFinishedSemaphore[i] = createSemaphore(vk, false);
		vk->synchronization.computeFinishedSemaphore[i] = createSemaphore(vk, false);
		vk->synchronization.renderFence[i] = createFence(vk);
		vk->synchronization.updateFence[i] = createFence(vk);
		vk->synchronization.computeFence[i] = createFence(vk);
	}
}

//////////////////////////////////////////// END INIT ////////////////////////////////////////////////
////////////////////////////////////////// BEGIN UPDATE //////////////////////////////////////////////

void vulkanDeviceBeginCompute(struct VulkanDevice* vk) {

}

void vulkanDeviceEndCompute(struct VulkanDevice* vk) {

}

/////////////////////////////////////////// END UPDATE ///////////////////////////////////////////////
////////////////////////////////////////// BEGIN RENDER //////////////////////////////////////////////

void vulkanDeviceBeginFrame(struct VulkanDevice* vk) {
	VK_ASSERT(vkAcquireNextImageKHR(vk->device, vk->swapchain.swapchain, UINT64_MAX, vk->synchronization.imageAvailableSemaphore[vk->bufferReadIndex], VK_NULL_HANDLE, &vk->imageIndex));

	renderBeginSingleTimeCommand(vk->commandBuffers.renderCommandBuffers[vk->bufferReadIndex]);
	stagingBufferLoad(&vk->stagingBuffer, vk->commandBuffers.renderCommandBuffers[vk->bufferReadIndex]);
	framebufferBeginRenderPass(&vk->framebuffer, vk, vk->commandBuffers.renderCommandBuffers[vk->bufferReadIndex]);
}

void vulkanDeviceEndFrame(struct VulkanDevice* vk) {
	framebufferEndRenderPass(&vk->framebuffer, vk->commandBuffers.renderCommandBuffers[vk->bufferReadIndex]);
	renderEndSingleTimeCommand(vk->commandBuffers.renderCommandBuffers[vk->bufferReadIndex]);
}

/////////////////////////////////////////// END RENDER ///////////////////////////////////////////////

struct VulkanDevice* vulkanDeviceInit(struct Window* window) {
	struct VulkanDevice* vk = malloc(sizeof(struct VulkanDevice));
	assert(vk);

	vk->allocationBuffer = malloc(QUERY_BUFFER_SIZE);
	vk->imageIndex = 0;
	createInstance(vk);
	setupDebugCallback(vk->instance);
	createSurface(vk, window);
	selectPhysicalDevice(vk);
	createDevice(vk);
	swapchainInit(&vk->swapchain, vk);
	createPools(vk);
	createSyncObjects(vk);
	memoryAllocatorInit(&vk->memoryAllocator, vk);
	textureAllocatorInit(&vk->textureAllocator, vk);
	stagingBufferInit(&vk->stagingBuffer, vk);
	framebufferInit(&vk->framebuffer, vk);
	viewInit(&vk->view, vk, window);
	voxelPipelineInit(&vk->voxelPipeline, vk);
	particlePipelineInit(&vk->particlesPipeline, vk);
	guiPipelineInit(&vk->guiPipeline, vk);
	return vk;
}

void vulkanDeviceDestroy(struct VulkanDevice* vk) {
	free(vk->allocationBuffer);
	viewDestroy(&vk->view, vk);

	voxelPipelineDestroy(&vk->voxelPipeline);
	particlePipelineDestroy(&vk->particlesPipeline);
	guiPipelineDestroy(&vk->guiPipeline);
	framebufferDestroy(&vk->framebuffer, vk);
	stagingBufferDestroy(&vk->stagingBuffer);

	textureAllocatorDestroy(&vk->textureAllocator, vk);
	memoryAllocatorDestroy(&vk->memoryAllocator, vk);

	for (int i = 0; i < IMAGE_COUNT; ++i) {
		vkDestroyFence(vk->device, vk->synchronization.computeFence[i], NULL);
		vkDestroyFence(vk->device, vk->synchronization.updateFence[i], NULL);
		vkDestroyFence(vk->device, vk->synchronization.renderFence[i], NULL);
		vkDestroySemaphore(vk->device, vk->synchronization.computeFinishedSemaphore[i], NULL);
		vkDestroySemaphore(vk->device, vk->synchronization.updateFinishedSemaphore[i], NULL);
		vkDestroySemaphore(vk->device, vk->synchronization.renderFinishedSemaphore[i], NULL);
		vkDestroySemaphore(vk->device, vk->synchronization.imageAvailableSemaphore[i], NULL);
	}
	vkDestroyQueryPool(vk->device, vk->queryPool, NULL);
	for (uint32_t i = 0; i < QUEUE_COUNT; ++i) {
		vkDestroyCommandPool(vk->device, vk->commandBuffers.commandPools[i], NULL);
	}
	swapchainDestroy(&vk->swapchain, vk);
	vkDestroyDevice(vk->device, NULL);
	vkDestroySurfaceKHR(vk->instance, vk->surface, NULL);
	destroyDebugCallback(vk->instance);
	vkDestroyInstance(vk->instance, NULL);

	free(vk);
}

void vulkanDeviceWaitFrame(struct VulkanDevice* vk) {
	VkFence fences[3];
	fences[0] = vk->synchronization.renderFence[vk->bufferReadIndex];
	fences[1] = vk->synchronization.computeFence[vk->bufferWriteIndex];
	fences[2] = vk->synchronization.updateFence[vk->bufferWriteIndex];
	VK_ASSERT(vkWaitForFences(vk->device, ARRAY_SIZE(fences), fences, VK_TRUE, UINT64_MAX));
}

void vulkanDeviceUpdateBegin(struct VulkanDevice* vk) {
	viewBufferUpdate(&vk->view, vk->bufferWriteIndex);

	updateBeginSingleTimeCommand(vk->commandBuffers.updateCommandBuffers[vk->bufferWriteIndex]);
	updateBeginSingleTimeCommand(vk->commandBuffers.computeCommandBuffers[vk->bufferWriteIndex]);
}

void vulkanDeviceUpdateEnd(struct VulkanDevice* vk) {
	stagingBufferSubmit(&vk->stagingBuffer, vk->commandBuffers.updateCommandBuffers[vk->bufferWriteIndex]);

	updateEndSingleTimeCommand(vk->commandBuffers.updateCommandBuffers[vk->bufferWriteIndex]);
	updateEndSingleTimeCommand(vk->commandBuffers.computeCommandBuffers[vk->bufferWriteIndex]);

	VkSubmitInfo updateSubmitInfo;
	updateSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	updateSubmitInfo.pNext = NULL;
	updateSubmitInfo.waitSemaphoreCount = 0;
	updateSubmitInfo.pWaitDstStageMask = NULL;
	updateSubmitInfo.pWaitSemaphores = NULL;
	updateSubmitInfo.commandBufferCount = 1;
	updateSubmitInfo.pCommandBuffers = &vk->commandBuffers.updateCommandBuffers[vk->bufferWriteIndex];
	updateSubmitInfo.signalSemaphoreCount = 1;
	updateSubmitInfo.pSignalSemaphores = &vk->synchronization.updateFinishedSemaphore[vk->bufferWriteIndex];

	vkResetFences(vk->device, 1, &vk->synchronization.updateFence[vk->bufferWriteIndex]);
	VK_ASSERT(vkQueueSubmit(vk->queues[TRANSFER_INDEX].queue, 1, &updateSubmitInfo, vk->synchronization.updateFence[vk->bufferWriteIndex]));

	VkSubmitInfo computeSubmitInfo;
	computeSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	computeSubmitInfo.pNext = NULL;
	computeSubmitInfo.waitSemaphoreCount = 0;
	computeSubmitInfo.pWaitDstStageMask = NULL;
	computeSubmitInfo.pWaitSemaphores = NULL;
	computeSubmitInfo.commandBufferCount = 1;
	computeSubmitInfo.pCommandBuffers = &vk->commandBuffers.computeCommandBuffers[vk->bufferWriteIndex];
	computeSubmitInfo.signalSemaphoreCount = 1;
	computeSubmitInfo.pSignalSemaphores = &vk->synchronization.computeFinishedSemaphore[vk->bufferWriteIndex];

	vkResetFences(vk->device, 1, &vk->synchronization.computeFence[vk->bufferWriteIndex]);
	VK_ASSERT(vkQueueSubmit(vk->queues[COMPUTE_INDEX].queue, 1, &computeSubmitInfo, vk->synchronization.computeFence[vk->bufferWriteIndex]));
 }

void vulkanDeviceRenderBegin(struct VulkanDevice* vk) {
	vulkanDeviceBeginFrame(vk);
	viewBind(&vk->view, vk, vk->commandBuffers.renderCommandBuffers[vk->bufferReadIndex]);
}

void vulkanDeviceRenderEnd(struct VulkanDevice* vk) {
	vulkanDeviceEndFrame(vk);
}

void vulkanDeviceEditorBegin(struct VulkanDevice* vk) {
	renderBeginSingleTimeCommand(vk->commandBuffers.editorCommandBuffers[vk->bufferReadIndex]);
	framebufferBeginGuiRenderPass(&vk->framebuffer, vk, vk->commandBuffers.editorCommandBuffers[vk->bufferReadIndex]);
}

void vulkanDeviceEditorEnd(struct VulkanDevice* vk) {
	framebufferEndRenderPass(&vk->framebuffer, vk->commandBuffers.editorCommandBuffers[vk->bufferReadIndex]);
	renderEndSingleTimeCommand(vk->commandBuffers.editorCommandBuffers[vk->bufferReadIndex]);
}

void vulkanDevicePresentFrame(struct VulkanDevice* vk) {
	VkPipelineStageFlags waitStages[3];
	waitStages[0] = VK_PIPELINE_STAGE_TRANSFER_BIT;
	waitStages[1] = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
	waitStages[2] = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	VkSemaphore waitSemaphores[3];
	waitSemaphores[0] = vk->synchronization.updateFinishedSemaphore[vk->bufferReadIndex];
	waitSemaphores[1] = vk->synchronization.computeFinishedSemaphore[vk->bufferReadIndex];
	waitSemaphores[2] = vk->synchronization.imageAvailableSemaphore[vk->bufferReadIndex];

	VkSemaphore signalSemaphores[1];
	signalSemaphores[0] = vk->synchronization.renderFinishedSemaphore[vk->bufferReadIndex];

#ifdef EDITOR
	VkCommandBuffer commandBuffers[2];
	commandBuffers[0] = vk->commandBuffers.renderCommandBuffers[vk->bufferReadIndex];
	commandBuffers[1] = vk->commandBuffers.editorCommandBuffers[vk->bufferReadIndex];
#else
	VkCommandBuffer commandBuffers[1];
	commandBuffers[0] = vk->renderCommandBuffers[vk->bufferReadIndex];
#endif

	VkSubmitInfo submitInfo;
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = NULL;
	submitInfo.waitSemaphoreCount = ARRAY_SIZE(waitSemaphores);
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.commandBufferCount = ARRAY_SIZE(commandBuffers);
	submitInfo.pCommandBuffers = commandBuffers;
	submitInfo.signalSemaphoreCount = ARRAY_SIZE(signalSemaphores);
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(vk->device, 1, &vk->synchronization.renderFence[vk->bufferReadIndex]);
	VK_ASSERT(vkQueueSubmit(vk->queues[GRAPHICS_INDEX].queue, 1, &submitInfo, vk->synchronization.renderFence[vk->bufferReadIndex]));

	VkPresentInfoKHR presentInfo;
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pNext = NULL;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &vk->synchronization.renderFinishedSemaphore[vk->bufferReadIndex];
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &vk->swapchain.swapchain;
	presentInfo.pImageIndices = &vk->imageIndex;
	presentInfo.pResults = NULL;

	VK_ASSERT(vkQueuePresentKHR(vk->queues[GRAPHICS_INDEX].queue, &presentInfo));
}

uint32_t findMemoryType(struct VulkanDevice* vk, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
	for (uint32_t i = 0; i < vk->physicalDevice.memoryProperties.memoryTypeCount; ++i) {
		if ((typeFilter & (1 << i)) && (vk->physicalDevice.memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}
	return UINT32_MAX;
}

VkFormat findDepthFormat(struct VulkanDevice* vk) {
	VkFormat candidates[] = {
		VK_FORMAT_D32_SFLOAT,
		VK_FORMAT_D32_SFLOAT_S8_UINT,
		VK_FORMAT_D24_UNORM_S8_UINT
	};
	VkFormatFeatureFlags features = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;

	for (int i = 0; i < ARRAY_SIZE(candidates); ++i) {
		VkFormatProperties2 props;
		props.sType = VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2;
		props.pNext = NULL;
		vkGetPhysicalDeviceFormatProperties2(vk->physicalDevice.physicalDevice, candidates[i], &props);

		if ((props.formatProperties.optimalTilingFeatures & features) == features) {
			return candidates[i];
			break;
		}
	}
	return VK_FORMAT_D24_UNORM_S8_UINT;
}

void renderAllocateCommand(struct VulkanDevice* vk, VkCommandBuffer* commandBuffer) {
	VkCommandBufferAllocateInfo commandBufferAllocateInfo;
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.pNext = NULL;
	commandBufferAllocateInfo.commandPool = vk->commandBuffers.commandPools[GRAPHICS_INDEX];
	commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	commandBufferAllocateInfo.commandBufferCount = 1;

	VK_ASSERT(vkAllocateCommandBuffers(vk->device, &commandBufferAllocateInfo, commandBuffer));
}

void renderBeginSingleTimeCommand(VkCommandBuffer commandBuffer) {
	VkCommandBufferBeginInfo commandBufferBeginInfo;
	commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	commandBufferBeginInfo.pNext = NULL;
	commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	commandBufferBeginInfo.pInheritanceInfo = NULL;

	VK_ASSERT(vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo));
}

void renderEndSingleTimeCommand(VkCommandBuffer commandBuffer) {
	vkEndCommandBuffer(commandBuffer);
}

void updateBeginSingleTimeCommand(VkCommandBuffer commandBuffer) {
	VkCommandBufferBeginInfo commandBufferBeginInfo;
	commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	commandBufferBeginInfo.pNext = NULL;
	commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	commandBufferBeginInfo.pInheritanceInfo = NULL;

	VK_ASSERT(vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo));
}

void updateEndSingleTimeCommand(VkCommandBuffer commandBuffer) {
	vkEndCommandBuffer(commandBuffer);
}

void reloadPipelines(struct VulkanDevice* vk) {
	vkDeviceWaitIdle(vk->device);
	voxelPipelineReload(&vk->voxelPipeline);
	particlePipelineReload(&vk->particlesPipeline);
	guiPipelineReload(&vk->guiPipeline);
}

void* getAllocationBuffer(struct VulkanDevice* vk, size_t size) {
	assert(size < QUERY_BUFFER_SIZE);
	return vk->allocationBuffer;
}
