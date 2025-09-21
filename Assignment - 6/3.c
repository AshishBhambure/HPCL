#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        if(rank == 0) 
            printf("Need at least 2 processes to demonstrate deadlock.\n");
        MPI_Finalize();
        return 0;
    }

    int msg[100000];

    if(rank == 0) {
        printf("Process 0: Sending message to Process 1\n");
        MPI_Send(&msg, 100000, MPI_INT, 1, 0, MPI_COMM_WORLD); 

        printf("Process 0: Waiting to receive message from Process 1\n");
        MPI_Recv(&msg, 100000, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } 
    else if(rank == 1) {
        printf("Process 1: Sending message to Process 0\n");
        MPI_Send(&msg, 100000, MPI_INT, 0, 0, MPI_COMM_WORLD); 

        printf("Process 1: Waiting to receive message from Process 0\n");
        MPI_Recv(&msg, 100000, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    MPI_Finalize();
    return 0;
}
