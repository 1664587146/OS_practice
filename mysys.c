#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_ARGC 10
void mysys(char *command){
    int pid ;
    char *argv[MAX_ARGC + 1];
    char *string;
    string = (char*)calloc(strlen(command),sizeof(char));
    strcpy(string,command);
    int argc = 0;

    if(command == NULL){
        exit(0);
    }

    //实现该函数，该函数执行一条命令，并等待该命令执行结束
    argv[argc] = strtok (string," ");
    argc ++ ;
    while (argv[argc - 1] != NULL){
        argv[argc] = strtok(NULL, " ");
        argc ++ ;
    } 
    argv[argc] = NULL;
    //spil string
    
    //make new fork
    /*printf("\n%d\n",argc);
    for(int i = 0;i<argc;i++){
        printf("\n%s\n",argv[i]);
    }*/
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
    wait(NULL);
}

int main()
{
    printf("--------------------------------------------------\n");
    mysys("echo HELLO WORLD");
    printf("--------------------------------------------------\n");
    mysys("ls /");
    printf("--------------------------------------------------\n");
    return 0;
}