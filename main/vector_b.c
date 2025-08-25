#include "include/vector_b.h"

int vector_init(vector_t* vec, size_t init_cap) {
    if (vec->d_size <= 0) {
        return BLOCK_SIZE_WRONG;
    }
    if (init_cap % vec->d_size != 0) {
        return DATA_NOT_ALIGNED;
    }
    vec->data = heap_caps_malloc(init_cap, MALLOC_CAP_DMA);
    if (!vec->data) {
        return DATA_INIT_FAIL;
    }
    vec->cap = init_cap;
    vec->lnt = 0;
    return 0;
}

int vector_add(vector_t* vec, void* src) {
    if (vec->lnt >= vec->cap) {
        vec->cap *= 2;
        vec->data = heap_caps_realloc(vec->data, vec->cap, MALLOC_CAP_DMA);
        if (!vec->data) {
            return DATA_INIT_FAIL;
        }
    }
    vec->data = memcpy(vec->data + (vec->lnt * vec->d_size), src, vec->d_size);
    vec->lnt += 1;
    return 0;
}

int vector_remove(vector_t* vec) {
    if (vec->lnt == 0) {
        return EMPTY_VECTOR;
    }
    memset(vec->data - (vec->lnt-1) * vec->d_size, 0x0, vec->d_size);
    vec->lnt -= 1;
    return 0;
}

void vector_destroy(vector_t* vec) {
    heap_caps_free(vec->data);
    vec->cap = 0;
    vec->lnt = 0;
    vec->d_size = 0;
}