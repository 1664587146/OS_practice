#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_ARGC 10
#define bool int 
#define TRUE 1
#define FALSE 0 


void mysys(char *command){
    int pid ;
    char *last =NULL,*first = NULL;
    char *argv[MAX_ARGC + 1];
    char *string;
    int len = strlen(command);  //获取命令长度，将最后一位即'\n'改为'\0'即可
    string = (char*)calloc(len,sizeof(char));
    int argc = 0;

    if(command == NULL){
        exit(0);
    }

    if(len == 1)
        return ;
    else if(command[len - 1] == '\n')
        command[len - 1] = '\0';


        //printf("\n1\n%s\n",command);

    strcpy(string,command);
    first = strchr(string,'>');
    last = strrchr(string,'>');
    if (first ==NULL && last ==NULL){
        argv[argc] = strtok (string," ");
        argc ++ ;
        while (argv[argc - 1] != NULL){
            argv[argc] = strtok(NULL, " ");
            argc ++ ;
        } 
        argv[argc] = NULL;
        //split string
        
        if(strcmp(argv[0],"cd") == 0){
            if((chdir(argv[1])) !=0 )
                printf("cd:%s: No such file or directory\n", argv[1]);
        }
        else if(strcmp(argv[0],"exit") == 0){
            exit(0);
        }
        else{
            pid = fork(); 
            if (pid < 0){ 
                perror("error"); 
                exit(0);
            }
            else if (pid == 0){
                int res = execvp(argv[0],argv); 
                if(res < 0){
                    perror("error"); 
                    exit(0);
                }
            }
        }
    }
    else{
        argv[0] = (char*)calloc(10,sizeof(char));
        argv[1] = (char*)calloc(1000,sizeof(char));
        argv[2] = (char*)calloc(1000,sizeof(char));
        char *source,*des_file;
        des_file = strtok(last ,">");
        des_file = strtok(des_file," ");
        source = strtok(string,">");
        strncpy(argv[0],source , 4);
        source = source + 5;
        strcpy(argv[1],source);
        argv[2] = NULL;
        pid = fork();
        if (pid == 0) {
            int file;
            if (des_file != NULL) {
                if( (file = open(des_file,O_RDWR|O_CREAT,0700)) < 0){
                    perror("打开失败\n");
                    _exit(0);
            }
                dup2(file, 1);
            }
            close(file);
            execvp(argv[0], argv);
        }
        
        free(argv[0]);
        free(argv[1]);
        free(argv[2]);
        
        wait(NULL);
    }
    wait(NULL);
}

int main()
{
    char buf[80];
    int count;
    while (TRUE) {
        printf("> ");
        fgets(buf, 80, stdin);
        int len = strlen(buf);
        buf[len] = '\0';
        mysys(buf);
    }
    return 0;
}