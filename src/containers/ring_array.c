#include "ring_array.h"

#include <stdlib.h>
#include <assert.h>

void ringArrayInit(struct RingArray* a, int size, int stride) {
	a->current = 0;
	a->stride = stride;
	a->size = size;
	a->data = malloc((size_t)a->size * a->stride);
	a->used = malloc(a->size * sizeof(bool));
}

void ringArrayDestroy(struct RingArray* a) {
	free(a->data);
	free(a->used);
}

int ringArrayAdd(struct RingArray* a) {
	while (a->used[a->current]) {
		a->current = (a->current + 1) % a->size;
	}
	a->used[a->current] = true;
	return a->current++;
}

void ringArrayRemove(struct RingArray* a, int index) {
	a->used[index] = false;
}

void* ringArrayGet(struct RingArray* a, int index) {
	assert(a->used[index]);
	return (void*)&(((char*)a->data)[a->stride * index]);
}
