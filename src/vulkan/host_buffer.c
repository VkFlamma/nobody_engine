#include "host_buffer.h"

#include "vulkan_device.h"
#include "util/defines.h"

void hostBufferInit(struct HostBuffer* hostBuffer, struct VulkanDevice* vk, int stride, int maxCount, VkBufferUsageFlags usage) {
	hostBuffer->buffer = VK_NULL_HANDLE;
	hostBuffer->stride = stride;
	hostBuffer->count = 0;
	hostBuffer->maxCount = maxCount;
	hostBuffer->size = stride * maxCount;
	hostBuffer->totalSize = hostBuffer->size * IMAGE_COUNT;

	VkMemoryPropertyFlags memoryFlags = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
	hostBuffer->buffer = createBuffer(vk, hostBuffer->totalSize, usage | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT);
	hostBuffer->memoryBlock = requestBufferMemory(&vk->memoryAllocator, vk, hostBuffer->buffer, memoryFlags, hostBuffer->totalSize);

	// hostBuffer->deviceAddress = getBufferAddress(vk, hostBuffer->buffer);
}

void hostBufferDestroy(struct HostBuffer* hostBuffer, struct VulkanDevice* vk) {
	vkDestroyBuffer(vk->device, hostBuffer->buffer, NULL);
	freeBufferMemory(&vk->memoryAllocator, &hostBuffer->memoryBlock);
}

void hostBufferWrite(struct HostBuffer* hostBuffer, int bufferWriteIndex, int first, int count, void* data) {
	size_t dstOffset = hostBuffer->size * bufferWriteIndex + hostBuffer->stride * first;
	uint8_t* dst = &hostBuffer->memoryBlock.mapped[dstOffset];
	memcpy(dst, data, (size_t)hostBuffer->stride * count);
}
