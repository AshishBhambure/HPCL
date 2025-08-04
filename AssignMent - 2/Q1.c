#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <float.h>  

int main() {

    printf("Default number of threads: %d\n", omp_get_max_threads());
    printf("Available processors: %d\n", omp_get_num_procs());
    
    long long n = 100, k = 1e9;
    int *arr = (int *)malloc(n * sizeof(int));
    int *copy = (int *)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        copy[i] = 11;
        arr[i] = copy[i];
    }

    double mini = DBL_MAX;
    int threadsForMinTime = -1;

    for (int t = 1; t <= 16; t++) {
        for (int i = 0; i < n; i++) {
            arr[i] = copy[i];
        }

        omp_set_num_threads(t);
        double tStart = omp_get_wtime();

        #pragma omp parallel for
        for (int i = 0; i < n; i++) {
            arr[i] += k;
        }

        double tEnd = omp_get_wtime();
        double timeTaken = tEnd - tStart;

        printf("Time taken: %.8f seconds (Threads :: %d)\n", timeTaken, t);

        if (timeTaken < mini) {
            mini = timeTaken;
            threadsForMinTime = t;
        }
    }

    printf("Min Time Got For Threads = %d\n", threadsForMinTime);

    free(arr);
    free(copy);
    return 0;
}
