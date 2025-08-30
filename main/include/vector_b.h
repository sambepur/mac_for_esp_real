#include <stddef.h>
#include "esp_heap_caps.h"
#include <string.h>

#ifndef VECTOR_B
#define VECTOR_B

#define DATA_INIT_FAIL 1
#define BLOCK_SIZE_WRONG 2
#define DATA_NOT_ALIGNED 3
#define EMPTY_VECTOR 4

/*
    In this vector implementation the limitation for block size is 15 bytes
*/

typedef struct {
    unsigned d_size: 4; // size of block
    void* _start;
    void* data;
    size_t lnt;
    size_t cap;
} __attribute__((packed)) vector_t;

int vector_init(vector_t*, size_t);
void* vector_get(vector_t*, size_t);
int vector_add(vector_t*, void*);
int vector_remove(vector_t*);
void vector_destroy(vector_t*);
void vector_print(vector_t*);

#endif