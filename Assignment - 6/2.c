#include<stdio.h>
#include<mpi.h>

int main(int argc, char** argv)
{
    MPI_Init(&argc,&argv);
    int ws,rank;
    MPI_Comm_size(MPI_COMM_WORLD,&ws);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    printf(" Rank - %d , Total Processes/world Size -%d \n",rank,ws);

    MPI_Finalize();
}