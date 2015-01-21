#ifndef SHELL_H
#define SHELL_H

#define DEBUG 0

void out(const char* txt);

static inline void debug(char* txt) {
#if DEBUG
    out(txt);
#endif
}

#endif
