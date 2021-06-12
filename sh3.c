#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>


#define MAX_ARGC 16
#define MAX_COM 10
#define bool int 
#define TRUE 1
#define FALSE 0 

typedef struct command {
    int argc;
    char *argv[MAX_ARGC];
    char *input; // 用于重定向输入
    char *output; // 用于重定向输出
}Command;

int com_c = 0;
Command commands[MAX_COM];

// 字符串分割函数_having pipe
void parse_commands(char *line){
    com_c = 0;
    commands[com_c].argc = 0;
    int t = 0;

    commands[com_c].argv[0] = strtok(line,"|");
    com_c ++;

    //having pipe
    for (;(commands[com_c].argv[0] = strtok(NULL,"|")) != NULL ; com_c++){
    }
    //for every commands
    for (int i = 0; i < com_c; i++){

        char *last =NULL,*first = NULL,*string = NULL;
        string =(char *)calloc(strlen(commands[i].argv[0]),sizeof(char)); 
        commands[i].argc = 0;
        strcpy(string,commands[i].argv[0]);

        // printf("%s\n",string);
        first = strchr(string,'>');
        last = strrchr(string,'>');

        //no dump
        if (first ==NULL && last ==NULL){
            commands[i].argv[commands[i].argc] = strtok (string," ");
            commands[i].argc ++ ;
            while (commands[i].argv[commands[i].argc - 1] != NULL){
                commands[i].argv[commands[i].argc] = strtok(NULL, " ");
                commands[i].argc ++ ;
            } 
            commands[i].argv[commands[i].argc] = NULL;
        }
        else{
        //having dup
            char *source,*des_file,*argv;
            des_file = strtok(last ,">");
            des_file = strtok(des_file," ");
            source = strtok(string,">");
            argv = strtok(source," ");
            commands[i].argv[0] = argv;
            commands[i].argv[1] = source;
            commands[i].argv[2] = NULL;
            commands[i].argc = 2;
        }
    }
    line = NULL;

    // printf("%d",com_c);
    // for (int i = 0; i < com_c; i++){
    //     printf("\ncommond %d\n",i);
    //     for (int j = 0; j < commands[i].argc; j++){
    //         printf("%s\n",commands[i].argv[j]);
    //     }
    // }
}

// void test_dump(){
//     int i = 0, j = 0;
//     for (i = 0; i < com_c; i++) {
//         for (j = 0; j < commands[i].argc; j++) {
//             printf("%d,%s ", j, commands[i].argv[j]);
//         }
//         printf("\n");
//         if (commands[i].input)
//             printf("in:%s ", commands[i].input);
//         if (commands[i].output)
//             printf("out:%s ", commands[i].output);
//         printf("\n");
//     }
// }

void exec_simple(int child_count){

    // 重定向输入
    if(commands[child_count].input!=NULL){
            int infd;
            infd = open(commands[child_count].input,O_RDWR, 0666);
            dup2(infd,0);
            close(infd);
        }

    //重定向输出
    if (commands[child_count].output != NULL){
        int outfd;
        outfd =open(commands[child_count].output,O_CREAT | O_RDWR | O_TRUNC,0666);
        dup2(outfd, 1);
        close(outfd);
    }

    execvp(commands[child_count].argv[0], commands[child_count].argv);
}

void exec_pipe(int child_count){

    if (child_count == 0){
            exit(0);
        }
    
    // 单命令直接执行exec_simple
    if(com_c==1){
        exec_simple(0);
        exit(0);
    }

    pid_t pid;
    int fd[2];
    pipe(fd);

    // 递归执行多管道命令

    pid = fork();
    if (pid == 0) {
        //使用管道的写端
        dup2(fd[1], 1);   
        close(fd[0]);
        close(fd[1]);
        exec_pipe(child_count - 1);
    }
    //使用管道的读端
    dup2(fd[0], 0);
    close(fd[0]);
    close(fd[1]);
    exec_simple(child_count - 1);
    wait(NULL);
}



void mysys(char *line){
    int len = strlen(line);     //获取命令长度，将最后一位即'\n'改为'\0'即可
    if(len==1)                  
        return ;
    line[len - 1] = '\0';       
    parse_commands(line);       //划分命令
    // test_dump();
    if (strcmp(commands[0].argv[0], "exit") == 0) {             
        exit(0);
    } else if (strcmp(commands[0].argv[0], "cd") == 0) {
        if (chdir(commands[com_c - 1].argv[1]) == -1) {
            printf("cd:%s:", commands[com_c - 1].argv[1]);
            printf("No such file or directory\n");
        }
        return ;
    } else {
        pid_t pid;
        pid = fork();
        if (pid == 0)
            exec_pipe(com_c);
        wait(NULL);
    }

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