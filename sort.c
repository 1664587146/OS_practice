#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int array[] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };

#define NR_TOTAL 10
#define NR_CPU  2
#define NR_CHILD (NR_TOTAL / NR_CPU)



typedef struct Param{
    int *array;
    int length;
}Param;

typedef struct Res{
    int  *array;
}Res;

void *select_sort(void *arg){
    Param *param;
    param =(Param *)arg;
    int t;
    for (int i = 0; i < param -> length; i++){
        for (int  j = i; j < param -> length; j++){
            if ( param -> array[i] > param -> array[j]){
                t = param -> array[i];
                param -> array[i]= param -> array[j];
                param -> array[j]=t;
            }
        }
    }
    return NULL;
}

void merge(){
	int tmp[NR_TOTAL];
	int i = 0,j = NR_CHILD,t = 0,k;
	
    for(; i < NR_CHILD && j < NR_TOTAL;){
		if( array[i] < array[j] ){
			tmp[t ++] = array[i ++];
        }
		else{
			tmp[t ++] = array[j ++];
        }
    }

    if (i == NR_CHILD){
		for (k = j;k < 10;k++)
			tmp[t++] = array[k];
    }
	
    if (j == 10){
		for (k = i;k < 5;k ++)
			tmp[t ++] = array[k];
    }
	
    for (i=0;i<10;i ++)
		array[i]=tmp[i];
}

int main(){ 
    pthread_t workers[NR_CPU];
    Param params[NR_CPU]; 
    int i;
    //使用线程参数
    for (i = 0; i < NR_CPU; i++) {
        Param *param;
        param = &params[i];
        param -> length = NR_CHILD; 
        param -> array = &array[i * NR_CHILD];
        pthread_create(&workers[i], NULL, select_sort , param);
    }
    for (i = 0; i < NR_CPU; i++) {
        pthread_join(workers[i],NULL);
    }
    merge();

    for(i = 0 ; i < NR_TOTAL ; i++){
        printf("%d\t",array[i]);
    }
    printf("\n");
    return 0;
}