#include "vulkan_util.h"

#include "vulkan_device.h"
#include "vulkan_debug.h"

#include <stdio.h>
#include <stdlib.h>

VkShaderModule createShaderModule(struct VulkanDevice* vk, const char* shader_name) {
	FILE* pFile = fopen(shader_name, "rb");
	fseek(pFile, 0L, SEEK_END);
	size_t size = ftell(pFile);
	fseek(pFile, 0L, SEEK_SET);
	uint8_t* buffer = malloc(size);
	assert(buffer);
	if (pFile != NULL) {
		size_t counter = 0;
		do {
			buffer[counter] = fgetc(pFile);
			counter++;
		} while (counter < size);
		fclose(pFile);
	}

	VkShaderModuleCreateInfo shaderModuleCreateInfo;
	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleCreateInfo.pNext = NULL;
	shaderModuleCreateInfo.flags = 0;
	shaderModuleCreateInfo.codeSize = size;
	shaderModuleCreateInfo.pCode = (uint32_t*)buffer;

	VkShaderModule shaderModule;
	VK_ASSERT(vkCreateShaderModule(vk->device, &shaderModuleCreateInfo, NULL, &shaderModule));
	free(buffer);

	return shaderModule;
}

void createBufferAndMemory(struct VulkanDevice* vk, VkBuffer* buffer, VkDeviceMemory* memory, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) {
	VkBufferCreateInfo bufferCreateInfo;
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.pNext = NULL;
	bufferCreateInfo.flags = 0;
	bufferCreateInfo.size = size;
	bufferCreateInfo.usage = usage;
	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	bufferCreateInfo.queueFamilyIndexCount = 0;
	bufferCreateInfo.pQueueFamilyIndices = NULL;

	VK_ASSERT(vkCreateBuffer(vk->device, &bufferCreateInfo, NULL, buffer));

	VkBufferMemoryRequirementsInfo2 bufferMemoryRequirementsInfo;
	bufferMemoryRequirementsInfo.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_REQUIREMENTS_INFO_2;
	bufferMemoryRequirementsInfo.pNext = NULL;
	bufferMemoryRequirementsInfo.buffer = *buffer;

	VkMemoryDedicatedRequirements memoryDedicatedRequirements;
	memoryDedicatedRequirements.sType = VK_STRUCTURE_TYPE_MEMORY_DEDICATED_REQUIREMENTS;
	memoryDedicatedRequirements.pNext = NULL;
	memoryDedicatedRequirements.prefersDedicatedAllocation = VK_FALSE;
	memoryDedicatedRequirements.requiresDedicatedAllocation = VK_FALSE;

	VkMemoryRequirements2 memoryRequirements;
	memoryRequirements.sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2;
	memoryRequirements.pNext = &memoryDedicatedRequirements;

	vkGetBufferMemoryRequirements2(vk->device, &bufferMemoryRequirementsInfo, &memoryRequirements);

	uint32_t memoryType = findMemoryType(vk, memoryRequirements.memoryRequirements.memoryTypeBits, properties);
	assert(memoryType != UINT32_MAX);

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
	memoryAllocateInfo.allocationSize = memoryRequirements.memoryRequirements.size;
	memoryAllocateInfo.memoryTypeIndex = memoryType;

	VK_ASSERT(vkAllocateMemory(vk->device, &memoryAllocateInfo, NULL, memory));

	VkBindBufferMemoryInfo bindBUfferMemoryInfo;
	bindBUfferMemoryInfo.sType = VK_STRUCTURE_TYPE_BIND_BUFFER_MEMORY_INFO;
	bindBUfferMemoryInfo.pNext = NULL;
	bindBUfferMemoryInfo.buffer = *buffer;
	bindBUfferMemoryInfo.memory = *memory;
	bindBUfferMemoryInfo.memoryOffset = 0;

	VK_ASSERT(vkBindBufferMemory2(vk->device, 1, &bindBUfferMemoryInfo));
}

VkDeviceAddress getBufferAddress(struct VulkanDevice* vk, VkBuffer buffer) {
	VkBufferDeviceAddressInfo bufferDeviceAddressInfo;
	bufferDeviceAddressInfo.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
	bufferDeviceAddressInfo.pNext = NULL;
	bufferDeviceAddressInfo.buffer = buffer;
	return vkGetBufferDeviceAddress(vk->device, &bufferDeviceAddressInfo);
}

void destroyBufferAndMemory(struct VulkanDevice* vk, VkBuffer* buffer, VkDeviceMemory* memory) {
	vkDestroyBuffer(vk->device, *buffer, NULL);
	vkFreeMemory(vk->device, *memory, NULL);
	*buffer = VK_NULL_HANDLE;
	*memory = VK_NULL_HANDLE;
}

VkFence createFence(struct VulkanDevice* vk) {
	VkFenceCreateInfo fenceCreateInfo;
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.pNext = NULL;
	fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	VkFence fence;
	VK_ASSERT(vkCreateFence(vk->device, &fenceCreateInfo, NULL, &fence));
	return fence;
}

void recompileSpirv() {
	printf("Reload shaders\n");
	system("..\\spirv\\compile_shaders.bat");
}
