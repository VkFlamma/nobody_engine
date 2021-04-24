#include "texture.h"

#include <stdbool.h>

#include "vulkan_device.h"
#include "vulkan_debug.h"
#include "util/ktx.h"

void textureCreateImage(struct Texture* texture, struct VulkanDevice* vk) {
	VkImageFormatListCreateInfo imageFormatListCreateInfo;
	imageFormatListCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_FORMAT_LIST_CREATE_INFO;
	imageFormatListCreateInfo.pNext = NULL;
	imageFormatListCreateInfo.viewFormatCount = 0;
	imageFormatListCreateInfo.pViewFormats = NULL;

	VkImageCreateInfo imageCreateInfo;
	imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageCreateInfo.pNext = &imageFormatListCreateInfo;
	imageCreateInfo.flags = 0;
	imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
	imageCreateInfo.format = texture->format;
	imageCreateInfo.extent.width = texture->extent.width;
	imageCreateInfo.extent.height = texture->extent.height;
	imageCreateInfo.extent.depth = 1;
	imageCreateInfo.mipLevels = 1;
	imageCreateInfo.arrayLayers = 1;
	imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageCreateInfo.usage = texture->usage;
	imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageCreateInfo.queueFamilyIndexCount = 0;
	imageCreateInfo.pQueueFamilyIndices = NULL;
	imageCreateInfo.initialLayout = texture->layout;

	VK_ASSERT(vkCreateImage(vk->device, &imageCreateInfo, NULL, &texture->image));
}

void textureCreateImageView(struct Texture* texture, struct VulkanDevice* vk) {
	VkImageViewCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.pNext = NULL;
	createInfo.flags = 0;
	createInfo.image = texture->image;
	createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	createInfo.format = texture->format;
	createInfo.components.r = VK_COMPONENT_SWIZZLE_R;
	createInfo.components.g = VK_COMPONENT_SWIZZLE_G;
	createInfo.components.b = VK_COMPONENT_SWIZZLE_B;
	createInfo.components.a = VK_COMPONENT_SWIZZLE_A;
	if (texture->usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT) {
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	} else {
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	}
	createInfo.subresourceRange.baseMipLevel = 0;
	createInfo.subresourceRange.levelCount = 1;
	createInfo.subresourceRange.baseArrayLayer = 0;
	createInfo.subresourceRange.layerCount = 1;

	VK_ASSERT(vkCreateImageView(vk->device, &createInfo, NULL, &texture->view));
}

void textureCreateSampler(struct Texture* texture, struct VulkanDevice* vk) {
	VkSamplerCreateInfo samplerCreateInfo;
	samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerCreateInfo.pNext = NULL;
	samplerCreateInfo.flags = 0;
	samplerCreateInfo.magFilter = texture->filter;
	samplerCreateInfo.minFilter = texture->filter;
	if (texture->filter == VK_FILTER_NEAREST) {
		samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
	} else {
		samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	}
	samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCreateInfo.mipLodBias = 0.0f;
	samplerCreateInfo.anisotropyEnable = VK_FALSE;
	samplerCreateInfo.maxAnisotropy = 0.0f;
	samplerCreateInfo.compareEnable = VK_FALSE;
	samplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerCreateInfo.minLod = 0.0f;
	samplerCreateInfo.maxLod = 1.0f;
	samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

	VK_ASSERT(vkCreateSampler(vk->device, &samplerCreateInfo, NULL, &texture->sampler));
}

void textureCreateDescriptorSet(struct Texture* texture, struct VulkanDevice* vk) {
	VkDescriptorSetAllocateInfo allocateInfo;
	allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocateInfo.pNext = NULL;
	allocateInfo.descriptorPool = vk->textureAllocator.descriptorPool;
	allocateInfo.descriptorSetCount = 1;
	allocateInfo.pSetLayouts = &vk->textureAllocator.descriptorSetLayout;

	vkAllocateDescriptorSets(vk->device, &allocateInfo, &texture->descriptorSet);

	VkDescriptorImageInfo imageInfo;
	imageInfo.sampler = texture->sampler;
	imageInfo.imageView = texture->view;
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkWriteDescriptorSet writeDescriptorSet;
	writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescriptorSet.pNext = NULL;
	writeDescriptorSet.dstSet = texture->descriptorSet;
	writeDescriptorSet.dstBinding = 0;
	writeDescriptorSet.dstArrayElement = 0;
	writeDescriptorSet.descriptorCount = 1;
	writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	writeDescriptorSet.pImageInfo = &imageInfo;
	writeDescriptorSet.pBufferInfo = NULL;
	writeDescriptorSet.pTexelBufferView = NULL;

	vkUpdateDescriptorSets(vk->device, 1, &writeDescriptorSet, 0, NULL);
}

void setBarrierFlags(VkImageLayout layout, VkAccessFlags2KHR* accessFlags, VkPipelineStageFlags2KHR* stageFlags) {
	switch (layout) {
	case VK_IMAGE_LAYOUT_UNDEFINED:
		*accessFlags = 0;
		*stageFlags = VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT_KHR;
		break;
	case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
		*accessFlags = VK_ACCESS_2_TRANSFER_WRITE_BIT_KHR;
		*stageFlags = VK_PIPELINE_STAGE_2_TRANSFER_BIT_KHR;
		break;
	case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
		*accessFlags = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT_KHR;
		*stageFlags = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR;
		break;
	case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
		*accessFlags = VK_ACCESS_2_SHADER_READ_BIT_KHR;
		*stageFlags = VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT_KHR;
		break;
	case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
		*accessFlags = VK_ACCESS_2_TRANSFER_READ_BIT_KHR;
		*stageFlags = VK_PIPELINE_STAGE_2_TRANSFER_BIT_KHR;
		break;
	case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
		*accessFlags = 0;
		*stageFlags = VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT_KHR;
		break;
	default:
		assert(0);
		break;
	}
}

void transitionImageLayout(struct Texture* texture, VkCommandBuffer commandBuffer, VkImageLayout newLayout) {
	transitionImageLayoutTransfer(texture, commandBuffer, newLayout, VK_QUEUE_FAMILY_IGNORED, VK_QUEUE_FAMILY_IGNORED);
}

void transitionImageLayoutTransfer(struct Texture* texture, VkCommandBuffer commandBuffer, VkImageLayout newLayout, uint32_t srcQueue, uint32_t dstQueue) {
	VkImageMemoryBarrier2KHR imageMemoryBarrier;
	imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2_KHR;
	imageMemoryBarrier.pNext = NULL;
	imageMemoryBarrier.oldLayout = texture->layout;
	imageMemoryBarrier.newLayout = newLayout;
	imageMemoryBarrier.srcQueueFamilyIndex = srcQueue;
	imageMemoryBarrier.dstQueueFamilyIndex = dstQueue;
	imageMemoryBarrier.image = texture->image;

	if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
		imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	} else {
		imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	}

	imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
	imageMemoryBarrier.subresourceRange.levelCount = 1;
	imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
	imageMemoryBarrier.subresourceRange.layerCount = 1;

	setBarrierFlags(texture->layout, &imageMemoryBarrier.srcAccessMask, &imageMemoryBarrier.srcStageMask);
	setBarrierFlags(newLayout, &imageMemoryBarrier.dstAccessMask, &imageMemoryBarrier.dstStageMask);

	VkDependencyInfoKHR dependencyInfo;
	dependencyInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO_KHR;
	dependencyInfo.pNext = NULL;
	dependencyInfo.dependencyFlags = 0;
	dependencyInfo.memoryBarrierCount = 0;
	dependencyInfo.pMemoryBarriers = NULL;
	dependencyInfo.bufferMemoryBarrierCount = 0;
	dependencyInfo.pBufferMemoryBarriers = NULL;
	dependencyInfo.imageMemoryBarrierCount = 1;
	dependencyInfo.pImageMemoryBarriers = &imageMemoryBarrier;

	vkCmdPipelineBarrier2KHR(commandBuffer, &dependencyInfo);
	texture->layout = newLayout;
}

void renderTransitionImageLayout(struct Texture* texture, struct VulkanDevice* vk, VkImageLayout newLayout) {
	if (texture->layout == newLayout) {
		return;
	}

	VkCommandBuffer commandBuffer;
	renderAllocateCommand(vk, &commandBuffer);
	renderBeginSingleTimeCommand(commandBuffer);
	transitionImageLayout(texture, commandBuffer, newLayout);
	renderEndSingleTimeCommand(commandBuffer);
}

void textureCreate(struct Texture* texture, struct VulkanDevice* vk) {
	textureCreateImage(texture, vk);
	texture->memoryBlock = requestImageMemory(&vk->textureAllocator, vk, texture->image);
	textureCreateImageView(texture, vk);
	textureCreateSampler(texture, vk);
}

void textureInitColor(struct Texture* texture, struct VulkanDevice* vk, const char* file) {
	texture->layout = VK_IMAGE_LAYOUT_UNDEFINED;
	texture->format = VK_FORMAT_BC7_UNORM_BLOCK;
	texture->filter = VK_FILTER_NEAREST;
	texture->usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;

	texture->stagingBufferOffset = stagingBufferGetOffset(&vk->stagingBuffer);
	uint8_t* ptr = stagingBufferGetPointer(&vk->stagingBuffer, texture->stagingBufferOffset);
	uint32_t maxSize = stagingBufferGetAvailableSize(&vk->stagingBuffer);

	struct KtxHeader header = readKtx(ptr, file, maxSize);
	texture->extent.width = header.pixelWidth;
	texture->extent.height = header.pixelHeight;

	textureCreate(texture, vk);
	stagingBufferSubmitTexture(&vk->stagingBuffer, texture, header.imageSize);
	textureCreateDescriptorSet(texture, vk);
}

void textureInitDepth(struct Texture* texture, struct VulkanDevice* vk, VkExtent2D extent) {
	texture->layout = VK_IMAGE_LAYOUT_UNDEFINED;
	texture->format = findDepthFormat(vk);
	texture->filter = VK_FILTER_LINEAR;
	texture->usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;
	texture->extent = extent;
	textureCreate(texture, vk);
	renderTransitionImageLayout(texture, vk, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
}

void textureDestroy(struct Texture* texture, struct VulkanDevice* vk) {
	vkDestroySampler(vk->device, texture->sampler, NULL);
	vkDestroyImageView(vk->device, texture->view, NULL);
	vkDestroyImage(vk->device, texture->image, NULL);
	freeImageMemory(&vk->textureAllocator, &texture->memoryBlock);
}
