// primes.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
void sieve(int fd) {
    int prime;
    read(fd, &prime, sizeof(int));
    printf("prime %d\n", prime);

    int num;
    if (read(fd, &num, sizeof(int)) > 0) {
        int p[2];
        pipe(p);
        if(fork() == 0) {
            close(p[1]);
            sieve(p[0]);
            close(p[0]);
        }else {
            close(p[0]);
            do {
                if (num % prime != 0) {
                    write(p[1], &num, sizeof(int));
                }
            }while(read(fd, &num, sizeof(int)) > 0);
            close(p[1]);
            wait(0);
        }
    }
    exit(0);
}
int main () {
    // 主进程 管道 0是读取 1是写入
    int p[2];
    pipe(p);
    if (fork() == 0) { // child process
        close(p[1]);
        sieve(p[0]);
        close(p[0]);

    }else {   // parent process
        close(p[0]);
        for (int i = 2; i <= 35; i++) {
            write(p[1], &i, sizeof (i));
        }
        close(p[1]);
        wait(0);
    } 
    exit(0); 
}