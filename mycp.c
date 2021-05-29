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
    int file_r,file_w;
    char buffer1[MAX_BUFFER];
    if(argc <= 2 ){
        perror("error\n");
        _exit(0);
    }
    if( (file_r = open(argv[1], O_RDONLY)) < 0){
        perror("打开失败\n");
        _exit(0);
    }

    if ( (file_w = open(argv[2],O_RDWR|O_CREAT,0700)) < 0){
        perror("打开失败\n");
        return -1;
    }

    buffer1[0]='\0';
    lseek(file_r,0,SEEK_SET);//定位到文件开头
    int res = 0;
    while((res = read(file_r,buffer1,BUFSIZ))>0 ){
        if( write(file_w,buffer1,res) < 0 ){
            perror("写入文件失败");
            return -1;
        }
    }

    //关闭文件
    if(close(file_r)<0){
        perror("关闭失败");
        _exit(0);
    }
    if(close(file_w)<0){
        perror("关闭失败");
        _exit(0);
    }
    return 0;
}