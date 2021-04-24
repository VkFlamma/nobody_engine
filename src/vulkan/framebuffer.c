#include "framebuffer.h"

#include <stdbool.h>

#include "vulkan_device.h"
#include "swapchain.h"
#include "vulkan_debug.h"

VkRenderPass createRenderPass(struct Framebuffer* framebuffer, struct VulkanDevice* vk, bool clearOnLoad) {
	VkAttachmentDescription2KHR colorAttachment;
	colorAttachment.sType = VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_2_KHR;
	colorAttachment.pNext = NULL;
	colorAttachment.flags = 0;
	colorAttachment.format = vk->swapchain.surfaceFormat.format;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	if (clearOnLoad) {
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	} else {
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
	}
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	if (clearOnLoad) {
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	} else{
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	}

	VkAttachmentDescription2KHR depthAttachment;
	depthAttachment.sType = VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_2_KHR;
	depthAttachment.pNext = NULL;
	depthAttachment.flags = 0;
	depthAttachment.format = framebuffer->depthTexture.format;
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription2KHR attachments[] = {
		colorAttachment,
		depthAttachment,
	};

	VkAttachmentReference2KHR colorAttachmentReference;
	colorAttachmentReference.sType = VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2_KHR;
	colorAttachmentReference.pNext = NULL;
	colorAttachmentReference.attachment = 0;
	colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	colorAttachmentReference.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

	VkAttachmentReference2KHR depthAttachmentReference;
	depthAttachmentReference.sType = VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2_KHR;
	depthAttachmentReference.pNext = NULL;
	depthAttachmentReference.attachment = 1;
	depthAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	depthAttachmentReference.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

	VkSubpassDescription2KHR subpassDescription;
	subpassDescription.sType = VK_STRUCTURE_TYPE_SUBPASS_DESCRIPTION_2_KHR;
	subpassDescription.pNext = NULL;
	subpassDescription.flags = 0;
	subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDescription.viewMask = 0;
	subpassDescription.inputAttachmentCount = 0;
	subpassDescription.pInputAttachments = NULL;
	subpassDescription.colorAttachmentCount = 1;
	subpassDescription.pColorAttachments = &colorAttachmentReference;
	subpassDescription.pResolveAttachments = NULL;
	subpassDescription.pDepthStencilAttachment = &depthAttachmentReference;
	subpassDescription.preserveAttachmentCount = 0;
	subpassDescription.pPreserveAttachments = NULL;

	VkSubpassDescription2KHR subpasses[] = {
		subpassDescription,
	};

	/*VkSubpassDependency2KHR subpassDependency;
	subpassDependency.sType = VK_STRUCTURE_TYPE_SUBPASS_DEPENDENCY_2_KHR;
	subpassDependency.pNext = NULL;
	subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	subpassDependency.dstSubpass = 0;
	subpassDependency.srcStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	subpassDependency.dstStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	subpassDependency.srcAccessMask = 0;
	subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	subpassDependency.dependencyFlags = 0;
	subpassDependency.viewOffset = 0;

	VkSubpassDependency2KHR dependencies[] = {
		subpassDependency,
	};*/

	VkRenderPassCreateInfo2KHR renderPassCreateInfo;
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO_2_KHR;
	renderPassCreateInfo.pNext = NULL;
	renderPassCreateInfo.flags = 0;
	renderPassCreateInfo.attachmentCount = ARRAY_SIZE(attachments);
	renderPassCreateInfo.pAttachments = attachments;
	renderPassCreateInfo.subpassCount = ARRAY_SIZE(subpasses);
	renderPassCreateInfo.pSubpasses = subpasses;
	renderPassCreateInfo.dependencyCount = 0;
	renderPassCreateInfo.pDependencies = NULL;
	renderPassCreateInfo.correlatedViewMaskCount = 0;
	renderPassCreateInfo.pCorrelatedViewMasks = NULL;

	VkRenderPass renderPass;
	VK_ASSERT(vkCreateRenderPass2(vk->device, &renderPassCreateInfo, NULL, &renderPass));
	return renderPass;
}

void createFramebuffers(struct Framebuffer* framebuffer, struct VulkanDevice* vk) {
	for (uint32_t i = 0; i < IMAGE_COUNT; ++i) {
		VkImageView attachments[] = { 
			vk->swapchain.imageViews[i],
			framebuffer->depthTexture.view,
		};

		VkFramebufferCreateInfo framebufferCreateInfo;
		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateInfo.pNext = NULL;
		framebufferCreateInfo.flags = 0;
		framebufferCreateInfo.renderPass = framebuffer->renderPass;
		framebufferCreateInfo.attachmentCount = ARRAY_SIZE(attachments);
		framebufferCreateInfo.pAttachments = attachments;
		framebufferCreateInfo.width = framebuffer->extent.width;
		framebufferCreateInfo.height = framebuffer->extent.height;
		framebufferCreateInfo.layers = 1;

		VK_ASSERT(vkCreateFramebuffer(vk->device, &framebufferCreateInfo, NULL, &framebuffer->framebuffers[i]));
	}
}

void framebufferInit(struct Framebuffer* framebuffer, struct VulkanDevice* vk) {
	framebuffer->extent = vk->swapchain.extent;
	textureInitDepth(&framebuffer->depthTexture, vk, framebuffer->extent);
	framebuffer->renderPass = createRenderPass(framebuffer, vk, true);
	framebuffer->guiRenderPass = createRenderPass(framebuffer, vk, false);
	createFramebuffers(framebuffer, vk);
}

void framebufferDestroy(struct Framebuffer* framebuffer, struct VulkanDevice* vk) {
	for (uint32_t i = 0; i < IMAGE_COUNT; ++i) {
		vkDestroyFramebuffer(vk->device, framebuffer->framebuffers[i], NULL);
	}
	vkDestroyRenderPass(vk->device, framebuffer->guiRenderPass, NULL);
	vkDestroyRenderPass(vk->device, framebuffer->renderPass, NULL);
	textureDestroy(&framebuffer->depthTexture, vk);
}

void framebufferBeginRenderPass(struct Framebuffer* framebuffer, struct VulkanDevice* vk, VkCommandBuffer commandBuffer) {
	VkClearValue clearValues[2];
	clearValues[0].color.int32[0] = 0;
	clearValues[0].color.int32[1] = 0;
	clearValues[0].color.int32[2] = 0;
	clearValues[0].color.int32[3] = 1;
	clearValues[1].depthStencil.depth = 1;
	clearValues[1].depthStencil.stencil = 0;

	VkRenderPassBeginInfo renderPassBeginInfo;
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.pNext = NULL;
	renderPassBeginInfo.renderPass = framebuffer->renderPass;
	renderPassBeginInfo.framebuffer = framebuffer->framebuffers[vk->imageIndex];
	renderPassBeginInfo.renderArea.offset.x = 0;
	renderPassBeginInfo.renderArea.offset.y = 0;
	renderPassBeginInfo.renderArea.extent = framebuffer->extent;
	renderPassBeginInfo.clearValueCount = ARRAY_SIZE(clearValues);
	renderPassBeginInfo.pClearValues = clearValues;

	VkSubpassBeginInfoKHR subpassBeginInfo;
	subpassBeginInfo.sType = VK_STRUCTURE_TYPE_SUBPASS_BEGIN_INFO_KHR;
	subpassBeginInfo.pNext = NULL;
	subpassBeginInfo.contents = VK_SUBPASS_CONTENTS_INLINE;

	vkCmdBeginRenderPass2(commandBuffer, &renderPassBeginInfo, &subpassBeginInfo);
}

void framebufferBeginGuiRenderPass(struct Framebuffer* framebuffer, struct VulkanDevice* vk, VkCommandBuffer commandBuffer) {
	VkClearValue clearValues[2];
	clearValues[0].color.int32[0] = 0;
	clearValues[0].color.int32[1] = 0;
	clearValues[0].color.int32[2] = 0;
	clearValues[0].color.int32[3] = 1;
	clearValues[1].depthStencil.depth = 1;
	clearValues[1].depthStencil.stencil = 0;

	VkRenderPassBeginInfo renderPassBeginInfo;
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.pNext = NULL;
	renderPassBeginInfo.renderPass = framebuffer->guiRenderPass;
	renderPassBeginInfo.framebuffer = framebuffer->framebuffers[vk->imageIndex];
	renderPassBeginInfo.renderArea.offset.x = 0;
	renderPassBeginInfo.renderArea.offset.y = 0;
	renderPassBeginInfo.renderArea.extent = framebuffer->extent;
	renderPassBeginInfo.clearValueCount = ARRAY_SIZE(clearValues);
	renderPassBeginInfo.pClearValues = clearValues;

	VkSubpassBeginInfoKHR subpassBeginInfo;
	subpassBeginInfo.sType = VK_STRUCTURE_TYPE_SUBPASS_BEGIN_INFO_KHR;
	subpassBeginInfo.pNext = NULL;
	subpassBeginInfo.contents = VK_SUBPASS_CONTENTS_INLINE;

	vkCmdBeginRenderPass2(commandBuffer, &renderPassBeginInfo, &subpassBeginInfo);
}

void framebufferEndRenderPass(struct Framebuffer* framebuffer, VkCommandBuffer commandBuffer) {
	VkSubpassEndInfoKHR subpassEndInfo;
	subpassEndInfo.sType = VK_STRUCTURE_TYPE_SUBPASS_END_INFO_KHR;
	subpassEndInfo.pNext = NULL;

	vkCmdEndRenderPass2(commandBuffer, &subpassEndInfo);
}
