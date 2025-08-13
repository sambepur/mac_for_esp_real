#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#ifndef PARSE_H
#define PARSE_H

int cut(unsigned char*, unsigned char*, int, size_t);
size_t len(char*);

#endif