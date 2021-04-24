#ifndef VOXEL_PIPELINE_H
#define VOXEL_PIPELINE_H

#include <stdbool.h>

#include "vulkan/vulkan_includes.h"
#include "vulkan/vulkan_util.h"
#include "vulkan/host_buffer.h"
#include "math/mat4.h"
#include "vulkan/context.h"
#include "containers/vector.h"

#define MAX_VOXELS 4096
#define MAX_VOXEL_OBJECT 256
#define MAX_BONES 8

#define VOXEL_GRAPHICS_PIPELINES 2

enum VoxelGraphicsSpecialization {
	VOXEL_GRAPHICS_DEFAULT,
	VOXEL_GRAPHICS_WIREFRAME,
};

struct VoxelVertex {
	float x;
	float y;
	float z;
	float r;
	float g;
	float b;
	float a;
	int boneIndex;
};

struct VoxelUniform {
	struct mat4 modelMatrix;
	struct mat4 boneMatrices[MAX_BONES];
};

struct VoxelPushConst {
	uint32_t index;
};

struct VoxelPipeline {
	struct VulkanDevice* vk;
	VkPipeline pipelines[VOXEL_GRAPHICS_PIPELINES];
	VkPipelineLayout layout;

	struct HostBuffer vertexBuffer;
	struct HostBuffer uniformBuffer;
	struct HostBuffer drawBuffer;

	struct Vector renderContexts[THREAD_COUNT];

	VkDescriptorPool descriptorPool;
	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorSet descriptorSets[IMAGE_COUNT];

	int queueIndex[THREAD_COUNT];
	int firstVertex;

	bool wireframe;
};

void voxelPipelineInit(struct VoxelPipeline* pipeline, struct VulkanDevice* vk);
void voxelPipelineDestroy(struct VoxelPipeline* pipeline);
void voxelPipelineReload(struct VoxelPipeline* pipeline);

struct RenderContext* voxelPipelineGetRenderContext(struct VoxelPipeline* pipeline);

void voxelPipelineRender(struct VoxelPipeline* pipeline);

void voxelPipelineAddToQueue(struct VoxelPipeline* pipeline, int vertexCount, void* vertexData, void* uniformData);

#endif // VOXEL_PIPELINE_H
