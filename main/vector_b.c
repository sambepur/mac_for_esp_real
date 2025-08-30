#include "include/vector_b.h"

int vector_init(vector_t* vec, size_t init_cap) {
    if (vec->d_size <= 0) {
        return BLOCK_SIZE_WRONG;
    }
    vec->_start = heap_caps_malloc(init_cap * vec->d_size, MALLOC_CAP_8BIT);
    vec->data = vec->_start;
    if (!vec->_start) {
        return DATA_INIT_FAIL;
    }
    vec->cap = init_cap;
    vec->lnt = 0;
    return 0;
}

int vector_add(vector_t* vec, void* src) {
    if (vec->lnt >= vec->cap) {
        vec->cap *= 2;
        vec->_start = heap_caps_realloc(vec->_start, vec->cap * vec->d_size, MALLOC_CAP_8BIT);
        vec->data = vec->_start;
        if (!vec->data) {
            return DATA_INIT_FAIL;
        }
    }
    vec->data = memcpy(vec->_start + (vec->lnt * vec->d_size), src, vec->d_size);
    vec->lnt += 1;
    return 0;
}

int vector_remove(vector_t* vec) {
    if (vec->lnt == 0) {
        return EMPTY_VECTOR;
    }
    memset(vec->data, 0x0, vec->d_size);
    vec->lnt -= 1;
    return 0;
}

// treated as index request
void* vector_get(vector_t* vec, size_t pos) {
    if (pos < vec->cap)
    return vec->_start + (pos * vec->d_size);
    else return NULL;
}

void vector_destroy(vector_t* vec) {
    heap_caps_free(vec->_start);
    vec->cap = 0;
    vec->lnt = 0;
    vec->d_size = 0;
}
