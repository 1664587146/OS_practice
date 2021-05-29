#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NR_TOTAL 1000000
#define NR_CPU  4
#define NR_CHILD (NR_TOTAL / NR_CPU)

typedef struct Param{
    int start;
    int end;
}Param;

typedef struct Res{
    float sum;
}Res;

void *compute(void *arg){
    Param *param;
    param =(Param *)arg;
    Res *res;
    int flag;
    float sum;
    if( ( param -> start + 1) % 2 == 0){
        flag = -1;
    }
    else{
        flag = 1;
    }
    for (int i = param -> start + 1; i < param -> end; i+=2){
        sum += 1.0 / i*flag;
        flag = -flag;
    }    
    res = malloc(sizeof(Res));
    res -> sum = sum;
    return res;
}


int main(){ 
    pthread_t workers[NR_CPU];
    Param params[NR_CPU]; 
    int i;
    //使用线程参数
    for (i = 0; i < NR_CPU; i++) {
        Param *param;
        param = &params[i];
        param->start = i * NR_CHILD; 
        param->end = (i + 1) * NR_CHILD;
        pthread_create(&workers[i], NULL, compute, param);
    }

    float PI = 0;
    for (i = 0; i < NR_CPU; i++) {
        Res *res;
        pthread_join(workers[i], (void **)&res);
        PI += res->sum;
        // printf("\n%f\n",res -> sum);
        free(res);
    }
    PI*=4;

    printf("PI = %f\n", PI);
    return 0;
}