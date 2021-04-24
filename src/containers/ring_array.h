#ifndef RING_ARRAY_H
#define RING_ARRAY_H

#include <stdbool.h>

struct RingArray {
	void* data;
	bool* used;
	int current;
	int stride;
	int size;
};

void ringArrayInit(struct RingArray* a, int size, int stride);
void ringArrayDestroy(struct RingArray* a);

int ringArrayAdd(struct RingArray* a);
void ringArrayRemove(struct RingArray* a, int index);
void* ringArrayGet(struct RingArray* a, int index);

#endif // !RING_ARRAY_H
