#ifndef VECTOR_H
#define VECTOR_H

struct Vector {
	void* data;
	int stride;
	int count;
	int size;
};

void vectorInit(struct Vector* v, int initialSize, int stride);
void vectorDestroy(struct Vector* v);

int vectorAdd(struct Vector* v);
void* vectorGet(struct Vector* v, int index);
void* vectorAddAndGet(struct Vector* v);

void vectorClear(struct Vector* v);
void vectorFree(struct Vector* v, int index);
void vectorFreeRange(struct Vector* v, int from, int to);

#endif // VECTOR_H
