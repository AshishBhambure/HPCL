#include<stdio.h>
#include<omp.h>

void main(){
    int n = 20;
    // #pragma omp parallel for schedule(static,5)
    // for(int i = 0;i<n;i++){
    //   printf("Thread :  %d | Iteration : %d \n",omp_get_thread_num(),i);
    // }

    #pragma omp parallel for schedule(dynamic,1)
    for(int i = 0;i<n;i++){
      printf("Thread :  %d | Iteration : %d \n",omp_get_thread_num(),i);
    }


}