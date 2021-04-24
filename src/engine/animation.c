#include "animation.h"

#include <stdlib.h>
#include <stdio.h>

void animationInit(struct Animation* anim, void* target, float begin, float end, int duration){
	anim->target = target;
	anim->begin = begin;
	anim->end = end;
	anim->duration = duration;
	anim->currentTime = 0;
}

void animationEngineInit(struct AnimationEngine* animEngine) {
	ringVectorInit(&animEngine->runningAnimations, 256, sizeof(struct Animation));
}

void animationEngineAdd(struct AnimationEngine* animEngine, struct Animation* anim) {
	int index = ringVectorAdd(&animEngine->runningAnimations);
	struct Animation* a = (struct Animation*)ringVectorGet(&animEngine->runningAnimations, index);
	a->ringVectorIndex = index;

	a->target = anim->target;
	a->begin = anim->begin;
	a->end = anim->end;
	a->duration = anim->duration;
	a->currentTime = 0;
}

void animationEngineDestroy(struct AnimationEngine* animEngine) {
	ringVectorDestroy(&animEngine->runningAnimations);
}

void animationEngineUpdate(struct AnimationEngine* animEngine, int delta) {
	for (int i = 0; i < animEngine->runningAnimations.size; ++i) {
		if (!ringVectorIsUsed(&animEngine->runningAnimations, i)) {
			continue;
		}
		struct Animation* a = (struct Animation*)ringVectorGet(&animEngine->runningAnimations, i);
		a->currentTime += delta;
		if (a->currentTime >= a->duration) {
			a->currentTime = a->duration;
			*a->target = a->end;
			ringVectorRemove(&animEngine->runningAnimations, a->ringVectorIndex);
			continue;
		}
		float d = (float)a->currentTime / (float)a->duration;
		*a->target = a->begin + (a->end - a->begin) * d;
	}
}
