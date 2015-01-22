#ifndef STRLIB_H
#define STRLIB_H

#include <stdint.h>
#include <stddef.h>

static inline void memcpy(char* dest, char* src, size_t num);

void memset(char* ptr, int value, size_t num);

size_t strlen(const char *str);

void tok(char *str, char *dest, const char *delimiters);

#endif
