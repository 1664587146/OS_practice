#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define MAX_BUFFER 800
//int open(const char *pathname,int flags);
//int open(const char *pathname,int flags,mode_t mode);
//int close(int fd);
//ssize_t read(int fd, void *addr, size_t count);

int main(int argc,char* argv[]){
    int file;
    char buffer1[MAX_BUFFER];
    if(argc <= 1 ){
        perror("error\n");
        _exit(0);
    }
    if( (file = open(argv[1], O_RDONLY)) < 0){
        perror("打开失败\n");
        _exit(0);
    }

    buffer1[0]='\0';
    lseek(file,0,SEEK_SET);//定位到文件开头
    while(read(file,buffer1,BUFSIZ)>0 ){
        printf("%s",buffer1);
    }

    //关闭文件
    if(close(file)<0){
        perror("关闭失败");
        _exit(0);
    }
    return 0;
}