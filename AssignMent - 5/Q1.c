#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <time.h>

int N = 1000;  

int** matrixMultiplicationSerialized(int** mat1 , int** mat2) {
    int rows = N;
    int **res = malloc(rows * sizeof(int *));
    for(int i = 0; i < N; i++) {
        res[i] = malloc(N * sizeof(int));
        for(int j = 0; j < N; j++) {
            res[i][j] = 0;  
            for(int k = 0; k < N; k++) {
                res[i][j] += (mat1[i][k] * mat2[k][j]);
            }
        }
    }
    return res;
}

int** matrixMultiplicationParallelCoarseGrained(int** mat1 , int** mat2) {
    int **res = malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++)
        res[i] = calloc(N, sizeof(int)); 

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int sum = 0;
            for (int k = 0; k < N; k++) {
                sum += mat1[i][k] * mat2[k][j];
            }
            res[i][j] = sum;  
        }
    }

    return res;
}


int** matrixMultiplicationParallelFineGrained(int** mat1 , int** mat2) {
    int **res = malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++)
        res[i] = calloc(N, sizeof(int)); 

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int sum = 0;
            #pragma omp parallel for reduction(+:sum)
            for (int k = 0; k < N; k++) {
                sum += mat1[i][k] * mat2[k][j];
            }
            res[i][j] = sum;
        }
    }

    return res;
}


int compareMat(int **mat1 , int **mat2)
{
    for(int i = 0;i<N;i++)
    {
        for(int j = 0;j<N;j++)
        {
            if(mat1[i][j] != mat2[i][j]) return 0;
        }
    }
    return 1;
}

void printMat(int** mat) {
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            printf("%d ", mat[i][j]); 
        }
        printf("\n");
    }
    printf("\n\n");
}

int main() {
    int rows = N;

    int **mat1 = malloc(rows * sizeof(int *));
    int **mat2 = malloc(rows * sizeof(int *));
    for(int i = 0; i < N; i++) {
        mat1[i] = malloc(N * sizeof(int));
        mat2[i] = malloc(N * sizeof(int));
    }

    int val = 1;
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            mat1[i][j] = val++;
        }
    }

    val = N * N;
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            mat2[i][j] = val--;
        }
    }

    // printf("Matrix 1:\n");
    // printMat(mat1);

    // printf("Matrix 2:\n");
    // printMat(mat2);
    int serialTime = 0;
    int parallelCoarseTime = 0;
    int parallelFineTime = 0;
    
     double start, end;

    // Serial
    start = omp_get_wtime();
    int **res = matrixMultiplicationSerialized(mat1, mat2);
    end = omp_get_wtime();
    double time_serial = end - start;

    // Parallel Coarse
    start = omp_get_wtime();
    int **res2 = matrixMultiplicationParallelCoarseGrained(mat1, mat2);
    end = omp_get_wtime();
    double time_coarse = end - start;

    // Parallel Fine
    start = omp_get_wtime();
    int **res3 = matrixMultiplicationParallelFineGrained(mat1, mat2);
    end = omp_get_wtime();
    double time_fine = end - start;


    // printf("Result (Mat1 x Mat2):\n");
    // printMat(res);
    // printf("Result Parallel: (Coarse) \n");
    // printMat(res2);

    int equal = compareMat(res, res2);
    if(equal) printf("Equal\n");
    else printf("Not Equal\n");

    // printf("Result Parallel: (Fine) \n");
    // printMat(res3);

    equal = compareMat(res, res3);
    if(equal) printf("Equal\n");
    else printf("Not Equal\n");


    printf("\n===== Execution Times (N=%d) =====\n", N);
    printf("Serial              : %.6f seconds\n", time_serial);
    printf("Parallel Coarse     : %.6f seconds\n", time_coarse);
    printf("Parallel Fine       : %.6f seconds\n", time_fine);

    return 0;
}
