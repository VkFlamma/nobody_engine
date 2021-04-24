#ifndef MAT4_H
#define MAT4_H

struct vec4;

struct mat4 {
	float m[16];
};

void mat4_zero(struct mat4* d);

void mat4_identity(struct mat4* d);

void mat4_mul(struct mat4* d, struct mat4* a, struct mat4* b);

void mat4_transpose(struct mat4* d);

void mat4_mul_vec(struct vec4* d, struct mat4* m, struct vec4* v);

#endif // MAT4_H
