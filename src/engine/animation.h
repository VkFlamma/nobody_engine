#ifndef ANIMATION_H
#define ANIMATION_H

#include <stdint.h>

#include "containers/ring_vector.h"

struct Animation {
	float* target;
	float begin;
	float end;
	int duration;
	int currentTime;
	int ringVectorIndex;
};

struct AnimationEngine {
	struct RingVector runningAnimations;
};

void animationInit(struct Animation* anim, void* target, float begin, float end, int duration);

void animationEngineInit(struct AnimationEngine* animEngine);
void animationEngineAdd(struct AnimationEngine* animEngine, struct Animation* anim);
void animationEngineDestroy(struct AnimationEngine* animEngine);

void animationEngineUpdate(struct AnimationEngine* animEngine, int delta);

#endif // ANIMATION_H
