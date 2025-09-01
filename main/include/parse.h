#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdbool.h>

#ifndef PARSE_H
#define PARSE_H

bool is_cached(uint8_t**, uint8_t*, size_t);
int cut(unsigned char*, unsigned char*, int, size_t);
size_t len(char*);

#endif