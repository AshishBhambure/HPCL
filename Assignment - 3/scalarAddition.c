#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <float.h>

#define SIZE 9000000
#define SCALAR 1000000000

void initialize_array(int *arr, int n, int val) {
    for (int i = 0; i < n; i++) {
        arr[i] = val;
    }
}

void scalar_add_static(int *arr, int *copy, int n, int chunk, int threads, double *staticTime) {
    for (int i = 0; i < n; i++) arr[i] = copy[i];
    omp_set_num_threads(threads);
    double start = omp_get_wtime();

    #pragma omp parallel for schedule(static, chunk) 
    for (int i = 0; i < n; i++) {
        arr[i] += SCALAR;
    }

    double end = omp_get_wtime();
    *staticTime = end - start;
}

void scalar_add_dynamic(int *arr, int *copy, int n, int chunk, int threads, double *dynamicTime) {
    for (int i = 0; i < n; i++) arr[i] = copy[i];
    omp_set_num_threads(threads);
    double start = omp_get_wtime();

    #pragma omp parallel for schedule(dynamic, chunk) 
    for (int i = 0; i < n; i++) {
        arr[i] += SCALAR;
    }

    double end = omp_get_wtime();
    *dynamicTime = end - start;
}

int main() {
    int *arr = (int *)malloc(SIZE * sizeof(int));
    int *copy = (int *)malloc(SIZE * sizeof(int));
    initialize_array(copy, SIZE, 5);


    int threadCounts[] = {2, 4, 8};
    int chunkSizes[] = {1, 5, 10, 20, 50, 100};
    
    printf("\n--- STATIC SCHEDULE ANALYSIS ---\n");
    for (int t = 0; t < 3; t++) {
        int threads = threadCounts[t];
        printf("\nThreads = %d\n", threads);
        for (int c = 0; c < 6; c++) {
            int chunk = chunkSizes[c];
            double serialTime, staticTime;
            
            // Serial
            initialize_array(arr, SIZE, 5);
            double start = omp_get_wtime();
            for (int i = 0; i < SIZE; i++) arr[i] += SCALAR;
            double end = omp_get_wtime();
            serialTime = end - start;

            // Static
            scalar_add_static(arr, copy, SIZE, chunk, threads, &staticTime);
            double speedup = serialTime / staticTime;

            printf("Chunk = %-3d | Static Time = %.8f s | Speedup = %.4f\n", chunk, staticTime, speedup);
        }
    }

    printf("\n--- DYNAMIC SCHEDULE ANALYSIS ---\n");
    for (int t = 0; t < 3; t++) {
        int threads = threadCounts[t];
        printf("\nThreads = %d\n", threads);
        for (int c = 0; c < 6; c++) {
            int chunk = chunkSizes[c];
            double serialTime, dynamicTime;
            
            // Serial
            initialize_array(arr, SIZE, 5);
            double start = omp_get_wtime();
            for (int i = 0; i < SIZE; i++) arr[i] += SCALAR;
            double end = omp_get_wtime();
            serialTime = end - start;

            // Dynamic
            scalar_add_dynamic(arr, copy, SIZE, chunk, threads, &dynamicTime);
            double speedup = serialTime / dynamicTime;

            printf("Chunk = %-3d | Dynamic Time = %.8f s | Speedup = %.4f\n", chunk, dynamicTime, speedup);
        }
    }

    free(arr);
    free(copy);
    return 0;
}
