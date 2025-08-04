#include<stdio.h>
#include<omp.h>

void main(){
    printf(" Enter Number Of Times You Want To Run (Sequential)\n");
    int s = 0;
    scanf("%d",&s);
    for(int i = 0;i<s;i++){
        printf("Hello World! (Seq.) \n");
    }
     printf(" Enter Number Of Times You Want To Run (Parallel) \n");
     s = 0;
     scanf("%d",&s);
     omp_set_num_threads(s);
    #pragma omp parallel
    {
       printf("Hello , World !! :: Thread - %d \n", omp_get_thread_num());
    }
}