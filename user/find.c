#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
void find(char *path, char *filename) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    // 防御性编程
    if (read(fd, &de, sizeof(de)) != sizeof(de)){
        exit(1);
    }
    switch (st.type) {

    case T_FILE: 
        if (strcmp(de.name, filename) == 0) {
            printf("%s/%s\n", path, filename);
        }
        break;
    case T_DIR:
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
            fprintf(2, "find: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while(read(fd, &de, sizeof(de)) == sizeof(de)){
            if(de.inum == 0)
                continue;
            if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") ==0) 
                continue;

            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if(stat(buf, &st) < 0){
                fprintf(2, "find: cannot stat %s\n", buf);
                continue;
            }
            if (st.type == T_FILE){
                if (strcmp(de.name, filename) == 0){
                    printf("%s\n", buf);
                }
            }else if (st.type == T_DIR){
            // 如果当前目录是文件夹, 则递归处理, buf为下一级目录
                find(buf, filename);
            }
        }
        break;    
    default:
        break;
    }
}
int main(int argc, char *agrv[]) {
    if (argc < 3) {
        // 标准错误文件fid = 2
        fprintf(2, "Please enter a dir and a filename!\n");
        exit(1);
    }else {
        char *path = agrv[1];
        char *filename = agrv[2];
        find(path, filename);
        exit(0);
    }
}