#include "vulkan_includes.h"

#define VK_LOAD_INSTANCE_FUNCTION(f) f = (PFN_##f)(vkGetInstanceProcAddr(instance, #f)); 
#define VK_LOAD_DEVICE_FUNCTION(f) f = (PFN_##f)(vkGetDeviceProcAddr(device, #f));

PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR;
PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers;
PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets;
PFN_vkAllocateMemory vkAllocateMemory;
PFN_vkBeginCommandBuffer vkBeginCommandBuffer;
PFN_vkBindBufferMemory2 vkBindBufferMemory2;
PFN_vkBindImageMemory2 vkBindImageMemory2;
PFN_vkCmdBeginRenderPass2 vkCmdBeginRenderPass2;
PFN_vkCmdBeginQuery vkCmdBeginQuery;
PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets;
PFN_vkCmdBindPipeline vkCmdBindPipeline;
PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers;
PFN_vkCmdBlitImage2KHR vkCmdBlitImage2KHR;
PFN_vkCmdCopyBuffer2KHR vkCmdCopyBuffer2KHR;
PFN_vkCmdCopyBufferToImage2KHR vkCmdCopyBufferToImage2KHR;
PFN_vkCmdCopyImage2KHR vkCmdCopyImage2KHR;
PFN_vkCmdCopyImageToBuffer2KHR vkCmdCopyImageToBuffer2KHR;
PFN_vkCmdDispatch vkCmdDispatch;
PFN_vkCmdDispatchIndirect vkCmdDispatchIndirect;
PFN_vkCmdDraw vkCmdDraw;
PFN_vkCmdDrawIndirect vkCmdDrawIndirect;
PFN_vkCmdEndRenderPass2 vkCmdEndRenderPass2;
PFN_vkCmdEndQuery vkCmdEndQuery;
PFN_vkCmdExecuteCommands vkCmdExecuteCommands;
PFN_vkCmdNextSubpass2 vkCmdNextSubpass2;
PFN_vkCmdPipelineBarrier2KHR vkCmdPipelineBarrier2KHR;
PFN_vkCmdPushConstants vkCmdPushConstants;
PFN_vkCmdPushDescriptorSetKHR vkCmdPushDescriptorSetKHR;
PFN_vkCmdPushDescriptorSetWithTemplateKHR vkCmdPushDescriptorSetWithTemplateKHR;
PFN_vkCmdSetViewport vkCmdSetViewport;
PFN_vkCreateBuffer vkCreateBuffer;
PFN_vkCreateCommandPool vkCreateCommandPool;
PFN_vkCreateComputePipelines vkCreateComputePipelines;
PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;
PFN_vkCreateDescriptorPool vkCreateDescriptorPool;
PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout;
PFN_vkCreateDescriptorUpdateTemplate vkCreateDescriptorUpdateTemplate;
PFN_vkCreateDevice vkCreateDevice;
PFN_vkCreateFence vkCreateFence;
PFN_vkCreateFramebuffer vkCreateFramebuffer;
PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines;
PFN_vkCreateImage vkCreateImage;
PFN_vkCreateImageView vkCreateImageView;
PFN_vkCreateInstance vkCreateInstance;
PFN_vkCreatePipelineCache vkCreatePipelineCache;
PFN_vkCreatePipelineLayout vkCreatePipelineLayout;
PFN_vkCreateQueryPool vkCreateQueryPool;
PFN_vkCreateRenderPass2 vkCreateRenderPass2;
PFN_vkCreateSampler vkCreateSampler;
PFN_vkCreateSemaphore vkCreateSemaphore;
PFN_vkCreateShaderModule vkCreateShaderModule;
PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;
PFN_vkCreateValidationCacheEXT vkCreateValidationCacheEXT;
PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR;
PFN_vkDestroyBuffer vkDestroyBuffer;
PFN_vkDestroyCommandPool vkDestroyCommandPool;
PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;
PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool;
PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout;
PFN_vkDestroyDescriptorUpdateTemplate vkDestroyDescriptorUpdateTemplate;
PFN_vkDestroyDevice vkDestroyDevice;
PFN_vkDestroyFence vkDestroyFence;
PFN_vkDestroyFramebuffer vkDestroyFramebuffer;
PFN_vkDestroyImage vkDestroyImage;
PFN_vkDestroyImageView vkDestroyImageView;
PFN_vkDestroyInstance vkDestroyInstance;
PFN_vkDestroyPipeline vkDestroyPipeline;
PFN_vkDestroyPipelineCache vkDestroyPipelineCache;
PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout;
PFN_vkDestroyQueryPool vkDestroyQueryPool;
PFN_vkDestroyRenderPass vkDestroyRenderPass;
PFN_vkDestroySampler vkDestroySampler;
PFN_vkDestroySemaphore vkDestroySemaphore;
PFN_vkDestroyShaderModule vkDestroyShaderModule;
PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR;
PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;
PFN_vkDestroyValidationCacheEXT vkDestroyValidationCacheEXT;
PFN_vkDeviceWaitIdle vkDeviceWaitIdle;
PFN_vkEndCommandBuffer vkEndCommandBuffer;
PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties;
PFN_vkEnumerateDeviceLayerProperties vkEnumerateDeviceLayerProperties;
PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion;
PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
PFN_vkFreeCommandBuffers vkFreeCommandBuffers;
PFN_vkFreeDescriptorSets vkFreeDescriptorSets;
PFN_vkFreeMemory vkFreeMemory;
PFN_vkGetBufferDeviceAddress vkGetBufferDeviceAddress;
PFN_vkGetBufferMemoryRequirements2 vkGetBufferMemoryRequirements2;
PFN_vkGetDeviceMemoryCommitment vkGetDeviceMemoryCommitment;
PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
PFN_vkGetDeviceQueue2 vkGetDeviceQueue2;
PFN_vkGetImageMemoryRequirements2 vkGetImageMemoryRequirements2;
PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
PFN_vkGetPhysicalDeviceFeatures2 vkGetPhysicalDeviceFeatures2;
PFN_vkGetPhysicalDeviceFormatProperties2 vkGetPhysicalDeviceFormatProperties2;
PFN_vkGetPhysicalDeviceImageFormatProperties2 vkGetPhysicalDeviceImageFormatProperties2;
PFN_vkGetPhysicalDeviceMemoryProperties2 vkGetPhysicalDeviceMemoryProperties2;
PFN_vkGetPhysicalDeviceProperties2 vkGetPhysicalDeviceProperties2;
PFN_vkGetPhysicalDeviceQueueFamilyProperties2 vkGetPhysicalDeviceQueueFamilyProperties2;
PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR;
PFN_vkGetPhysicalDeviceSurfacePresentModes2EXT vkGetPhysicalDeviceSurfacePresentModes2EXT;
PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR;
PFN_vkGetPipelineCacheData vkGetPipelineCacheData;
PFN_vkGetSemaphoreCounterValue vkGetSemaphoreCounterValue;
PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
PFN_vkGetValidationCacheDataEXT vkGetValidationCacheDataEXT;
PFN_vkMapMemory vkMapMemory;
PFN_vkQueuePresentKHR vkQueuePresentKHR;
PFN_vkQueueSubmit vkQueueSubmit;
PFN_vkQueueWaitIdle vkQueueWaitIdle;
PFN_vkResetCommandBuffer vkResetCommandBuffer;
PFN_vkResetCommandPool vkResetCommandPool;
PFN_vkResetDescriptorPool vkResetDescriptorPool;
PFN_vkResetEvent vkResetEvent;
PFN_vkResetFences vkResetFences;
PFN_vkSignalSemaphore vkSignalSemaphore;
PFN_vkUnmapMemory vkUnmapMemory;
PFN_vkUpdateDescriptorSetWithTemplate vkUpdateDescriptorSetWithTemplate;
PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets;
PFN_vkWaitForFences vkWaitForFences;
PFN_vkWaitSemaphores vkWaitSemaphores;

void loadInstance() {
	HMODULE handle = LoadLibraryA("vulkan-1.dll");
	vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)GetProcAddress(handle, "vkGetInstanceProcAddr");
	vkGetDeviceProcAddr = (PFN_vkGetDeviceProcAddr)GetProcAddress(handle, "vkGetDeviceProcAddr");
	vkCreateInstance = (PFN_vkCreateInstance)vkGetInstanceProcAddr(NULL, "vkCreateInstance");
}

void loadVulkanInstanceFunctions(VkInstance instance) {
	VK_LOAD_INSTANCE_FUNCTION(vkAcquireNextImageKHR);
	VK_LOAD_INSTANCE_FUNCTION(vkAllocateCommandBuffers);
	VK_LOAD_INSTANCE_FUNCTION(vkAllocateDescriptorSets);
	VK_LOAD_INSTANCE_FUNCTION(vkAllocateMemory);
	VK_LOAD_INSTANCE_FUNCTION(vkBeginCommandBuffer);
	VK_LOAD_INSTANCE_FUNCTION(vkBindBufferMemory2);
	VK_LOAD_INSTANCE_FUNCTION(vkBindImageMemory2);
	VK_LOAD_INSTANCE_FUNCTION(vkCmdBeginRenderPass2);
	VK_LOAD_INSTANCE_FUNCTION(vkCmdBeginQuery);
	VK_LOAD_INSTANCE_FUNCTION(vkCmdBindDescriptorSets);
	VK_LOAD_INSTANCE_FUNCTION(vkCmdBindPipeline);
	VK_LOAD_INSTANCE_FUNCTION(vkCmdBindVertexBuffers);
	VK_LOAD_INSTANCE_FUNCTION(vkCmdBlitImage2KHR);
	VK_LOAD_INSTANCE_FUNCTION(vkCmdCopyBuffer2KHR);
	VK_LOAD_INSTANCE_FUNCTION(vkCmdCopyBufferToImage2KHR);
	VK_LOAD_INSTANCE_FUNCTION(vkCmdCopyImage2KHR);
	VK_LOAD_INSTANCE_FUNCTION(vkCmdCopyImageToBuffer2KHR);
	VK_LOAD_INSTANCE_FUNCTION(vkCmdDispatch);
	VK_LOAD_INSTANCE_FUNCTION(vkCmdDispatchIndirect);
	VK_LOAD_INSTANCE_FUNCTION(vkCmdDraw);
	VK_LOAD_INSTANCE_FUNCTION(vkCmdDrawIndirect);
	VK_LOAD_INSTANCE_FUNCTION(vkCmdEndRenderPass2);
	VK_LOAD_INSTANCE_FUNCTION(vkCmdEndQuery);
	VK_LOAD_INSTANCE_FUNCTION(vkCmdExecuteCommands);
	VK_LOAD_INSTANCE_FUNCTION(vkCmdNextSubpass2);
	VK_LOAD_INSTANCE_FUNCTION(vkCmdPipelineBarrier2KHR);
	VK_LOAD_INSTANCE_FUNCTION(vkCmdPushConstants);
	VK_LOAD_INSTANCE_FUNCTION(vkCmdPushDescriptorSetKHR);
	VK_LOAD_INSTANCE_FUNCTION(vkCmdPushDescriptorSetWithTemplateKHR);
	VK_LOAD_INSTANCE_FUNCTION(vkCmdSetViewport);
	VK_LOAD_INSTANCE_FUNCTION(vkCreateBuffer);
	VK_LOAD_INSTANCE_FUNCTION(vkCreateCommandPool);
	VK_LOAD_INSTANCE_FUNCTION(vkCreateComputePipelines);
	VK_LOAD_INSTANCE_FUNCTION(vkCreateDebugUtilsMessengerEXT);
	VK_LOAD_INSTANCE_FUNCTION(vkCreateDescriptorPool);
	VK_LOAD_INSTANCE_FUNCTION(vkCreateDescriptorSetLayout);
	VK_LOAD_INSTANCE_FUNCTION(vkCreateDescriptorUpdateTemplate);
	VK_LOAD_INSTANCE_FUNCTION(vkCreateDevice);
	VK_LOAD_INSTANCE_FUNCTION(vkCreateFence);
	VK_LOAD_INSTANCE_FUNCTION(vkCreateFramebuffer);
	VK_LOAD_INSTANCE_FUNCTION(vkCreateGraphicsPipelines);
	VK_LOAD_INSTANCE_FUNCTION(vkCreateImage);
	VK_LOAD_INSTANCE_FUNCTION(vkCreateImageView);
	VK_LOAD_INSTANCE_FUNCTION(vkCreateInstance);
	VK_LOAD_INSTANCE_FUNCTION(vkCreatePipelineCache);
	VK_LOAD_INSTANCE_FUNCTION(vkCreatePipelineLayout);
	VK_LOAD_INSTANCE_FUNCTION(vkCreateQueryPool);
	VK_LOAD_INSTANCE_FUNCTION(vkCreateRenderPass2);
	VK_LOAD_INSTANCE_FUNCTION(vkCreateSampler);
	VK_LOAD_INSTANCE_FUNCTION(vkCreateSemaphore);
	VK_LOAD_INSTANCE_FUNCTION(vkCreateShaderModule);
	VK_LOAD_INSTANCE_FUNCTION(vkCreateSwapchainKHR);
	VK_LOAD_INSTANCE_FUNCTION(vkCreateValidationCacheEXT);
	VK_LOAD_INSTANCE_FUNCTION(vkCreateWin32SurfaceKHR);
	VK_LOAD_INSTANCE_FUNCTION(vkDestroyBuffer);
	VK_LOAD_INSTANCE_FUNCTION(vkDestroyCommandPool);
	VK_LOAD_INSTANCE_FUNCTION(vkDestroyDebugUtilsMessengerEXT);
	VK_LOAD_INSTANCE_FUNCTION(vkDestroyDescriptorPool);
	VK_LOAD_INSTANCE_FUNCTION(vkDestroyDescriptorSetLayout);
	VK_LOAD_INSTANCE_FUNCTION(vkDestroyDescriptorUpdateTemplate);
	VK_LOAD_INSTANCE_FUNCTION(vkDestroyDevice);
	VK_LOAD_INSTANCE_FUNCTION(vkDestroyFence);
	VK_LOAD_INSTANCE_FUNCTION(vkDestroyFramebuffer);
	VK_LOAD_INSTANCE_FUNCTION(vkDestroyImage);
	VK_LOAD_INSTANCE_FUNCTION(vkDestroyImageView);
	VK_LOAD_INSTANCE_FUNCTION(vkDestroyInstance);
	VK_LOAD_INSTANCE_FUNCTION(vkDestroyPipeline);
	VK_LOAD_INSTANCE_FUNCTION(vkDestroyPipelineCache);
	VK_LOAD_INSTANCE_FUNCTION(vkDestroyPipelineLayout);
	VK_LOAD_INSTANCE_FUNCTION(vkDestroyQueryPool);
	VK_LOAD_INSTANCE_FUNCTION(vkDestroyRenderPass);
	VK_LOAD_INSTANCE_FUNCTION(vkDestroySampler);
	VK_LOAD_INSTANCE_FUNCTION(vkDestroySemaphore);
	VK_LOAD_INSTANCE_FUNCTION(vkDestroyShaderModule);
	VK_LOAD_INSTANCE_FUNCTION(vkDestroySurfaceKHR);
	VK_LOAD_INSTANCE_FUNCTION(vkDestroySwapchainKHR);
	VK_LOAD_INSTANCE_FUNCTION(vkDestroyValidationCacheEXT);
	VK_LOAD_INSTANCE_FUNCTION(vkDeviceWaitIdle);
	VK_LOAD_INSTANCE_FUNCTION(vkEndCommandBuffer);
	VK_LOAD_INSTANCE_FUNCTION(vkEnumerateDeviceExtensionProperties);
	VK_LOAD_INSTANCE_FUNCTION(vkEnumerateDeviceLayerProperties);
	VK_LOAD_INSTANCE_FUNCTION(vkEnumerateInstanceExtensionProperties);
	VK_LOAD_INSTANCE_FUNCTION(vkEnumerateInstanceLayerProperties);
	VK_LOAD_INSTANCE_FUNCTION(vkEnumerateInstanceVersion);
	VK_LOAD_INSTANCE_FUNCTION(vkEnumeratePhysicalDevices);
	VK_LOAD_INSTANCE_FUNCTION(vkFreeCommandBuffers);
	VK_LOAD_INSTANCE_FUNCTION(vkFreeDescriptorSets);
	VK_LOAD_INSTANCE_FUNCTION(vkFreeMemory);
	VK_LOAD_INSTANCE_FUNCTION(vkGetBufferDeviceAddress);
	VK_LOAD_INSTANCE_FUNCTION(vkGetBufferMemoryRequirements2);
	VK_LOAD_INSTANCE_FUNCTION(vkGetDeviceMemoryCommitment);
	VK_LOAD_INSTANCE_FUNCTION(vkGetDeviceProcAddr);
	VK_LOAD_INSTANCE_FUNCTION(vkGetDeviceQueue2);
	VK_LOAD_INSTANCE_FUNCTION(vkGetImageMemoryRequirements2);
	VK_LOAD_INSTANCE_FUNCTION(vkGetInstanceProcAddr);
	VK_LOAD_INSTANCE_FUNCTION(vkGetPhysicalDeviceFeatures2);
	VK_LOAD_INSTANCE_FUNCTION(vkGetPhysicalDeviceFormatProperties2);
	VK_LOAD_INSTANCE_FUNCTION(vkGetPhysicalDeviceImageFormatProperties2);
	VK_LOAD_INSTANCE_FUNCTION(vkGetPhysicalDeviceMemoryProperties2);
	VK_LOAD_INSTANCE_FUNCTION(vkGetPhysicalDeviceProperties2);
	VK_LOAD_INSTANCE_FUNCTION(vkGetPhysicalDeviceQueueFamilyProperties2);
	VK_LOAD_INSTANCE_FUNCTION(vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
	VK_LOAD_INSTANCE_FUNCTION(vkGetPhysicalDeviceSurfaceFormatsKHR);
	VK_LOAD_INSTANCE_FUNCTION(vkGetPhysicalDeviceSurfacePresentModes2EXT);
	VK_LOAD_INSTANCE_FUNCTION(vkGetPhysicalDeviceSurfaceSupportKHR);
	VK_LOAD_INSTANCE_FUNCTION(vkGetPipelineCacheData);
	VK_LOAD_INSTANCE_FUNCTION(vkGetSemaphoreCounterValue);
	VK_LOAD_INSTANCE_FUNCTION(vkGetSwapchainImagesKHR);
	VK_LOAD_INSTANCE_FUNCTION(vkGetValidationCacheDataEXT);
	VK_LOAD_INSTANCE_FUNCTION(vkMapMemory);
	VK_LOAD_INSTANCE_FUNCTION(vkQueuePresentKHR);
	VK_LOAD_INSTANCE_FUNCTION(vkQueueSubmit);
	VK_LOAD_INSTANCE_FUNCTION(vkQueueWaitIdle);
	VK_LOAD_INSTANCE_FUNCTION(vkResetCommandBuffer);
	VK_LOAD_INSTANCE_FUNCTION(vkResetCommandPool);
	VK_LOAD_INSTANCE_FUNCTION(vkResetDescriptorPool);
	VK_LOAD_INSTANCE_FUNCTION(vkResetEvent);
	VK_LOAD_INSTANCE_FUNCTION(vkResetFences);
	VK_LOAD_INSTANCE_FUNCTION(vkSignalSemaphore);
	VK_LOAD_INSTANCE_FUNCTION(vkUnmapMemory);
	VK_LOAD_INSTANCE_FUNCTION(vkUpdateDescriptorSetWithTemplate);
	VK_LOAD_INSTANCE_FUNCTION(vkUpdateDescriptorSets);
	VK_LOAD_INSTANCE_FUNCTION(vkWaitForFences);
	VK_LOAD_INSTANCE_FUNCTION(vkWaitSemaphores);
}

void loadVulkanDeviceFunctions(VkDevice device) {
	VK_LOAD_DEVICE_FUNCTION(vkAcquireNextImageKHR);
	VK_LOAD_DEVICE_FUNCTION(vkAllocateCommandBuffers);
	VK_LOAD_DEVICE_FUNCTION(vkAllocateDescriptorSets);
	VK_LOAD_DEVICE_FUNCTION(vkAllocateMemory);
	VK_LOAD_DEVICE_FUNCTION(vkBeginCommandBuffer);
	VK_LOAD_DEVICE_FUNCTION(vkBindBufferMemory2);
	VK_LOAD_DEVICE_FUNCTION(vkBindImageMemory2);
	VK_LOAD_DEVICE_FUNCTION(vkCmdBeginRenderPass2);
	VK_LOAD_DEVICE_FUNCTION(vkCmdBeginQuery);
	VK_LOAD_DEVICE_FUNCTION(vkCmdBindDescriptorSets);
	VK_LOAD_DEVICE_FUNCTION(vkCmdBindPipeline);
	VK_LOAD_DEVICE_FUNCTION(vkCmdBindVertexBuffers);
	VK_LOAD_DEVICE_FUNCTION(vkCmdBlitImage2KHR);
	VK_LOAD_DEVICE_FUNCTION(vkCmdCopyBuffer2KHR);
	VK_LOAD_DEVICE_FUNCTION(vkCmdCopyBufferToImage2KHR);
	VK_LOAD_DEVICE_FUNCTION(vkCmdCopyImage2KHR);
	VK_LOAD_DEVICE_FUNCTION(vkCmdCopyImageToBuffer2KHR);
	VK_LOAD_DEVICE_FUNCTION(vkCmdDispatch);
	VK_LOAD_DEVICE_FUNCTION(vkCmdDispatchIndirect);
	VK_LOAD_DEVICE_FUNCTION(vkCmdDraw);
	VK_LOAD_DEVICE_FUNCTION(vkCmdDrawIndirect);
	VK_LOAD_DEVICE_FUNCTION(vkCmdEndRenderPass2);
	VK_LOAD_DEVICE_FUNCTION(vkCmdEndQuery);
	VK_LOAD_DEVICE_FUNCTION(vkCmdExecuteCommands);
	VK_LOAD_DEVICE_FUNCTION(vkCmdNextSubpass2);
	VK_LOAD_DEVICE_FUNCTION(vkCmdPipelineBarrier2KHR);
	VK_LOAD_DEVICE_FUNCTION(vkCmdPushConstants);
	VK_LOAD_DEVICE_FUNCTION(vkCmdPushDescriptorSetKHR);
	VK_LOAD_DEVICE_FUNCTION(vkCmdPushDescriptorSetWithTemplateKHR);
	VK_LOAD_DEVICE_FUNCTION(vkCmdSetViewport);
	VK_LOAD_DEVICE_FUNCTION(vkCreateBuffer);
	VK_LOAD_DEVICE_FUNCTION(vkCreateCommandPool);
	VK_LOAD_DEVICE_FUNCTION(vkCreateComputePipelines);
	VK_LOAD_DEVICE_FUNCTION(vkCreateDebugUtilsMessengerEXT);
	VK_LOAD_DEVICE_FUNCTION(vkCreateDescriptorPool);
	VK_LOAD_DEVICE_FUNCTION(vkCreateDescriptorSetLayout);
	VK_LOAD_DEVICE_FUNCTION(vkCreateDescriptorUpdateTemplate);
	VK_LOAD_DEVICE_FUNCTION(vkCreateDevice);
	VK_LOAD_DEVICE_FUNCTION(vkCreateFence);
	VK_LOAD_DEVICE_FUNCTION(vkCreateFramebuffer);
	VK_LOAD_DEVICE_FUNCTION(vkCreateGraphicsPipelines);
	VK_LOAD_DEVICE_FUNCTION(vkCreateImage);
	VK_LOAD_DEVICE_FUNCTION(vkCreateImageView);
	VK_LOAD_DEVICE_FUNCTION(vkCreateInstance);
	VK_LOAD_DEVICE_FUNCTION(vkCreatePipelineCache);
	VK_LOAD_DEVICE_FUNCTION(vkCreatePipelineLayout);
	VK_LOAD_DEVICE_FUNCTION(vkCreateQueryPool);
	VK_LOAD_DEVICE_FUNCTION(vkCreateRenderPass2);
	VK_LOAD_DEVICE_FUNCTION(vkCreateSampler);
	VK_LOAD_DEVICE_FUNCTION(vkCreateSemaphore);
	VK_LOAD_DEVICE_FUNCTION(vkCreateShaderModule);
	VK_LOAD_DEVICE_FUNCTION(vkCreateSwapchainKHR);
	VK_LOAD_DEVICE_FUNCTION(vkCreateValidationCacheEXT);
	VK_LOAD_DEVICE_FUNCTION(vkDestroyBuffer);
	VK_LOAD_DEVICE_FUNCTION(vkDestroyCommandPool);
	VK_LOAD_DEVICE_FUNCTION(vkDestroyDescriptorPool);
	VK_LOAD_DEVICE_FUNCTION(vkDestroyDescriptorSetLayout);
	VK_LOAD_DEVICE_FUNCTION(vkDestroyDescriptorUpdateTemplate);
	VK_LOAD_DEVICE_FUNCTION(vkDestroyDevice);
	VK_LOAD_DEVICE_FUNCTION(vkDestroyFence);
	VK_LOAD_DEVICE_FUNCTION(vkDestroyFramebuffer);
	VK_LOAD_DEVICE_FUNCTION(vkDestroyImage);
	VK_LOAD_DEVICE_FUNCTION(vkDestroyImageView);
	VK_LOAD_DEVICE_FUNCTION(vkDestroyPipeline);
	VK_LOAD_DEVICE_FUNCTION(vkDestroyPipelineCache);
	VK_LOAD_DEVICE_FUNCTION(vkDestroyPipelineLayout);
	VK_LOAD_DEVICE_FUNCTION(vkDestroyQueryPool);
	VK_LOAD_DEVICE_FUNCTION(vkDestroyRenderPass);
	VK_LOAD_DEVICE_FUNCTION(vkDestroySampler);
	VK_LOAD_DEVICE_FUNCTION(vkDestroySemaphore);
	VK_LOAD_DEVICE_FUNCTION(vkDestroyShaderModule);
	VK_LOAD_DEVICE_FUNCTION(vkDestroyValidationCacheEXT);
	VK_LOAD_DEVICE_FUNCTION(vkDeviceWaitIdle);
	VK_LOAD_DEVICE_FUNCTION(vkEndCommandBuffer);
	VK_LOAD_DEVICE_FUNCTION(vkEnumerateDeviceExtensionProperties);
	VK_LOAD_DEVICE_FUNCTION(vkEnumerateDeviceLayerProperties);
	VK_LOAD_DEVICE_FUNCTION(vkEnumerateInstanceExtensionProperties);
	VK_LOAD_DEVICE_FUNCTION(vkEnumerateInstanceLayerProperties);
	VK_LOAD_DEVICE_FUNCTION(vkEnumerateInstanceVersion);
	VK_LOAD_DEVICE_FUNCTION(vkEnumeratePhysicalDevices);
	VK_LOAD_DEVICE_FUNCTION(vkFreeCommandBuffers);
	VK_LOAD_DEVICE_FUNCTION(vkFreeDescriptorSets);
	VK_LOAD_DEVICE_FUNCTION(vkFreeMemory);
	VK_LOAD_DEVICE_FUNCTION(vkGetBufferDeviceAddress);
	VK_LOAD_DEVICE_FUNCTION(vkGetBufferMemoryRequirements2);
	VK_LOAD_DEVICE_FUNCTION(vkGetDeviceMemoryCommitment);
	VK_LOAD_DEVICE_FUNCTION(vkGetImageMemoryRequirements2);
	VK_LOAD_DEVICE_FUNCTION(vkGetPipelineCacheData);
	VK_LOAD_DEVICE_FUNCTION(vkGetSemaphoreCounterValue);
	VK_LOAD_DEVICE_FUNCTION(vkGetSwapchainImagesKHR);
	VK_LOAD_DEVICE_FUNCTION(vkGetValidationCacheDataEXT);
	VK_LOAD_DEVICE_FUNCTION(vkMapMemory);
	VK_LOAD_DEVICE_FUNCTION(vkQueuePresentKHR);
	VK_LOAD_DEVICE_FUNCTION(vkQueueSubmit);
	VK_LOAD_DEVICE_FUNCTION(vkQueueWaitIdle);
	VK_LOAD_DEVICE_FUNCTION(vkResetCommandBuffer);
	VK_LOAD_DEVICE_FUNCTION(vkResetCommandPool);
	VK_LOAD_DEVICE_FUNCTION(vkResetDescriptorPool);
	VK_LOAD_DEVICE_FUNCTION(vkResetEvent);
	VK_LOAD_DEVICE_FUNCTION(vkResetFences);
	VK_LOAD_DEVICE_FUNCTION(vkSignalSemaphore);
	VK_LOAD_DEVICE_FUNCTION(vkUnmapMemory);
	VK_LOAD_DEVICE_FUNCTION(vkUpdateDescriptorSetWithTemplate);
	VK_LOAD_DEVICE_FUNCTION(vkUpdateDescriptorSets);
	VK_LOAD_DEVICE_FUNCTION(vkWaitForFences);
	VK_LOAD_DEVICE_FUNCTION(vkWaitSemaphores);
}
