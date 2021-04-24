#include "staging_buffer.h"

#include "vulkan_device.h"

void stagingBufferInit(struct StagingBuffer* stagingBuffer, struct VulkanDevice* vk) {
	stagingBuffer->vk = vk;
	stagingBuffer->offset = 0;

	VkDeviceSize size = STAGING_BUFFER_SIZE * IMAGE_COUNT;
	createBufferAndMemory(vk, &stagingBuffer->buffer, &stagingBuffer->memory, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	vkMapMemory(vk->device, stagingBuffer->memory, 0, size, 0, (void**)&stagingBuffer->mapped);

	for (int i = 0; i < THREAD_COUNT; ++i) {
		vectorInit(&stagingBuffer->textureSubmits[i], 256, sizeof(struct Texture));
	}
}

void stagingBufferDestroy(struct StagingBuffer* stagingBuffer) {
	destroyBufferAndMemory(stagingBuffer->vk, &stagingBuffer->buffer, &stagingBuffer->memory);
	for (int i = 0; i < THREAD_COUNT; ++i) {
		vectorDestroy(&stagingBuffer->textureSubmits[i]);
	}
}

uint32_t stagingBufferGetOffset(struct StagingBuffer* stagingBuffer) {
	return stagingBuffer->offset;
}

uint8_t* stagingBufferGetPointer(struct StagingBuffer* stagingBuffer, uint32_t offset) {
	return &stagingBuffer->mapped[offset];
}

uint32_t stagingBufferGetAvailableSize(struct StagingBuffer* stagingBuffer) {
	return STAGING_BUFFER_SIZE - stagingBuffer->offset;
}

void stagingBufferSubmitTexture(struct StagingBuffer* stagingBuffer, struct Texture* texture, uint32_t imageSize) {
	void* p = vectorAddAndGet(&stagingBuffer->textureSubmits[stagingBuffer->vk->writeIndex]);
	memcpy(p, texture, sizeof(struct Texture));
	stagingBuffer->offset += imageSize;
}

void stagingBufferSubmit(struct StagingBuffer* stagingBuffer, VkCommandBuffer commandBuffer) {
	for (int i = 0; i < stagingBuffer->textureSubmits[stagingBuffer->vk->writeIndex].count; ++i) {
		struct Texture* texture = vectorGet(&stagingBuffer->textureSubmits[stagingBuffer->vk->writeIndex], i);

		transitionImageLayout(texture, commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		VkBufferImageCopy2KHR bufferImageCopy;
		bufferImageCopy.sType = VK_STRUCTURE_TYPE_BUFFER_IMAGE_COPY_2_KHR;
		bufferImageCopy.pNext = NULL;
		bufferImageCopy.bufferOffset = texture->stagingBufferOffset;
		bufferImageCopy.bufferRowLength = 0;
		bufferImageCopy.bufferImageHeight = 0;
		bufferImageCopy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		bufferImageCopy.imageSubresource.mipLevel = 0;
		bufferImageCopy.imageSubresource.baseArrayLayer = 0;
		bufferImageCopy.imageSubresource.layerCount = 1;
		bufferImageCopy.imageOffset.x = 0;
		bufferImageCopy.imageOffset.y = 0;
		bufferImageCopy.imageOffset.z = 0;
		bufferImageCopy.imageExtent.width = texture->extent.width;
		bufferImageCopy.imageExtent.height = texture->extent.height;
		bufferImageCopy.imageExtent.depth = 1;

		VkCopyBufferToImageInfo2KHR copyBufferToImageInfo;
		copyBufferToImageInfo.sType = VK_STRUCTURE_TYPE_COPY_BUFFER_TO_IMAGE_INFO_2_KHR;
		copyBufferToImageInfo.pNext = NULL;
		copyBufferToImageInfo.srcBuffer = stagingBuffer->buffer;
		copyBufferToImageInfo.dstImage = texture->image;
		copyBufferToImageInfo.dstImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		copyBufferToImageInfo.regionCount = 1;
		copyBufferToImageInfo.pRegions = &bufferImageCopy;

		vkCmdCopyBufferToImage2KHR(commandBuffer, &copyBufferToImageInfo);
		transitionImageLayoutTransfer(texture, commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, stagingBuffer->vk->queues[TRANSFER_INDEX].index, stagingBuffer->vk->queues[GRAPHICS_INDEX].index);
	}
	stagingBuffer->offset = 0;
}

void stagingBufferLoad(struct StagingBuffer* stagingBuffer, VkCommandBuffer commandBuffer) {
	for (int i = 0; i < stagingBuffer->textureSubmits[stagingBuffer->vk->readIndex].count; ++i) {
		struct Texture* texture = vectorGet(&stagingBuffer->textureSubmits[stagingBuffer->vk->readIndex], i);

		transitionImageLayoutTransfer(texture, commandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, stagingBuffer->vk->queues[TRANSFER_INDEX].index, stagingBuffer->vk->queues[GRAPHICS_INDEX].index);
	}
	vectorClear(&stagingBuffer->textureSubmits[stagingBuffer->vk->readIndex]);
}
