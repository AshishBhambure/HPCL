#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <time.h>

int N = 2;  
int K = 1e4;

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
int **ScalarMul(int**mat , int k){
    
    #pragma omp parallel for collapse(2)
    for(int i = 0;i<N;i++)
    {
        for(int j = 0;j<N;j++)
        {
            mat[i][j]*=k;
        }
    }
    return mat;
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
    // int **mat2 = malloc(rows * sizeof(int *));
    for(int i = 0; i < N; i++) {
        mat1[i] = malloc(N * sizeof(int));
        // mat2[i] = malloc(N * sizeof(int));
    }

    int val = 1;
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            mat1[i][j] = val++;
        }
    }

    int** res = ScalarMul(mat1,K);

    printf("Res :: \n");
    printMat(res);

    return 0;
}
