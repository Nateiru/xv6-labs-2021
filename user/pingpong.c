// pingpong.c 
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    // 创建管道会得到一个长度为 2 的 int 数组
	// 其中 0 为用于从管道读取数据的文件描述符，1 为用于向管道写入数据的文件描述符
    int pp2c[2], pc2p[2];
    pipe(pp2c); // 创建用于 父进程 -> 子进程 的管道
    pipe(pc2p); // 创建用于 子进程 -> 父进程 的管道

    if (fork() != 0) { // parent process
        write(pp2c[1], "!", 1); // 1. 父进程首先向发出该字节
        char buf;
        read(pc2p[0], &buf, 1); // 2. 父进程读取子进程
        printf("%d: received pong\n", getpid()); // 5. 子进程收到数据，read 返回，输出 pong
		wait(0); 
    }else {
        char buf;
        read(pp2c[0], &buf, 1);
        printf("%d: received ping\n", getpid()); // 5. 子进程收到数据，read 返回，输出 pong
        write(pc2p[1], &buf, 1); 
    }
    exit(0);
}