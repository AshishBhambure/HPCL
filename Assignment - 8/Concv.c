#include <stdio.h>
#include <mpi.h>

#define N 100  // Image size N x N
#define K 3  // Kernel size K x K

int main(int argc, char* argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv);                
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size); 

    int image[N][N];
    int kernel[K][K];

    if (rank == 0) {
        int count = 1;
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                image[i][j] = count++;

        int temp[K][K] = { {1,0,-1}, {1,0,-1}, {1,0,-1} };
        for (int i = 0; i < K; i++)
            for (int j = 0; j < K; j++)
                kernel[i][j] = temp[i][j];
    }

    MPI_Bcast(&image, N*N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&kernel, K*K, MPI_INT, 0, MPI_COMM_WORLD);

    int out_rows = N - K + 1;
    int out_cols = N - K + 1;

    int rows_per_proc = out_rows / size;
    int extra = out_rows % size;

    int start_row = rank * rows_per_proc + (rank < extra ? rank : extra);
    int local_rows = rows_per_proc + (rank < extra ? 1 : 0);

    int local_output[local_rows][out_cols];

    MPI_Barrier(MPI_COMM_WORLD); 
    double t_start = MPI_Wtime();

    for (int i = 0; i < local_rows; i++) {
        int row = start_row + i; 
        for (int j = 0; j < out_cols; j++) {
            int sum = 0;
            for (int ki = 0; ki < K; ki++)
                for (int kj = 0; kj < K; kj++)
                    sum += image[row + ki][j + kj] * kernel[ki][kj];
            local_output[i][j] = sum;
        }
    }

    if (rank == 0) {
        int output[out_rows][out_cols];

        for (int i = 0; i < local_rows; i++)
            for (int j = 0; j < out_cols; j++)
                output[i][j] = local_output[i][j];

        int offset = local_rows;
        for (int p = 1; p < size; p++) {
            int p_rows = out_rows / size + (p < extra ? 1 : 0);
            MPI_Recv(&output[offset][0], p_rows*out_cols, MPI_INT, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            offset += p_rows;
        }

        double t_end = MPI_Wtime();
        printf("Number of processes (np) = %d\n", size);
        printf("Time taken for convolution = %lf seconds\n", t_end - t_start);

        printf("Convolution Output:\n");
        // for (int i = 0; i < out_rows; i++) {
        //     for (int j = 0; j < out_cols; j++)
        //         printf("%d ", output[i][j]);
        //     printf("\n");
        // }
    } else {
        MPI_Send(&local_output, local_rows*out_cols, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
