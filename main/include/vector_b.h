#include <stddef.h>
#include "esp_heap_caps.h"
#include <string.h>
#include <stdbool.h>

#ifndef VECTOR_B
#define VECTOR_B

/*
    In this vector implementation the limitation for block size is 15 bytes
    This is used to work only with unaligned DRAM and IRAM memory
    fields vector_t.s_size an vector_t.mem-type must be initalized befor call to vector_init()
*/

typedef enum {
    VEC_OK = 0,
    BAD_INIT_CAP,
    TOO_BIG_CHUNK_REQUESTED,
    DRAM_FULL,
    IRAM_FULL,
    UNKNOWN_ALLOCATION_ERROR,
    EMPTY_VECTOR
} vector_err_t;

#pragma pack(push, 2)
typedef struct {
    uint8_t d_size: 4; // size of object
    void* _start;
    void* data;
    size_t lnt;
    size_t cap;
    uint32_t mem_type; 
} vector_t;
#pragma pack(pop)

vector_err_t vector_init(vector_t*, size_t);
void* vector_get(vector_t*, size_t);
vector_err_t vector_add(vector_t*, void*);
vector_err_t vector_remove(vector_t*);
void vector_destroy(vector_t*);
void vector_print(vector_t*);
bool vector_search(vector_t*, void*);

#endif