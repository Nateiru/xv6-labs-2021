// xargs.c
#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int readline(char **pars, int n) {
    #define MAXBUF 1024 
    char buf[MAXBUF];
    int i = 0;
    for(;read(0, buf + i, 1); i++) {
        fprintf(1, buf);
        fprintf(1, "\n");
        if (i == MAXBUF) {
            fprintf(2, "Parameters are too long!\n");
            exit(1);
        }
        if (buf[i] == '\n') {
            buf[i] = 0;
            break;
        }
    }
    if (i == 0) {
        return -1; // 这行是空的
    }
    // 处理一行 以空格隔离的参数
    for (int j = 0; j < i; ) {
        if (n > MAXARG){
            fprintf(2, "Too much parameters!\n");
            exit(1);
        }
        int l, r;
        while (j < i && buf[j] == ' ') j++;
        l = j;
        while (j < i && buf[j] != ' ') j++;
        buf[r = j++] = 0;
        // l ~ r是一个参数
        pars[n] = malloc(strlen(buf + l) + 1);
        strcpy(pars[n], buf + l);
        n++;
    }
    return n;
}

int main (int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "Please enter commmand!\n");
        exit(1);
    }
    else {
        
        if (argc - 1 > MAXARG) {
            fprintf(2, "Too many argv!\n");
        }
        char *pars[MAXARG];
        // 读取 xargs 后面的参数
        for (int i = 1; i < argc; i++) {
            pars[i - 1] = malloc(strlen(argv[i]) + 1);
            strcpy(pars[i - 1], argv[i]);
        }
        // 从stdin读出管道传递的参数 每次读取一行
        int ed;
        while ((ed = readline(pars, argc - 1)) != -1) {
            pars[ed] = 0;
            if(fork() == 0) {
                exec(pars[0], pars);
                for (int i = argc - 1; i < ed; i++) {
                    free(pars[i]);
                }
                exit(0);
            }
            else {
                wait(0);
            }
        }
        for (int i = 1; i < argc; i++) 
            free(pars[i]);
        exit(0);
    }
    
}

