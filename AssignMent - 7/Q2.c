#include <stdio.h>
#include <mpi.h>

#define N 500   // Matrix size (NxN)

// Initialize a matrix with sample values
void init_matrix(int mat[N][N]) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            mat[i][j] = i * N + j + 1;
}

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int A[N][N], B[N][N], C[N][N];
    int local_A[N][N], local_C[N][N];

    if (rank == 0) {
        init_matrix(A);
        init_matrix(B);
        // printf("Matrix A:\n");
        // for (int i = 0; i < N; i++) {
        //     for (int j = 0; j < N; j++) printf("%d ", A[i][j]);
        //     printf("\n");
        // }
        // printf("\nMatrix B:\n");
        // for (int i = 0; i < N; i++) {
        //     for (int j = 0; j < N; j++) printf("%d ", B[i][j]);
        //     printf("\n");
        // }
    }

    double start = MPI_Wtime();

    MPI_Bcast(B, N*N, MPI_INT, 0, MPI_COMM_WORLD);

    int rows_per_proc = N / size; 
    MPI_Scatter(A, rows_per_proc * N, MPI_INT,
                local_A, rows_per_proc * N, MPI_INT,
                0, MPI_COMM_WORLD);

    for (int i = 0; i < rows_per_proc; i++) {
        for (int j = 0; j < N; j++) {
            local_C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                local_C[i][j] += local_A[i][k] * B[k][j];
            }
        }
    }

    MPI_Gather(local_C, rows_per_proc * N, MPI_INT,
               C, rows_per_proc * N, MPI_INT,
               0, MPI_COMM_WORLD);

    double end = MPI_Wtime();

    if (rank == 0) {
        // printf("\nFinal Result Matrix C = A x B:\n");
        // for (int i = 0; i < N; i++) {
        //     for (int j = 0; j < N; j++)
        //         printf("%d ", C[i][j]);
        //     printf("\n");
        // }
        printf("\nTime taken: %f seconds\n", end - start);
    }

    MPI_Finalize();
    return 0;
}
