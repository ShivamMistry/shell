#include "shell.h"
#include "strlib.h"

void write_prompt(void) {
    //TODO: grab prompt from an env var
    write(STDOUT, PROMPT, strlen(PROMPT));
}

void out(const char *buf) {
    write(STDOUT, buf, strlen(buf));
}

void parse_cmd(char *cmd) {
    int cmdlen = strlen(cmd);
    char token[cmdlen];
    memset(token, 0, cmdlen);
    tok(cmd, token, " \n");
    if (token[0] != 0) {
        size_t max_args = 50;
        char **argv = (char**) malloc(sizeof(char*) * max_args);
        size_t argc = 0;
        while (token[0] != 0) {
            size_t token_len = strlen(token) + 1;
            char *ptr = (char*) malloc(sizeof(char) * token_len);
            memcpy(ptr, token, token_len);
            if (argc >= max_args) {
                max_args *= 2;
                realloc(argv, max_args);
            }
            argv[argc++] = ptr;
            tok(0, token, " \n");
        }
        argv[argc] = 0;
        pid_t pid = fork();
        size_t loc;
        if (pid == 0) {
            // child
            int val = execvp(argv[0], argv);
            out("No such command: ");
            out(argv[0]);
            out("\n");
        } else {
            pid_t tpid;
            do {
                tpid = wait(&loc);
            } while(tpid != pid);
            for (size_t i = 0; i < argc; i++) {
                if (argv[i] != 0) {
                    free(argv[i]);
                }
            }
            free(argv);
        }

    }

}

int main(void) {
    size_t buf_size = 128;
    char* strbuf = (char *) malloc(sizeof(char) * buf_size);
    size_t buflen = 0;
    memset(strbuf,0,buf_size);
    while(1) {
        memset(strbuf, 0, buflen);
        write_prompt(); 
        size_t pos = 0;
        char c;
        do {
            read(STDIN, &c, 1);
            if (pos >= buf_size) {
                buf_size *= 2;
                realloc(strbuf, buf_size);
            }
            strbuf[pos++] = c;
            // TODO: prevent overflow
        } while (c != '\n');
        debug(strbuf);
        buflen = strlen(strbuf);
        parse_cmd(strbuf);
    }
    return 0;
}
