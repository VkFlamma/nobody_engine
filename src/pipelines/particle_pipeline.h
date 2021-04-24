#ifndef PARTICLE_PIPELINE_H
#define PARTICLE_PIPELINE_H

#include <stdbool.h>

#include "vulkan/vulkan_includes.h"
#include "vulkan/vulkan_util.h"
#include "vulkan/host_buffer.h"
#include "math/mat4.h"
#include "math/vec2.h"
#include "math/vec3.h"
#include "containers/vector.h"

#define MAX_PARTICLES 4096
#define MAX_PARTICLE_OBJECTS 256

#define PARTICLES_COMPUTE_PIPELINES 2

enum ParticleComputeSpecialization {
	PARTICLE_COMPUTE_INIT,
	PARTICLE_COMPUTE_UPDATE,
};

struct ParticleVertex {
	float x;
	float y;
	float z;
	float alive;
	float vx;
	float vy;
	float vz;
	float life;
};

struct ParticleUniform {
	struct mat4 modelMatrix;
};

struct ParticlePushConst {
	uint32_t index;
};

struct ParticleEmitter {
	uint32_t readOffset;
	uint32_t writeOffset;
	float startPosX;
	float startPosY;
	float startPosZ;
	float endPosX;
	float endPosY;
	float endPosZ;
	float startVelX;
	float startVelY;
	float startVelZ;
	float endVelX;
	float endVelY;
	float endVelZ;
	float delta;
	float startLife;
	float endLife;
};

struct ParticlePipeline {
	struct VulkanDevice* vk;
	VkPipeline graphicsPipeline;
	VkPipeline computePipelines[PARTICLES_COMPUTE_PIPELINES];
	VkPipelineLayout graphicsLayout;
	VkPipelineLayout computeLayout;

	struct HostBuffer vertexBuffer;
	struct HostBuffer uniformBuffer;
	struct HostBuffer emitterBuffer;
	struct HostBuffer drawBuffer;
	struct HostBuffer dispatchBuffer;

	struct Vector renderContexts[THREAD_COUNT];
	struct Vector computeContexts;

	VkDescriptorPool graphicsDescriptorPool;
	VkDescriptorPool computeDescriptorPool;
	VkDescriptorSetLayout graphicsDescriptorSetLayout;
	VkDescriptorSetLayout computeDescriptorSetLayout;
	VkDescriptorSet graphicsDescriptorSets[IMAGE_COUNT];
	VkDescriptorSet computeDescriptorSets[IMAGE_COUNT];

	int queueIndex[THREAD_COUNT];
	int firstInstance;

	bool wireframe;
};

void particlePipelineInit(struct ParticlePipeline* pipeline, struct VulkanDevice* vk);
void particlePipelineDestroy(struct ParticlePipeline* pipeline);
void particlePipelineReload(struct ParticlePipeline* pipeline);

struct ComputeContext* particlePipelineGetComputeContext(struct ParticlePipeline* pipeline);
struct RenderContext* particlePipelineGetRenderContext(struct ParticlePipeline* pipeline);

void particlePipelineCompute(struct ParticlePipeline* pipeline);
void particlePipelineRender(struct ParticlePipeline* pipeline);

void particlePipelineAddToQueue(struct ParticlePipeline* pipeline, int instanceCount, void* uniformData, void* emitterData);

#endif // PARTICLE_PIPELINE_H
