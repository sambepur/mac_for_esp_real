#include "include/vector_b.h"

int vector_init(vector_t* vec, size_t init_cap) {
    if (vec->d_size <= 0) {
        return BAD_INIT_CAP;
    }
    if (heap_caps_get_free_size(vec->mem_type) < init_cap * vec->d_size) {
        return TOO_BIG_CHUNK_REQUESTED;
    }
    vec->_start = heap_caps_malloc(init_cap * vec->d_size, vec->mem_type);
    vec->data = vec->_start;
    if (!vec->_start) {
        switch (vec->mem_type)
        {
        case MALLOC_CAP_8BIT:
            return DRAM_FULL;
            break;
        case MALLOC_CAP_IRAM_8BIT:
            return IRAM_FULL;
            break;
        default:
            return UNKNOWN_ALLOCATION_ERROR;
            break;
        };
    }
    vec->cap = init_cap;
    vec->lnt = 0;
    return VEC_OK;
}

int vector_add(vector_t* vec, void* src) {
    if (vec->lnt >= vec->cap) {
        vec->cap *= 2;
        vec->_start = heap_caps_realloc(vec->_start, vec->cap * vec->d_size, vec->mem_type);
        vec->data = vec->_start;
        if (!vec->_start) {
            switch (vec->mem_type)
            {
            case MALLOC_CAP_8BIT:
                return DRAM_FULL;
                break;
            case MALLOC_CAP_IRAM_8BIT:
                return IRAM_FULL;
                break;
            default:
                return UNKNOWN_ALLOCATION_ERROR;
                break;
            };
        }
    }
    vec->data = memcpy(vec->_start + (vec->lnt * vec->d_size), src, vec->d_size);
    vec->lnt += 1;
    return VEC_OK;
}

int vector_remove(vector_t* vec) {
    if (vec->lnt == 0) {
        return EMPTY_VECTOR;
    }
    vec->data = memset(vec->data, 0x0, vec->d_size);
    vec->lnt -= 1;
    return VEC_OK;
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

bool vector_search(vector_t* vec, void* src) {
    for (register size_t i = 1; i<vec->lnt+1; i++) {
        if (memcmp(vec->_start, src, vec->d_size * i)) {
            return true;
        }
    }
    return false;
}