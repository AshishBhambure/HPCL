#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
#include<time.h>
#include<float.h>

void fillMatrix(int** mat, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            mat[i][j] = rand() % 100;  
}

void main()
{
    int nList[] = {250,500,750,1000,2000};
     srand(time(NULL));

    for(int a = 0;a<5;a++){
       int n = nList[a];
        int** matrix1 = (int**)malloc(n * sizeof(int*));
        int** matrix2 = (int**)malloc(n * sizeof(int*));

        for (int i = 0; i < n; i++) {
            matrix1[i] = (int*)malloc(n * sizeof(int));
            matrix2[i] = (int*)malloc(n * sizeof(int));
        }

        fillMatrix(matrix1, n);
        fillMatrix(matrix2, n);

        // Let us Go Now 

        double mini = DBL_MAX;
        int threadsforMinTime = 0;
        for(int t = 1;t<=16;t++)
        {
            omp_set_num_threads(t);
           int** matrix3 = (int**)malloc(n * sizeof(int*));
        
           for (int i = 0; i < n; i++) {
            matrix3[i] = (int*)malloc(n * sizeof(int));
            }
   
            double tin = omp_get_wtime();
            #pragma omp parallel for collapse(2)
            for(int i = 0;i<n;i++){
                for(int j = 0;j<n;j++){
                    matrix3[i][j] = matrix1[i][j] + matrix2[i][j];
                }
              }
              double tout = omp_get_wtime();

              if((tout - tin) < mini)
              {
                mini = tout-tin;
                threadsforMinTime = t;
              }

            printf("N = %d | Threads = %d | Time = %0.9f \n", n,t,tout-tin);
        }
      
         printf("N: %d | Min Time : %0.9f | Min Threads : %d",n,mini,threadsforMinTime);

        printf("\n\n");

    }

}