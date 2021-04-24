#include "voxel_pipeline.h"

#include <stdio.h>
#include <stdlib.h>

#include "util/defines.h"
#include "vulkan/vulkan_device.h"
#include "vulkan/vulkan_debug.h"
#include "vulkan/view.h"

void createPipelineLayout(struct VoxelPipeline* pipeline) {
	struct VulkanDevice* vk = pipeline->vk;

	VkDescriptorSetLayout layouts[2];
	layouts[0] = vk->view.projectionSetLayout;
	layouts[1] = pipeline->descriptorSetLayout;

	VkPushConstantRange pushConst;
	pushConst.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	pushConst.offset = 0;
	pushConst.size = sizeof(struct VoxelPushConst);

	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo;
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.pNext = NULL;
	pipelineLayoutCreateInfo.flags = 0;
	pipelineLayoutCreateInfo.setLayoutCount = ARRAY_SIZE(layouts);
	pipelineLayoutCreateInfo.pSetLayouts = layouts;
	pipelineLayoutCreateInfo.pushConstantRangeCount = 1;
	pipelineLayoutCreateInfo.pPushConstantRanges = &pushConst;

	VK_ASSERT(vkCreatePipelineLayout(vk->device, &pipelineLayoutCreateInfo, NULL, &pipeline->layout));
}

void createVoxelGraphicsPipeline(struct VoxelPipeline* pipeline, uint32_t specialization) {
	struct VulkanDevice* vk = pipeline->vk;

	VkShaderModule shaderModules[2];
	shaderModules[0] = createShaderModule(vk, "../spirv/quad.vspv");
	shaderModules[1] = createShaderModule(vk, "../spirv/quad.fspv");

	VkSpecializationInfo specializationInfo;
	specializationInfo.mapEntryCount = 0;
	specializationInfo.pMapEntries = NULL;
	specializationInfo.dataSize = 0;
	specializationInfo.pData = NULL;

	VkPipelineShaderStageCreateInfo shaderStagesCreateInfo[2];

	shaderStagesCreateInfo[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStagesCreateInfo[0].pNext = NULL;
	shaderStagesCreateInfo[0].flags = 0;
	shaderStagesCreateInfo[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	shaderStagesCreateInfo[0].module = shaderModules[0];
	shaderStagesCreateInfo[0].pName = "main";
	shaderStagesCreateInfo[0].pSpecializationInfo = &specializationInfo;

	shaderStagesCreateInfo[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStagesCreateInfo[1].pNext = NULL;
	shaderStagesCreateInfo[1].flags = 0;
	shaderStagesCreateInfo[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	shaderStagesCreateInfo[1].module = shaderModules[1];
	shaderStagesCreateInfo[1].pName = "main";
	shaderStagesCreateInfo[1].pSpecializationInfo = &specializationInfo;

	VkVertexInputBindingDescription vertexBindingDescription;
	vertexBindingDescription.binding = 0;
	vertexBindingDescription.stride = sizeof(struct VoxelVertex);
	vertexBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	VkVertexInputAttributeDescription vertexAttributeDescriptions[3];
	vertexAttributeDescriptions[0].location = 0;
	vertexAttributeDescriptions[0].binding = 0;
	vertexAttributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	vertexAttributeDescriptions[0].offset = offsetof(struct VoxelVertex, x);
	vertexAttributeDescriptions[1].location = 1;
	vertexAttributeDescriptions[1].binding = 0;
	vertexAttributeDescriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
	vertexAttributeDescriptions[1].offset = offsetof(struct VoxelVertex, r);
	vertexAttributeDescriptions[2].location = 2;
	vertexAttributeDescriptions[2].binding = 0;
	vertexAttributeDescriptions[2].format = VK_FORMAT_R32_SINT;
	vertexAttributeDescriptions[2].offset = offsetof(struct VoxelVertex, boneIndex);

	VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo;
	vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputStateCreateInfo.pNext = NULL;
	vertexInputStateCreateInfo.flags = 0;
	vertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;
	vertexInputStateCreateInfo.pVertexBindingDescriptions = &vertexBindingDescription;
	vertexInputStateCreateInfo.vertexAttributeDescriptionCount = ARRAY_SIZE(vertexAttributeDescriptions);
	vertexInputStateCreateInfo.pVertexAttributeDescriptions = vertexAttributeDescriptions;

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo;
	inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyStateCreateInfo.pNext = NULL;
	inputAssemblyStateCreateInfo.flags = 0;
	inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

	VkPipelineTessellationStateCreateInfo tessellationStateCreateInfo;
	tessellationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
	tessellationStateCreateInfo.pNext = NULL;
	tessellationStateCreateInfo.flags = 0;
	tessellationStateCreateInfo.patchControlPoints = 4;

	VkExtent2D windowExtent = vk->framebuffer.extent;

	VkViewport viewport;
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)windowExtent.width;
	viewport.height = (float)windowExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor;
	scissor.offset.x = 0;
	scissor.offset.y = 0;
	scissor.extent = windowExtent;

	VkPipelineViewportStateCreateInfo viewportStateCreateInfo;
	viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportStateCreateInfo.pNext = NULL;
	viewportStateCreateInfo.flags = 0;
	viewportStateCreateInfo.viewportCount = 1;
	viewportStateCreateInfo.pViewports = &viewport;
	viewportStateCreateInfo.scissorCount = 1;
	viewportStateCreateInfo.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo;
	rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizationStateCreateInfo.pNext = NULL;
	rasterizationStateCreateInfo.flags = 0;
	rasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
	rasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
	if (pipeline->wireframe || specialization == VOXEL_GRAPHICS_WIREFRAME) {
		rasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_LINE;
	} else {
		rasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
	}
	rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_NONE;
	rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
	rasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f;
	rasterizationStateCreateInfo.depthBiasClamp = 0.0f;
	rasterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f;
	rasterizationStateCreateInfo.lineWidth = 1.0f;

	VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo;
	multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampleStateCreateInfo.pNext = NULL;
	multisampleStateCreateInfo.flags = 0;
	multisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
	multisampleStateCreateInfo.minSampleShading = 0.0f;
	multisampleStateCreateInfo.pSampleMask = NULL;
	multisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
	multisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;

	VkStencilOpState stencilOpState;
	stencilOpState.failOp = VK_STENCIL_OP_ZERO;
	stencilOpState.passOp = VK_STENCIL_OP_ZERO;
	stencilOpState.depthFailOp = VK_STENCIL_OP_ZERO;
	stencilOpState.compareOp = VK_STENCIL_OP_ZERO;
	stencilOpState.compareMask = 0;
	stencilOpState.writeMask = 0;
	stencilOpState.reference = 0;

	VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo;
	depthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencilStateCreateInfo.pNext = NULL;
	depthStencilStateCreateInfo.flags = 0;
	depthStencilStateCreateInfo.depthTestEnable = VK_TRUE;
	depthStencilStateCreateInfo.depthWriteEnable = VK_TRUE;
	depthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
	depthStencilStateCreateInfo.depthBoundsTestEnable = VK_TRUE;
	depthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;
	depthStencilStateCreateInfo.front = stencilOpState;
	depthStencilStateCreateInfo.back = stencilOpState;
	depthStencilStateCreateInfo.minDepthBounds = 0.0f;
	depthStencilStateCreateInfo.maxDepthBounds = 1.0f;

	VkPipelineColorBlendAttachmentState colorBlendAttachmentState;
	colorBlendAttachmentState.blendEnable = VK_TRUE;
	colorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

	VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo;
	colorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendStateCreateInfo.pNext = NULL;
	colorBlendStateCreateInfo.flags = 0;
	colorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
	colorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_COPY;
	colorBlendStateCreateInfo.attachmentCount = 1;
	colorBlendStateCreateInfo.pAttachments = &colorBlendAttachmentState;
	colorBlendStateCreateInfo.blendConstants[0] = 0.0f;
	colorBlendStateCreateInfo.blendConstants[1] = 0.0f;
	colorBlendStateCreateInfo.blendConstants[2] = 0.0f;
	colorBlendStateCreateInfo.blendConstants[3] = 0.0f;

	VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo;
	dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicStateCreateInfo.pNext = NULL;
	dynamicStateCreateInfo.flags = 0;
	dynamicStateCreateInfo.dynamicStateCount = 0;
	dynamicStateCreateInfo.pDynamicStates = NULL;

	VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo;
	graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	graphicsPipelineCreateInfo.pNext = NULL;
	graphicsPipelineCreateInfo.flags = 0;
	graphicsPipelineCreateInfo.stageCount = ARRAY_SIZE(shaderStagesCreateInfo);
	graphicsPipelineCreateInfo.pStages = shaderStagesCreateInfo;
	graphicsPipelineCreateInfo.pVertexInputState = &vertexInputStateCreateInfo;
	graphicsPipelineCreateInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;
	graphicsPipelineCreateInfo.pTessellationState = &tessellationStateCreateInfo;
	graphicsPipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
	graphicsPipelineCreateInfo.pRasterizationState = &rasterizationStateCreateInfo;
	graphicsPipelineCreateInfo.pMultisampleState = &multisampleStateCreateInfo;
	graphicsPipelineCreateInfo.pDepthStencilState = &depthStencilStateCreateInfo;
	graphicsPipelineCreateInfo.pColorBlendState = &colorBlendStateCreateInfo;
	graphicsPipelineCreateInfo.pDynamicState = &dynamicStateCreateInfo;
	graphicsPipelineCreateInfo.layout = pipeline->layout;
	graphicsPipelineCreateInfo.renderPass = vk->framebuffer.renderPass;
	graphicsPipelineCreateInfo.subpass = 0;
	graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
	graphicsPipelineCreateInfo.basePipelineIndex = -1;

	VK_ASSERT(vkCreateGraphicsPipelines(vk->device, VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, NULL, &pipeline->pipelines[specialization]));

	vkDestroyShaderModule(vk->device, shaderModules[0], NULL);
	vkDestroyShaderModule(vk->device, shaderModules[1], NULL);
}

void createDescriptorSet(struct VoxelPipeline* pipeline) {
	struct VulkanDevice* vk = pipeline->vk;

	VkDescriptorPoolSize descriptorPoolSize;
	descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	descriptorPoolSize.descriptorCount = 1;

	VkDescriptorPoolCreateInfo descriptorPoolCreateInfo;
	descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolCreateInfo.pNext = NULL;
	descriptorPoolCreateInfo.flags = 0;
	descriptorPoolCreateInfo.maxSets = IMAGE_COUNT;
	descriptorPoolCreateInfo.poolSizeCount = 1;
	descriptorPoolCreateInfo.pPoolSizes = &descriptorPoolSize;
	VK_ASSERT(vkCreateDescriptorPool(vk->device, &descriptorPoolCreateInfo, NULL, &pipeline->descriptorPool));

	VkDescriptorSetLayoutBinding storageBufferBinding;
	storageBufferBinding.binding = 0;
	storageBufferBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	storageBufferBinding.descriptorCount = 1;
	storageBufferBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	storageBufferBinding.pImmutableSamplers = NULL;

	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo;
	descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSetLayoutCreateInfo.pNext = NULL;
	descriptorSetLayoutCreateInfo.flags = 0;
	descriptorSetLayoutCreateInfo.bindingCount = 1;
	descriptorSetLayoutCreateInfo.pBindings = &storageBufferBinding;

	VK_ASSERT(vkCreateDescriptorSetLayout(vk->device, &descriptorSetLayoutCreateInfo, NULL, &pipeline->descriptorSetLayout));

	VkDescriptorSetLayout layouts[IMAGE_COUNT];
	for (int i = 0; i < IMAGE_COUNT; ++i) {
		layouts[i] = pipeline->descriptorSetLayout;
	}

	VkDescriptorSetAllocateInfo descriptorSetAllocateInfo;
	descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descriptorSetAllocateInfo.pNext = NULL;
	descriptorSetAllocateInfo.descriptorPool = pipeline->descriptorPool;
	descriptorSetAllocateInfo.descriptorSetCount = IMAGE_COUNT;
	descriptorSetAllocateInfo.pSetLayouts = layouts;

	VK_ASSERT(vkAllocateDescriptorSets(vk->device, &descriptorSetAllocateInfo, pipeline->descriptorSets));

	for (int i = 0; i < IMAGE_COUNT; ++i) {
		VkDescriptorBufferInfo bufferInfo;
		bufferInfo.buffer = pipeline->uniformBuffer.buffer;
		bufferInfo.offset = (VkDeviceSize)pipeline->uniformBuffer.size * i;
		bufferInfo.range = pipeline->uniformBuffer.size;

		VkWriteDescriptorSet storageBufferWrite;
		storageBufferWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		storageBufferWrite.pNext = NULL;
		storageBufferWrite.dstSet = pipeline->descriptorSets[i];
		storageBufferWrite.dstBinding = 0;
		storageBufferWrite.dstArrayElement = 0;
		storageBufferWrite.descriptorCount = 1;
		storageBufferWrite.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		storageBufferWrite.pImageInfo = NULL;
		storageBufferWrite.pBufferInfo = &bufferInfo;
		storageBufferWrite.pTexelBufferView = NULL;

		vkUpdateDescriptorSets(vk->device, 1, &storageBufferWrite, 0, NULL);
	}
}

void createVoxelPipelines(struct VoxelPipeline* pipeline) {
	struct VulkanDevice* vk = pipeline->vk;

	for (int i = 0; i < PARTICLES_COMPUTE_PIPELINES; ++i) {
		createVoxelGraphicsPipeline(pipeline, i);
	}
}

void voxelPipelineInit(struct VoxelPipeline* pipeline, struct VulkanDevice* vk) {
	pipeline->vk = vk;
	pipeline->firstVertex = 0;
	pipeline->wireframe = false;

	hostBufferInit(&pipeline->vertexBuffer, vk, (int)sizeof(struct VoxelVertex), MAX_VOXELS, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
	hostBufferInit(&pipeline->uniformBuffer, vk, (int)sizeof(struct VoxelUniform), MAX_VOXEL_OBJECT, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
	hostBufferInit(&pipeline->drawBuffer, vk, (int)sizeof(VkDrawIndirectCommand), MAX_VOXEL_OBJECT, VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT);

	for (int i = 0; i < THREAD_COUNT; ++i) {
		vectorInit(&pipeline->renderContexts[i], 256, sizeof(struct RenderContext));
	}

	createDescriptorSet(pipeline);
	createPipelineLayout(pipeline);
	createVoxelPipelines(pipeline);
}

void voxelPipelineDestroy(struct VoxelPipeline* pipeline) {
	for (int i = 0; i < THREAD_COUNT; ++i) {
		vectorDestroy(&pipeline->renderContexts[i]);
	}

	struct VulkanDevice* vk = pipeline->vk;

	for (int i = 0; i < VOXEL_GRAPHICS_PIPELINES; ++i) {
		vkDestroyPipeline(vk->device, pipeline->pipelines[i], NULL);
	}
	vkDestroyPipelineLayout(vk->device, pipeline->layout, NULL);
	vkDestroyDescriptorSetLayout(vk->device, pipeline->descriptorSetLayout, NULL);
	vkDestroyDescriptorPool(vk->device, pipeline->descriptorPool, NULL);
	hostBufferDestroy(&pipeline->vertexBuffer, vk);
	hostBufferDestroy(&pipeline->uniformBuffer, vk);
	hostBufferDestroy(&pipeline->drawBuffer, vk);
}

void voxelPipelineReload(struct VoxelPipeline* pipeline) {
	struct VulkanDevice* vk = pipeline->vk;

	for (int i = 0; i < VOXEL_GRAPHICS_PIPELINES; ++i) {
		vkDestroyPipeline(vk->device, pipeline->pipelines[i], NULL);
	}
	createVoxelPipelines(pipeline);
}

struct RenderContext* voxelPipelineGetRenderContext(struct VoxelPipeline* pipeline) {
	return (struct RenderContext*)vectorGet(&pipeline->renderContexts[pipeline->vk->writeIndex], pipeline->queueIndex[pipeline->vk->writeIndex]);
}

void voxelPipelineRender(struct VoxelPipeline* pipeline) {
	struct VulkanDevice* vk = pipeline->vk;

	VkCommandBuffer commandBuffer = vk->commandBuffers.renderCommandBuffers[vk->bufferReadIndex];
	VkDeviceSize vertexOffset = (VkDeviceSize)pipeline->vertexBuffer.size * vk->bufferReadIndex;
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &pipeline->vertexBuffer.buffer, &vertexOffset);
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->layout, 1, 1, &pipeline->descriptorSets[vk->bufferReadIndex], 0, NULL);
	for (int i = 0; i < pipeline->queueIndex[vk->readIndex]; ++i) {
		struct RenderContext* ctx = vectorGet(&pipeline->renderContexts[vk->readIndex], i);
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipelines[ctx->specialization]);
		struct VoxelPushConst pushConst;
		pushConst.index = i;
		vkCmdPushConstants(commandBuffer, pipeline->layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(struct VoxelPushConst), &pushConst);
		VkDeviceSize indirectOffset = (i + vk->bufferReadIndex * MAX_VOXEL_OBJECT) * sizeof(VkDrawIndirectCommand);
		vkCmdDrawIndirect(commandBuffer, pipeline->drawBuffer.buffer, indirectOffset, 1, sizeof(VkDrawIndirectCommand));
	}
}

void voxelPipelineAddToQueue(struct VoxelPipeline* pipeline, int vertexCount, void* vertexData, void* uniformData) {
	struct VulkanDevice* vk = pipeline->vk;
	VkDrawIndirectCommand cmd;
	cmd.vertexCount = vertexCount;
	cmd.instanceCount = 1;
	cmd.firstVertex = pipeline->firstVertex;
	cmd.firstInstance = pipeline->queueIndex[vk->writeIndex];

	int index = pipeline->queueIndex[vk->writeIndex];
	pipeline->firstVertex += vertexCount;
	hostBufferWrite(&pipeline->vertexBuffer, vk->bufferWriteIndex, cmd.firstVertex, vertexCount, vertexData);
	hostBufferWrite(&pipeline->uniformBuffer, vk->bufferWriteIndex, index, 1, uniformData);
	hostBufferWrite(&pipeline->drawBuffer, vk->bufferWriteIndex, index, 1, &cmd);
	++pipeline->queueIndex[vk->writeIndex];
}
