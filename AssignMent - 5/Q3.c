#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int N = 3;   


int* matVecMulSerialized(int** mat, int* vec) {
    int* res = malloc(N * sizeof(int));
    for (int i = 0; i < N; i++) {
        res[i] = 0;
        for (int j = 0; j < N; j++) {
            res[i] += mat[i][j] * vec[j];
        }
    }
    return res;
}


int* matVecMulParallelCoarse(int** mat, int* vec) {
    int* res = calloc(N, sizeof(int));

    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        int sum = 0;
        for (int j = 0; j < N; j++) {
            sum += mat[i][j] * vec[j];
        }
        res[i] = sum;
    }
    return res;
}

int* matVecMulParallelFine(int** mat, int* vec) {
    int* res = calloc(N, sizeof(int));

    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        int sum = 0;
        #pragma omp parallel for reduction(+:sum)
        for (int j = 0; j < N; j++) {
            sum += mat[i][j] * vec[j];
        }
        res[i] = sum;
    }
    return res;
}


void printVec(int* vec) {
    for (int i = 0; i < N; i++) {
        printf("%d ", vec[i]);
    }
    printf("\n");
}

void printMat(int** mat) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    int** mat = malloc(N * sizeof(int*));
    for (int i = 0; i < N; i++) {
        mat[i] = malloc(N * sizeof(int));
    }

    int val = 1;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            mat[i][j] = val++;
        }
    }

    int* vec = malloc(N * sizeof(int));
    for (int i = 0; i < N; i++) vec[i] = i + 1;   

    printf("Matrix:\n");
    printMat(mat);

    printf("Vector:\n");
    printVec(vec);

    int* res1 = matVecMulSerialized(mat, vec);
    int* res2 = matVecMulParallelCoarse(mat, vec);
    int* res3 = matVecMulParallelFine(mat, vec);

    printf("\nResult (Serialized):\n");
    printVec(res1);

    printf("\nResult (Parallel Coarse-Grained):\n");
    printVec(res2);

    printf("\nResult (Parallel Fine-Grained):\n");
    printVec(res3);

    return 0;
}
