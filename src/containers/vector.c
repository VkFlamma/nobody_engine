#include "vector.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

void vectorInit(struct Vector* v, int initialSize, int stride) {
	v->stride = stride;
	v->count = 0;
	v->size = initialSize;
	v->data = malloc(v->size * v->stride);
}

void vectorDestroy(struct Vector* v) {
	free(v->data);
}

int vectorAdd(struct Vector* v) {
	if (v->count == v->size) {
		v->size *= 2;
		void* re = realloc(v->data, v->size * v->stride);
		if (re) {
			v->data = re;
		} else {
			assert(0);
		}
	}
	return v->count++;
}

void* vectorGet(struct Vector* v, int index) {
	return (void*)&(((char*)v->data)[v->stride * index]);
}

void* vectorAddAndGet(struct Vector* v) {
	int i = vectorAdd(v);
	return vectorGet(v, i);
}

void vectorClear(struct Vector* v) {
	v->count = 0;
}

void vectorFree(struct Vector* v, int index) {
	vectorFreeRange(v, index, index);
}

void vectorFreeRange(struct Vector* v, int from, int to) {
	char* dst = &((char*)v->data)[from * v->stride];
	char* src = &((char*)v->data)[(to + 1) * v->stride];
	size_t size = v->count - to;
	if (size > 0) {
		memmove(dst, src, size * v->stride);
	}
	v->count = v->count - (to + 1) + from;
}
