#include "ring_vector.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

void ringVectorInit(struct RingVector* v, int initialSize, int stride) {
	v->current = 0;
	v->stride = stride;
	v->count = 0;
	v->size = initialSize;
	v->data = malloc((size_t)v->size * v->stride);
	v->used = (bool*)calloc(v->size, sizeof(bool));
}

void ringVectorDestroy(struct RingVector* v) {
	free(v->data);
	free(v->used);
}

int ringVectorAdd(struct RingVector* v) {
	if (v->count == v->size) {
		v->size *= 2;
		void* re = realloc(v->data, (size_t)v->size * v->stride);
		if (re) {
			v->data = re;
		} else {
			assert(0);
		}
		re = realloc(v->used, v->size * sizeof(bool));
		if (re) {
			v->used = re;
			memset(&v->used[v->size / 2], 0, v->size / 2 * sizeof(bool));
		} else {
			assert(0);
		}
	}

	while (v->used[v->current]) {
		v->current = (v->current + 1) % v->size;
	}
	v->used[v->current] = true;
	++v->count;
	return v->current++;
}

void ringVectorRemove(struct RingVector* v, int index) {
	--v->count;
	v->used[index] = false;
}

void* ringVectorGet(struct RingVector* v, int index) {
	assert(v->used[index]);
	return (void*)&(((char*)v->data)[v->stride * index]);
}

bool ringVectorIsUsed(struct RingVector* v, int index) {
	return v->used[index];
}

void ringVectorClear(struct RingVector* v) {
	for (int i = 0; i < v->size; ++i) {
		v->used[i] = false;
	}
	v->count = 0;
}
