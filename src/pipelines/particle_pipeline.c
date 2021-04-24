#include "particle_pipeline.h"

#include "util/defines.h"
#include "vulkan/vulkan_device.h"
#include "vulkan/vulkan_debug.h"
#include "vulkan/view.h"

void createParticlesPipelineLayout(struct ParticlePipeline* pipeline) {
	struct VulkanDevice* vk = pipeline->vk;

	VkPushConstantRange pushConst;
	pushConst.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	pushConst.offset = 0;
	pushConst.size = sizeof(struct ParticlePushConst);

	VkDescriptorSetLayout graphicsLayouts[2];
	graphicsLayouts[0] = vk->view.projectionSetLayout;
	graphicsLayouts[1] = pipeline->graphicsDescriptorSetLayout;

	VkPipelineLayoutCreateInfo graphicsLayoutCreateInfo;
	graphicsLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	graphicsLayoutCreateInfo.pNext = NULL;
	graphicsLayoutCreateInfo.flags = 0;
	graphicsLayoutCreateInfo.setLayoutCount = ARRAY_SIZE(graphicsLayouts);
	graphicsLayoutCreateInfo.pSetLayouts = graphicsLayouts;
	graphicsLayoutCreateInfo.pushConstantRangeCount = 1;
	graphicsLayoutCreateInfo.pPushConstantRanges = &pushConst;
	VK_ASSERT(vkCreatePipelineLayout(vk->device, &graphicsLayoutCreateInfo, NULL, &pipeline->graphicsLayout));

	VkDescriptorSetLayout computeLayouts[1];
	computeLayouts[0] = pipeline->computeDescriptorSetLayout;

	VkPipelineLayoutCreateInfo computeLayoutCreateInfo;
	computeLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	computeLayoutCreateInfo.pNext = NULL;
	computeLayoutCreateInfo.flags = 0;
	computeLayoutCreateInfo.setLayoutCount = ARRAY_SIZE(computeLayouts);
	computeLayoutCreateInfo.pSetLayouts = computeLayouts;
	computeLayoutCreateInfo.pushConstantRangeCount = 0;
	computeLayoutCreateInfo.pPushConstantRanges = NULL;
	VK_ASSERT(vkCreatePipelineLayout(vk->device, &computeLayoutCreateInfo, NULL, &pipeline->computeLayout));
}

void createParticlesGraphicsPipeline(struct ParticlePipeline* pipeline) {
	struct VulkanDevice* vk = pipeline->vk;

	VkShaderModule shaderModules[2];
	shaderModules[0] = createShaderModule(vk, "../spirv/particles.vspv");
	shaderModules[1] = createShaderModule(vk, "../spirv/particles.fspv");

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
	vertexBindingDescription.stride = sizeof(struct ParticleVertex);
	vertexBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;

	VkVertexInputAttributeDescription vertexAttributeDescriptions[2];
	vertexAttributeDescriptions[0].location = 0;
	vertexAttributeDescriptions[0].binding = 0;
	vertexAttributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	vertexAttributeDescriptions[0].offset = offsetof(struct ParticleVertex, x);
	vertexAttributeDescriptions[1].location = 1;
	vertexAttributeDescriptions[1].binding = 0;
	vertexAttributeDescriptions[1].format = VK_FORMAT_R32_SFLOAT;
	vertexAttributeDescriptions[1].offset = offsetof(struct ParticleVertex, alive);

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
	inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
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
	if (pipeline->wireframe) {
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
	graphicsPipelineCreateInfo.layout = pipeline->graphicsLayout;
	graphicsPipelineCreateInfo.renderPass = vk->framebuffer.renderPass;
	graphicsPipelineCreateInfo.subpass = 0;
	graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
	graphicsPipelineCreateInfo.basePipelineIndex = -1;

	VK_ASSERT(vkCreateGraphicsPipelines(vk->device, VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, NULL, &pipeline->graphicsPipeline));

	vkDestroyShaderModule(vk->device, shaderModules[0], NULL);
	vkDestroyShaderModule(vk->device, shaderModules[1], NULL);
}

void createParticlesComputePipeline(struct ParticlePipeline* pipeline, int specialization) {
	struct VulkanDevice* vk = pipeline->vk;

	VkShaderModule shaderModule = createShaderModule(vk, "../spirv/particles.cspv");

	VkSpecializationMapEntry specializationMapEntry;
	specializationMapEntry.constantID = 0;
	specializationMapEntry.offset = 0;
	specializationMapEntry.size = sizeof(int);

	VkSpecializationInfo specializationInfo;
	specializationInfo.mapEntryCount = 1;
	specializationInfo.pMapEntries = &specializationMapEntry;
	specializationInfo.dataSize = sizeof(int);
	specializationInfo.pData = &specialization;

	VkPipelineShaderStageCreateInfo computeShaderStageCreateInfo;
	computeShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	computeShaderStageCreateInfo.pNext = NULL;
	computeShaderStageCreateInfo.flags = 0;
	computeShaderStageCreateInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
	computeShaderStageCreateInfo.module = shaderModule;
	computeShaderStageCreateInfo.pName = "main";
	computeShaderStageCreateInfo.pSpecializationInfo = &specializationInfo;

	VkComputePipelineCreateInfo computePipelineCreateInfo;
	computePipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	computePipelineCreateInfo.pNext = NULL;
	computePipelineCreateInfo.flags = 0;
	computePipelineCreateInfo.stage = computeShaderStageCreateInfo;
	computePipelineCreateInfo.layout = pipeline->computeLayout;
	computePipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
	computePipelineCreateInfo.basePipelineIndex = -1;

	vkCreateComputePipelines(vk->device, VK_NULL_HANDLE, 1, &computePipelineCreateInfo, NULL, &pipeline->computePipelines[specialization]);

	vkDestroyShaderModule(vk->device, shaderModule, NULL);
}

void createParticlesGraphicsDescriptorSet(struct ParticlePipeline* pipeline) {
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
	VK_ASSERT(vkCreateDescriptorPool(vk->device, &descriptorPoolCreateInfo, NULL, &pipeline->graphicsDescriptorPool));

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

	VK_ASSERT(vkCreateDescriptorSetLayout(vk->device, &descriptorSetLayoutCreateInfo, NULL, &pipeline->graphicsDescriptorSetLayout));

	VkDescriptorSetLayout layouts[IMAGE_COUNT];
	for (int i = 0; i < IMAGE_COUNT; ++i) {
		layouts[i] = pipeline->graphicsDescriptorSetLayout;
	}

	VkDescriptorSetAllocateInfo descriptorSetAllocateInfo;
	descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descriptorSetAllocateInfo.pNext = NULL;
	descriptorSetAllocateInfo.descriptorPool = pipeline->graphicsDescriptorPool;
	descriptorSetAllocateInfo.descriptorSetCount = IMAGE_COUNT;
	descriptorSetAllocateInfo.pSetLayouts = layouts;

	VK_ASSERT(vkAllocateDescriptorSets(vk->device, &descriptorSetAllocateInfo, pipeline->graphicsDescriptorSets));

	for (int i = 0; i < IMAGE_COUNT; ++i) {
		VkDescriptorBufferInfo bufferInfo;
		bufferInfo.buffer = pipeline->uniformBuffer.buffer;
		bufferInfo.offset = (VkDeviceSize)pipeline->uniformBuffer.size * i;
		bufferInfo.range = pipeline->uniformBuffer.size;

		VkWriteDescriptorSet storageBufferWrite;
		storageBufferWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		storageBufferWrite.pNext = NULL;
		storageBufferWrite.dstSet = pipeline->graphicsDescriptorSets[i];
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

void createParticlesComputeDescriptorSet(struct ParticlePipeline* pipeline) {
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
	VK_ASSERT(vkCreateDescriptorPool(vk->device, &descriptorPoolCreateInfo, NULL, &pipeline->computeDescriptorPool));

	VkDescriptorSetLayoutBinding bufferBindings[3];
	bufferBindings[0].binding = 0;
	bufferBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	bufferBindings[0].descriptorCount = 1;
	bufferBindings[0].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
	bufferBindings[0].pImmutableSamplers = NULL;
	bufferBindings[1].binding = 1;
	bufferBindings[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	bufferBindings[1].descriptorCount = 1;
	bufferBindings[1].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
	bufferBindings[1].pImmutableSamplers = NULL;
	bufferBindings[2].binding = 2;
	bufferBindings[2].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	bufferBindings[2].descriptorCount = 1;
	bufferBindings[2].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
	bufferBindings[2].pImmutableSamplers = NULL;

	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo;
	descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSetLayoutCreateInfo.pNext = NULL;
	descriptorSetLayoutCreateInfo.flags = 0;
	descriptorSetLayoutCreateInfo.bindingCount = ARRAY_SIZE(bufferBindings);
	descriptorSetLayoutCreateInfo.pBindings = bufferBindings;

	VK_ASSERT(vkCreateDescriptorSetLayout(vk->device, &descriptorSetLayoutCreateInfo, NULL, &pipeline->computeDescriptorSetLayout));

	VkDescriptorSetLayout layouts[IMAGE_COUNT];
	for (int i = 0; i < IMAGE_COUNT; ++i) {
		layouts[i] = pipeline->computeDescriptorSetLayout;
	}

	VkDescriptorSetAllocateInfo descriptorSetAllocateInfo;
	descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descriptorSetAllocateInfo.pNext = NULL;
	descriptorSetAllocateInfo.descriptorPool = pipeline->computeDescriptorPool;
	descriptorSetAllocateInfo.descriptorSetCount = IMAGE_COUNT;
	descriptorSetAllocateInfo.pSetLayouts = layouts;

	VK_ASSERT(vkAllocateDescriptorSets(vk->device, &descriptorSetAllocateInfo, pipeline->computeDescriptorSets));

	for (int i = 0; i < IMAGE_COUNT; ++i) {
		VkDescriptorBufferInfo bufferInfo[3];
		bufferInfo[0].buffer = pipeline->vertexBuffer.buffer;
		bufferInfo[0].offset = (VkDeviceSize)pipeline->vertexBuffer.size * ((i - 1llu + IMAGE_COUNT) % IMAGE_COUNT);
		bufferInfo[0].range = pipeline->vertexBuffer.size;
		bufferInfo[1].buffer = pipeline->vertexBuffer.buffer;
		bufferInfo[1].offset = (VkDeviceSize)pipeline->vertexBuffer.size * i;
		bufferInfo[1].range = pipeline->vertexBuffer.size;
		bufferInfo[2].buffer = pipeline->emitterBuffer.buffer;
		bufferInfo[2].offset = (VkDeviceSize)pipeline->emitterBuffer.size * i;
		bufferInfo[2].range = pipeline->emitterBuffer.size;

		VkWriteDescriptorSet storageBufferWrite[3];
		storageBufferWrite[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		storageBufferWrite[0].pNext = NULL;
		storageBufferWrite[0].dstSet = pipeline->computeDescriptorSets[i];
		storageBufferWrite[0].dstBinding = 0;
		storageBufferWrite[0].dstArrayElement = 0;
		storageBufferWrite[0].descriptorCount = 1;
		storageBufferWrite[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		storageBufferWrite[0].pImageInfo = NULL;
		storageBufferWrite[0].pBufferInfo = &bufferInfo[0];
		storageBufferWrite[0].pTexelBufferView = NULL;
		storageBufferWrite[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		storageBufferWrite[1].pNext = NULL;
		storageBufferWrite[1].dstSet = pipeline->computeDescriptorSets[i];
		storageBufferWrite[1].dstBinding = 1;
		storageBufferWrite[1].dstArrayElement = 0;
		storageBufferWrite[1].descriptorCount = 1;
		storageBufferWrite[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		storageBufferWrite[1].pImageInfo = NULL;
		storageBufferWrite[1].pBufferInfo = &bufferInfo[1];
		storageBufferWrite[1].pTexelBufferView = NULL;
		storageBufferWrite[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		storageBufferWrite[2].pNext = NULL;
		storageBufferWrite[2].dstSet = pipeline->computeDescriptorSets[i];
		storageBufferWrite[2].dstBinding = 2;
		storageBufferWrite[2].dstArrayElement = 0;
		storageBufferWrite[2].descriptorCount = 1;
		storageBufferWrite[2].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		storageBufferWrite[2].pImageInfo = NULL;
		storageBufferWrite[2].pBufferInfo = &bufferInfo[2];
		storageBufferWrite[2].pTexelBufferView = NULL;

		vkUpdateDescriptorSets(vk->device, ARRAY_SIZE(storageBufferWrite), storageBufferWrite, 0, NULL);
	}
}

void createParticlesPipelines(struct ParticlePipeline* pipeline) {
	struct VulkanDevice* vk = pipeline->vk;

	createParticlesGraphicsPipeline(pipeline);
	for (int i = 0; i < PARTICLES_COMPUTE_PIPELINES; ++i) {
		createParticlesComputePipeline(pipeline, i);
	}
}

void particlePipelineInit(struct ParticlePipeline* pipeline, struct VulkanDevice* vk) {
	pipeline->vk = vk;
	pipeline->firstInstance = 0;
	pipeline->wireframe = false;

	hostBufferInit(&pipeline->vertexBuffer, vk, (int)sizeof(struct VoxelVertex), MAX_VOXELS, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
	hostBufferInit(&pipeline->uniformBuffer, vk, (int)sizeof(struct VoxelUniform), MAX_VOXEL_OBJECT, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
	hostBufferInit(&pipeline->emitterBuffer, vk, (int)sizeof(struct ParticleEmitter), MAX_VOXEL_OBJECT, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
	hostBufferInit(&pipeline->drawBuffer, vk, (int)sizeof(VkDrawIndirectCommand), MAX_VOXEL_OBJECT, VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT);
	hostBufferInit(&pipeline->dispatchBuffer, vk, (int)sizeof(VkDispatchIndirectCommand), MAX_VOXEL_OBJECT, VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT);

	for (int i = 0; i < THREAD_COUNT; ++i) {
		vectorInit(&pipeline->renderContexts[i], 256, sizeof(struct RenderContext));
	}
	vectorInit(&pipeline->computeContexts, 256, sizeof(struct ComputeContext));

	createParticlesGraphicsDescriptorSet(pipeline);
	createParticlesComputeDescriptorSet(pipeline);
	createParticlesPipelineLayout(pipeline);
	createParticlesPipelines(pipeline);
}

void particlePipelineDestroy(struct ParticlePipeline* pipeline) {
	for (int i = 0; i < THREAD_COUNT; ++i) {
		vectorDestroy(&pipeline->renderContexts[i]);
	}
	vectorDestroy(&pipeline->computeContexts);

	struct VulkanDevice* vk = pipeline->vk;

	vkDestroyPipeline(vk->device, pipeline->graphicsPipeline, NULL);
	for (int i = 0; i < PARTICLES_COMPUTE_PIPELINES; ++i) {
		vkDestroyPipeline(vk->device, pipeline->computePipelines[i], NULL);
	}
	vkDestroyPipelineLayout(vk->device, pipeline->graphicsLayout, NULL);
	vkDestroyPipelineLayout(vk->device, pipeline->computeLayout, NULL);
	vkDestroyDescriptorSetLayout(vk->device, pipeline->graphicsDescriptorSetLayout, NULL);
	vkDestroyDescriptorSetLayout(vk->device, pipeline->computeDescriptorSetLayout, NULL);
	vkDestroyDescriptorPool(vk->device, pipeline->graphicsDescriptorPool, NULL);
	vkDestroyDescriptorPool(vk->device, pipeline->computeDescriptorPool, NULL);
	hostBufferDestroy(&pipeline->vertexBuffer, vk);
	hostBufferDestroy(&pipeline->uniformBuffer, vk);
	hostBufferDestroy(&pipeline->emitterBuffer, vk);
	hostBufferDestroy(&pipeline->drawBuffer, vk);
	hostBufferDestroy(&pipeline->dispatchBuffer, vk);
}

void particlePipelineReload(struct ParticlePipeline* pipeline) {
	struct VulkanDevice* vk = pipeline->vk;

	vkDestroyPipeline(vk->device, pipeline->graphicsPipeline, NULL);
	for (int i = 0; i < PARTICLES_COMPUTE_PIPELINES; ++i) {
		vkDestroyPipeline(vk->device, pipeline->computePipelines[i], NULL);
	}
	createParticlesPipelines(pipeline);
}

struct ComputeContext* particlePipelineGetComputeContext(struct ParticlePipeline* pipeline) {
	return (struct ComputeContext*)vectorGet(&pipeline->computeContexts, pipeline->queueIndex[pipeline->vk->writeIndex]);
}

struct RenderContext* particlePipelineGetRenderContext(struct ParticlePipeline* pipeline) {
	return (struct RenderContext*)vectorGet(&pipeline->renderContexts[pipeline->vk->writeIndex], pipeline->queueIndex[pipeline->vk->writeIndex]);
}

void particlePipelineCompute(struct ParticlePipeline* pipeline) {
	struct VulkanDevice* vk = pipeline->vk;

	VkCommandBuffer commandBuffer = vk->commandBuffers.computeCommandBuffers[vk->bufferWriteIndex];
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline->computeLayout, 0, 1, &pipeline->computeDescriptorSets[vk->bufferWriteIndex], 0, NULL);
	for (int i = 0; i < pipeline->queueIndex[vk->writeIndex]; ++i) {
		struct ComputeContext* ctx = vectorGet(&pipeline->computeContexts, i);
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline->computePipelines[ctx->specialization]);
		vkCmdDispatchIndirect(commandBuffer, pipeline->dispatchBuffer.buffer, sizeof(VkDispatchIndirectCommand) * i);
	}
}

void particlePipelineRender(struct ParticlePipeline* pipeline) {
	struct VulkanDevice* vk = pipeline->vk;

	VkCommandBuffer commandBuffer = vk->commandBuffers.renderCommandBuffers[vk->bufferReadIndex];
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->graphicsPipeline);
	VkDeviceSize vertexOffset = 0;
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->graphicsLayout, 1, 1, &pipeline->graphicsDescriptorSets[vk->bufferReadIndex], 0, NULL);
	for (int i = 0; i < pipeline->queueIndex[vk->readIndex]; ++i) {
		struct RenderContext* ctx = vectorGet(&pipeline->renderContexts[vk->readIndex], i);
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &pipeline->vertexBuffer.buffer, &vertexOffset);
		struct ParticlePushConst pushConst;
		pushConst.index = i;
		vkCmdPushConstants(commandBuffer, pipeline->graphicsLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(struct ParticlePushConst), &pushConst);
		VkDeviceSize indirectOffset = (i + vk->bufferReadIndex * MAX_PARTICLE_OBJECTS) * sizeof(VkDrawIndirectCommand);
		vkCmdDrawIndirect(commandBuffer, pipeline->drawBuffer.buffer, indirectOffset, 1, sizeof(VkDrawIndirectCommand));
	}
}

void particlePipelineAddToQueue(struct ParticlePipeline* pipeline, int instanceCount, void* uniformData, void* emitterData) {
	struct VulkanDevice* vk = pipeline->vk;

	hostBufferWrite(&pipeline->uniformBuffer, vk->bufferWriteIndex, pipeline->queueIndex[vk->writeIndex], 1, uniformData);
	hostBufferWrite(&pipeline->emitterBuffer, vk->bufferWriteIndex, pipeline->queueIndex[vk->writeIndex], 1, emitterData);

	VkDispatchIndirectCommand dispatchCmd;
	dispatchCmd.x = 1;
	dispatchCmd.y = 1;
	dispatchCmd.z = 1;
	int count = instanceCount;
	if (count / 4 > 0) {
		dispatchCmd.z = 4;
		if (count / 16 > 0) {
			dispatchCmd.y = 4;
			dispatchCmd.x = count / 16;
		}
	} else {
		dispatchCmd.z = count;
	}
	hostBufferWrite(&pipeline->dispatchBuffer, vk->bufferWriteIndex, pipeline->queueIndex[vk->writeIndex], 1, &dispatchCmd);

	VkDrawIndirectCommand drawCmd;
	drawCmd.vertexCount = 4;
	drawCmd.instanceCount = instanceCount;
	drawCmd.firstVertex = 0;
	drawCmd.firstInstance = pipeline->firstInstance;
	hostBufferWrite(&pipeline->drawBuffer, vk->bufferWriteIndex, pipeline->queueIndex[vk->writeIndex], 1, &drawCmd);

	++pipeline->queueIndex[vk->writeIndex];
	pipeline->firstInstance += instanceCount;
}
