#include "particle.h"

#include "pipelines/particle_pipeline.h"
#include "vulkan/context.h"

void particleUpdateUniform(struct Particle* particle, struct ParticlePipeline* pipeline, int deltaTime) {
	mat4_identity(&particle->uniform.modelMatrix);

	particle->emitter.startPosX = -7.0f;
	particle->emitter.startPosY = -7.0f;
	particle->emitter.startPosZ = -15.0f;
	particle->emitter.endPosX = 7.0f;
	particle->emitter.endPosY = -7.0f;
	particle->emitter.endPosZ = -15.0f;
	particle->emitter.startVelX = 0.0f;
	particle->emitter.startVelY = 0.5f;
	particle->emitter.startVelZ = 0.0f;
	particle->emitter.endVelX = 0.0f;
	particle->emitter.endVelY = 1.0f;
	particle->emitter.endVelZ = 0.0f;
	particle->emitter.startLife = 10000.0f;
	particle->emitter.endLife = 10000.0f;

	particle->emitter.readOffset = particle->emitter.writeOffset;
	particle->emitter.writeOffset = pipeline->firstInstance;

	particle->emitter.delta = (float)deltaTime / 1000000;
}

void particleInit(struct Particle* particle, struct ParticlePipeline* pipeline) {
	particle->count = 128;
	particle->emitter.writeOffset = 0;
	particle->initialized = false;
}

bool particleUpdate(struct Particle* particle, struct ParticlePipeline* pipeline, int deltaTime) {
	particleUpdateUniform(particle, pipeline, deltaTime);

	struct ComputeContext* computeCtx = particlePipelineGetComputeContext(pipeline);
	if (particle->initialized == false) {
		computeCtx->specialization = 0;
		particle->initialized = true;
	} else {
		computeCtx->specialization = 1;
	}

	struct RenderContext* renderCtx = particlePipelineGetRenderContext(pipeline);
	renderCtx->specialization = 0;

	particlePipelineAddToQueue(pipeline, particle->count, &particle->uniform, &particle->emitter);
	return true;
}

void particleDestroy(struct Particle* particle) {

}

void particleReload(struct Particle* particle, struct ParticlePipeline* pipeline) {
	particleDestroy(particle);
	particleInit(particle, pipeline);
}
