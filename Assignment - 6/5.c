#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>

int main(int argc , char** argv)
{
    int n ;
    n = 4;
    int arr[4] = {1,2,3,4};
    // arr = (cast_type*) malloc(number_of_elements * sizeof(type));

    MPI_Init(&argc,&argv);
        int r;
        MPI_Comm_rank(MPI_COMM_WORLD,&r);

        if(r == 0)
        {
            // Get Res 
            int halfSum;
            MPI_Recv(&halfSum,1,MPI_INT,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            int currSum= 0;
            for(int i = 0;i<n/2;i++)
            {
                currSum += arr[i];
            }
            int total = currSum + halfSum;
            printf("Partial Sum By Rank %d is %d\n" ,r,currSum);
            printf(" The  Toatl Sum = %d \n", total);
        }
        else if(r== 1){
            int sum = 0;
            for(int i = n/2;i<n;i++)
            {
                sum += arr[i];
            }
            printf("Partial Sum By Rank %d is %d\n" ,r,sum);
            MPI_Send(&sum , 1, MPI_INT,0,0,MPI_COMM_WORLD);

        }

    MPI_Finalize();
    return 0;
}