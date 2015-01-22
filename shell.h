#ifndef SHELL_H
#define SHELL_H

#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#if DEBUG
#include <string.h>
#endif

#define STDIN   0
#define STDOUT  1
#define STDERR  2

#define PROMPT "hell > "

#define DEBUG 0

void out(const char* txt);

static inline void debug(char* txt) {
#if DEBUG
    out(txt);
#endif
}

#endif
