#ifndef VEC3_H
#define VEC3_H

struct vec3 {
	float x;
	float y;
	float z;
};

void vec3_init(struct vec3* d, float x, float y, float z);

float vec3_dot_self(const struct vec3* a);
float vec3_dot(const struct vec3* a, const struct vec3* b);
float vec3_norm(const struct vec3* d);
void vec3_normalize(struct vec3* d);
void vec3_add(struct vec3* d, const struct vec3* a, const struct vec3* b);
void vec3_sub(struct vec3* d, const struct vec3* a, const struct vec3* b);
void vec3_cross(struct vec3* d, const struct vec3* a, const struct vec3* b);

#endif // VEC3_H
