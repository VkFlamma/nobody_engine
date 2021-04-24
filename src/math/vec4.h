#ifndef VEC4_H
#define VEC4_H

struct vec4 {
	float x;
	float y;
	float z;
	float w;
};

void vec4_init(struct vec4* v, float x, float y, float z, float w);

float vec4_length(struct vec4* v);

#endif // VEC4_H
