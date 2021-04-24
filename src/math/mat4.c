#include "math/mat4.h"

#include <immintrin.h>
#include <assert.h>

#include "math/vec4.h"
#include "util/defines.h"

void mat4_zero(struct mat4* d) {
    d->m[0] = 0.0f;
    d->m[1] = 0.0f;
    d->m[2] = 0.0f;
    d->m[3] = 0.0f;
    d->m[4] = 0.0f;
    d->m[5] = 0.0f;
    d->m[6] = 0.0f;
    d->m[7] = 0.0f;
    d->m[8] = 0.0f;
    d->m[9] = 0.0f;
    d->m[10] = 0.0f;
    d->m[11] = 0.0f;
    d->m[12] = 0.0f;
    d->m[13] = 0.0f;
    d->m[14] = 0.0f;
    d->m[15] = 0.0f;
}

void mat4_identity(struct mat4* d) {
    d->m[0] = 1.0f;
    d->m[1] = 0.0f;
    d->m[2] = 0.0f;
    d->m[3] = 0.0f;
    d->m[4] = 0.0f;
    d->m[5] = 1.0f;
    d->m[6] = 0.0f;
    d->m[7] = 0.0f;
    d->m[8] = 0.0f;
    d->m[9] = 0.0f;
    d->m[10] = 1.0f;
    d->m[11] = 0.0f;
    d->m[12] = 0.0f;
    d->m[13] = 0.0f;
    d->m[14] = 0.0f;
    d->m[15] = 1.0f;
}

void mat4_mul(struct mat4* d, struct mat4* a, struct mat4* b) {
#ifdef USE_SIMD
    __m128 row[4], sum[4];
    row[0] = _mm_load_ps(&a->m[0]);
    row[1] = _mm_load_ps(&a->m[4]);
    row[2] = _mm_load_ps(&a->m[8]);
    row[3] = _mm_load_ps(&a->m[12]);
    sum[0] = _mm_mul_ps(_mm_set1_ps(b->m[0]), row[0]);
    sum[0] = _mm_add_ps(_mm_mul_ps(_mm_set1_ps(b->m[1]), row[1]), sum[0]);
    sum[0] = _mm_add_ps(_mm_mul_ps(_mm_set1_ps(b->m[2]), row[2]), sum[0]);
    sum[0] = _mm_add_ps(_mm_mul_ps(_mm_set1_ps(b->m[3]), row[3]), sum[0]);
    sum[1] = _mm_mul_ps(_mm_set1_ps(b->m[4]), row[0]);
    sum[1] = _mm_add_ps(_mm_mul_ps(_mm_set1_ps(b->m[5]), row[1]), sum[1]);
    sum[1] = _mm_add_ps(_mm_mul_ps(_mm_set1_ps(b->m[6]), row[2]), sum[1]);
    sum[1] = _mm_add_ps(_mm_mul_ps(_mm_set1_ps(b->m[7]), row[3]), sum[1]);
    sum[2] = _mm_mul_ps(_mm_set1_ps(b->m[8]), row[0]);
    sum[2] = _mm_add_ps(_mm_mul_ps(_mm_set1_ps(b->m[9]), row[1]), sum[2]);
    sum[2] = _mm_add_ps(_mm_mul_ps(_mm_set1_ps(b->m[10]), row[2]), sum[2]);
    sum[2] = _mm_add_ps(_mm_mul_ps(_mm_set1_ps(b->m[11]), row[3]), sum[2]);
    sum[3] = _mm_mul_ps(_mm_set1_ps(b->m[12]), row[0]);
    sum[3] = _mm_add_ps(_mm_mul_ps(_mm_set1_ps(b->m[13]), row[1]), sum[3]);
    sum[3] = _mm_add_ps(_mm_mul_ps(_mm_set1_ps(b->m[14]), row[2]), sum[3]);
    sum[3] = _mm_add_ps(_mm_mul_ps(_mm_set1_ps(b->m[15]), row[3]), sum[3]);
    _mm_store_ps(&d->m[0], sum[0]);
    _mm_store_ps(&d->m[4], sum[1]);
    _mm_store_ps(&d->m[8], sum[2]);
    _mm_store_ps(&d->m[12], sum[3]);
#else
    d->m[0] = a->m[0] * b->m[0] + a->m[4] * b->m[1] + a->m[8] * b->m[2] + a->m[12] * b->m[3];
    d->m[1] = a->m[1] * b->m[0] + a->m[5] * b->m[1] + a->m[9] * b->m[2] + a->m[13] * b->m[3];
    d->m[2] = a->m[2] * b->m[0] + a->m[6] * b->m[1] + a->m[10] * b->m[2] + a->m[14] * b->m[3];
    d->m[3] = a->m[3] * b->m[0] + a->m[7] * b->m[1] + a->m[11] * b->m[2] + a->m[15] * b->m[3];
    d->m[4] = a->m[0] * b->m[4] + a->m[4] * b->m[5] + a->m[8] * b->m[6] + a->m[12] * b->m[7];
    d->m[5] = a->m[1] * b->m[4] + a->m[5] * b->m[5] + a->m[9] * b->m[6] + a->m[13] * b->m[7];
    d->m[6] = a->m[2] * b->m[4] + a->m[6] * b->m[5] + a->m[10] * b->m[6] + a->m[14] * b->m[7];
    d->m[7] = a->m[3] * b->m[4] + a->m[7] * b->m[5] + a->m[11] * b->m[6] + a->m[15] * b->m[7];
    d->m[8] = a->m[0] * b->m[8] + a->m[4] * b->m[9] + a->m[8] * b->m[10] + a->m[12] * b->m[11];
    d->m[9] = a->m[1] * b->m[8] + a->m[5] * b->m[9] + a->m[9] * b->m[10] + a->m[13] * b->m[11];
    d->m[10] = a->m[2] * b->m[8] + a->m[6] * b->m[9] + a->m[10] * b->m[10] + a->m[14] * b->m[11];
    d->m[11] = a->m[3] * b->m[8] + a->m[7] * b->m[9] + a->m[11] * b->m[10] + a->m[15] * b->m[11];
    d->m[12] = a->m[0] * b->m[12] + a->m[4] * b->m[13] + a->m[8] * b->m[14] + a->m[12] * b->m[15];
    d->m[13] = a->m[1] * b->m[12] + a->m[5] * b->m[13] + a->m[9] * b->m[14] + a->m[13] * b->m[15];
    d->m[14] = a->m[2] * b->m[12] + a->m[6] * b->m[13] + a->m[10] * b->m[14] + a->m[14] * b->m[15];
    d->m[15] = a->m[3] * b->m[12] + a->m[7] * b->m[13] + a->m[11] * b->m[14] + a->m[15] * b->m[15];
#endif
}

void mat4_transpose(struct mat4* d) {
    __m128 row[4];
    row[0] = _mm_load_ps(&d->m[0]);
    row[1] = _mm_load_ps(&d->m[4]);
    row[2] = _mm_load_ps(&d->m[8]);
    row[3] = _mm_load_ps(&d->m[12]);
                                      
    __m128 _Tmp0 = _mm_shuffle_ps(row[0], row[1], 0x44);
    __m128 _Tmp2 = _mm_shuffle_ps(row[0], row[1], 0xEE);
    __m128 _Tmp1 = _mm_shuffle_ps(row[2], row[3], 0x44);
    __m128 _Tmp3 = _mm_shuffle_ps(row[2], row[3], 0xEE);

    row[0] = _mm_shuffle_ps(_Tmp0, _Tmp1, 0x88);
    row[1] = _mm_shuffle_ps(_Tmp0, _Tmp1, 0xDD);
    row[2] = _mm_shuffle_ps(_Tmp2, _Tmp3, 0x88);
    row[3] = _mm_shuffle_ps(_Tmp2, _Tmp3, 0xDD);

    _mm_store_ps(&d->m[0], row[0]);
    _mm_store_ps(&d->m[4], row[1]);
    _mm_store_ps(&d->m[8], row[2]);
    _mm_store_ps(&d->m[12], row[3]);
}

void mat4_mul_vec(struct vec4* d, struct mat4* m, struct vec4* v) {
    assert(d != v);
    d->x = m->m[0] * v->x + m->m[4] * v->y + m->m[8] * v->z + m->m[12] * v->w;
    d->y = m->m[1] * v->x + m->m[5] * v->y + m->m[9] * v->z + m->m[13] * v->w;
    d->z = m->m[2] * v->x + m->m[6] * v->y + m->m[10] * v->z + m->m[14] * v->w;
    d->w = m->m[3] * v->x + m->m[7] * v->y + m->m[11] * v->z + m->m[15] * v->w;
}
