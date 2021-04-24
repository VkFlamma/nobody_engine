#ifndef RING_VECTOR_H
#define RING_VECTOR_H

#include <stdbool.h>

struct RingVector {
	void* data;
	bool* used;
	int current;
	int stride;
	int count;
	int size;
};

void ringVectorInit(struct RingVector* v, int initialSize, int stride);
void ringVectorDestroy(struct RingVector* v);

int ringVectorAdd(struct RingVector* v);
void ringVectorRemove(struct RingVector* v, int index);
void* ringVectorGet(struct RingVector* v, int index);
bool ringVectorIsUsed(struct RingVector* v, int index);

void ringVectorClear(struct RingVector* v);

#endif // RING_VECTOR_H
