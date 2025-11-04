#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int rank, size;
    int N = 1000000; 
    double *A = NULL, *B = NULL;
    double local_sum = 0.0, total_sum = 0.0;

    MPI_Init(&argc, &argv);              
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size); 

    int chunk_size = N / size; 

    double *local_A = (double*)malloc(chunk_size * sizeof(double));
    double *local_B = (double*)malloc(chunk_size * sizeof(double));

    if(rank == 0) {
        A = (double*)malloc(N * sizeof(double));
        B = (double*)malloc(N * sizeof(double));
        for(int i = 0; i < N; i++) {
            A[i] = 1.0; 
            B[i] = 1.0;
        }
    }

    double start_time = MPI_Wtime();

    MPI_Scatter(A, chunk_size, MPI_DOUBLE, local_A, chunk_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(B, chunk_size, MPI_DOUBLE, local_B, chunk_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for(int i = 0; i < chunk_size; i++)
        local_sum += local_A[i] * local_B[i];

    MPI_Reduce(&local_sum, &total_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();

    if(rank == 0) {
        printf("Dot Product = %lf\n", total_sum);
        printf("Time taken = %lf seconds with %d processes\n", end_time - start_time, size);
    }

    free(local_A);
    free(local_B);
    if(rank == 0) {
        free(A);
        free(B);
    }

    MPI_Finalize();
    return 0;
}
