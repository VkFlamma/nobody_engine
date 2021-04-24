#ifndef PARTICLE_H
#define PARTICLE_H

#include <stdbool.h>

#include "pipelines/particle_pipeline.h"
#include "math/transform.h"

struct Particle {
	struct ParticleUniform uniform;
	struct ParticleEmitter emitter;
	struct Transform transform;
	int count;
	int pipelineIndex;
	bool initialized;
};

void particleInit(struct Particle* particle, struct ParticlePipeline* pipeline);
bool particleUpdate(struct Particle* particle, struct ParticlePipeline* pipeline, int deltaTime);
void particleDestroy(struct Particle* particle);
void particleReload(struct Particle* particle, struct ParticlePipeline* pipeline);

#endif // PARTICLE_H
