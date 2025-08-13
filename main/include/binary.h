#include <sys/types.h>

#ifndef BINARY_H
#define BINARY_H

typedef unsigned char bin_t;

#define BIN_WORD_LOW_BITS(num) (num & 0xFF)
#define BIN_WORD_HIGH_BITS(num) ((num & 0xFF00) >> 8)
#define THIRD_OCTET(num) ((num & 0xFF0000) >> 16)
#define FORTH_OCTET(num) ((num & 0xFF000000) >> 24)

#define HALF_H(num) (num & 0xF0) >> 4
#define HALF_L(num) (num & 0xF)

#define SAFE_INVERT(byte) (byte ^ ((1 << 8) - 1))

#define ARRAY_INTO_BIN_WORD(buff, ind) (buff[ind] << 8) | buff[ind+1]
#define ARRAY_INTO_BIN_DWORD(buff, ind) (buff[ind] << 12) | (buff[ind+1] << 8) | (buff[ind+2] << 4) | buff[ind+3]

#define BIN_WORD_INTO_ARRAY(word, buff, ind) \
    buff[ind] = BIN_WORD_LOW_BITS(word); \
    buff[ind+1] = BIN_WORD_HIGH_BITS(word); \

#define BIN_DWORD_INTO_ARRAY(dword, buff, ind) \
    buff[ind] = BIN_WORD_LOW_BITS(dword); \
    buff[ind+1] = BIN_WORD_HIGH_BITS(dword); \
    buff[ind+2] = THIRD_OCTET(dword); \
    buff[ind+3] = FORTH_OCTET(dword); \

u_int16_t array_to_bin_word(u_int8_t*, int);
u_int32_t array_to_bin_dword(u_int16_t*);

#endif