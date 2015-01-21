#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>

#include "shell.h" 

#if DEBUG
#include <string.h>
#endif

#define STDIN   0
#define STDOUT  1
#define STDERR  2


#define PROMPT "hell > "

char* __tokbuffer;

size_t strlen(const char* buf) {
    size_t size = 0;
    while (buf[size++] != 0);
    return size;
}

#if !DEBUG
void memset(char* ptr, int value, size_t num) {
    for (int i = 0; i < num; i++) {
        ptr[i] = value;
    }
}

static inline void memcpy(char* dest, char* src, size_t num) {
    for (size_t i = 0; i < num; i++) dest[i] = src[i];
}
#endif

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
            memcpy(dest, __tokbuffer, ptr-1);
            dest[ptr-1] = 0;
            __tokbuffer = 0;
            return;
        }
        for (int i = 0; i < dellen; i++) {
            if (c == delimiters[i]) {
                memcpy(dest, __tokbuffer, ptr-1);
                dest[ptr-1] = 0;
                __tokbuffer = __tokbuffer + ptr;
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

void parse_cmd(char* cmd) {

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
        debug(strbuf);
        buflen = strlen(strbuf);
        char token[buflen];
        memset(token, 0, buflen);
        tok(strbuf, token, " \n");
        if (token[0] != 0) {
            char argbuf[50][255];
            size_t argc = 0;
            memcpy(argbuf[argc++], token, strlen(token)); 
            while (token[0] != 0) {
                debug(token);
                debug(": found token\n");
                tok(0, token, " \n");
                memcpy(argbuf[argc++], token, strlen(token));
            }
            char** argv = argbuf;
            argv[argc++] = 0;
           
            pid_t pid = fork();
            size_t loc;
            if (pid == 0) {
                // child
#if DEBUG
                char str[1024];
                sprintf(str, "Spawned %s with pid: %d\n", argv[0], pid);
                debug(str);
#endif
                int val = execvp(argv[0], argv);
                out("Failed to execute: ");
                out(argv[0]);
                out("\n");
                sprintf(str, "Returned: %d\n", val);
                debug(str);
                if (val == -1) {
                    int errsv = errno;
                    sprintf(str, "Error with %s: %d\n", argv[0], errsv);
                    debug(str);
                }
                
            } else {
#if DEBUG
                char str[255];
                sprintf(str, "Our pid is: %d\n", pid);
                debug(str);
#endif
                pid_t tpid;
                do {
                    tpid = wait(&loc);
                } while(tpid != pid);
            }

        }
    }
    return 0;
}
