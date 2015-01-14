#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>

#define STDIN   0
#define STDOUT  1
#define STDERR  2

#define DEBUG 0

#define PROMPT "hell > "

char *tokbuffer;

size_t strlen(const char* buf) {
    size_t size = 0;
    while (buf[size++] != 0);
    return size;
}

void memset(char* ptr, int value, size_t num) {
    for (int i = 0; i < num; i++) {
        ptr[i] = value;
    }
}

void tok(char* str, char* dest, const char* delimiters) {
    if (str != 0) {
        tokbuffer = str;
    }
    if (tokbuffer == 0) {
        dest[0] = 0;
        return;
    }
    size_t dellen = strlen(delimiters);
    int ptr = 0;
    while (1) {
        char c = tokbuffer[ptr++];
        if (c == 0) {
            memcpy(dest, tokbuffer, ptr-1);
            dest[ptr-1] = 0;
            tokbuffer = 0;
            return;
        }
        for (int i = 0; i < dellen; i++) {
            if (c == delimiters[i]) {
                memcpy(dest, tokbuffer, ptr-1);
                dest[ptr-1] = 0;
                tokbuffer = tokbuffer + ptr;
                return;
            }
        }
    }
}

void write_prompt(void) {
    //TODO: grab prompt from an env var
    write(STDOUT, PROMPT, strlen(PROMPT));
}

void out(const char* buf) {
    write(STDOUT, buf, strlen(buf));
}

int main(void) {
    char strbuf[512];
    size_t buflen = 0;
    memset(strbuf,0,512);
    while(1) {
        memset(strbuf, 0, buflen);
        write_prompt(); 
        size_t pos = 0;
        char c;
        do {
            read(STDIN, &c, 1);
            strbuf[pos++] = c;
            // TODO: prevent overflow
        } while (c != '\n');
#if DEBUG
        out(strbuf);
#endif
        buflen = strlen(strbuf);
        char token[buflen];
        memset(token, 0, buflen);
        tok(strbuf, token, " \n");
        if (token[0] != 0) {
            //TODO: actually test if program/command exists
            void* program = 0;
            if (program) {
                while (token[0] != 0) {
#if DEBUG
                    out(token);
                    out(": found token\n");
#endif
                    tok(0, token, " \n");
                    // TODO: add args to a list and pass to program
                }
            } else {
                out(token);
                out(": command not found\n");
            }
        }
    }
    return 0;
}
