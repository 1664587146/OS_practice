#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_ARGC 10
#define bool int 
#define TRUE 1
#define FALSE 0 

void mysys(char *command){
    int pid ;
    char *argv[MAX_ARGC + 1];
    char *string;
    string = (char*)calloc(strlen(command),sizeof(char));
    int argc = 0;

    if(command == NULL){
        exit(0);
    }

    int len = strlen(command);  //获取命令长度，将最后一位即'\n'改为'\0'即可
    if(len == 1)
        return ;
    else if(command[len - 1] == '\n')
        command[len - 1] = '\0';


    //printf("\n1\n%s\n",command);

    strcpy(string,command);
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
        buf[len - 1] = '\0';
        mysys(buf);
    }
    return 0;
}