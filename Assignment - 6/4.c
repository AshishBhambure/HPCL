#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int send_data = 101;
    int recv_data;

    int right = (rank + 1) % size;             
    int left  = (rank - 1 + size) % size;       

    if(rank % 2 == 0) {
        MPI_Send(&send_data, 1, MPI_INT, right, 0, MPI_COMM_WORLD);
        MPI_Recv(&recv_data, 1, MPI_INT, left, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
        MPI_Recv(&recv_data, 1, MPI_INT, left, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(&send_data, 1, MPI_INT, right, 0, MPI_COMM_WORLD);
    }

    printf("Process %d sent %d to %d and received %d from %d\n", 
            rank, send_data, right, recv_data, left);

    MPI_Finalize();
    return 0;
}
