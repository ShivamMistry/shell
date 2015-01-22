#include "strlib.h"

char *__tokbuffer;

void memset(char* ptr, int value, size_t num) {
    for (int i = 0; i < num; i++) {
        ptr[i] = value;
    }
}

static inline void memcpy(char* dest, char* src, size_t num) {
    for (size_t i = 0; i < num; i++) dest[i] = src[i];
}

size_t strlen(const char* buf) {
    size_t size = 0;
    while (buf[size++] != 0);
    return size;
}


void tok(char* str, char* dest, const char* delimiters) {
    if (str != 0) {
        __tokbuffer = str;
    }
    if (__tokbuffer == 0) {
        dest[0] = 0;
        return;
    }
    size_t dellen = strlen(delimiters);
    int ptr = 0;
    while (1) {
        char c = __tokbuffer[ptr++];
        if (c == 0) {
            if (__tokbuffer[ptr-1] == c) {
                __tokbuffer = 0;
                dest[0] = 0;
                return;
            }
            memcpy(dest, __tokbuffer, ptr-1);
            dest[ptr-1] = 0;
            __tokbuffer = 0;
            return;
        }
        for (int i = 0; i < dellen; i++) {
            if (ptr - 1 != 0 && c == delimiters[i]) {
                memcpy(dest, __tokbuffer, ptr-1);
                dest[ptr-1] = 0;
                __tokbuffer = __tokbuffer + ptr;
                return;
            }
        }
    }
}

