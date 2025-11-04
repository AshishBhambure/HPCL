#include<stdio.h>
#include<mpi.h>

#define N 3  
void init_matrix(int mat[N][N]) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            mat[i][j] = i * N + j + 1; 
}

void init_vector(int vec[N]) {
    for (int i = 0; i < N; i++)
        vec[i] = i + 1; 
}


int main(int argc , char** argv){
    int n = N;
     static int A[N][N]; 
     
    // int arr1[3][3] = {{1,2,3},{4,5,6},{7,8,9}}; //matrix 
    int v[N];  
    int res[N];
    int local_row[N];
    int local_res;
    int rank;
    int size;
    //  Now We Are doing Coarse Grained Matrix Vector Multiplication
    // Every Roe Multiplied By the Entore Vector ==> Divide Them 
    // Paralleize this 
    //  Need to Distribute This Work 

 MPI_Init(&argc , &argv);

 if(rank == 0) {
        init_matrix(A);
        init_vector(v);
    }
 MPI_Comm_rank(MPI_COMM_WORLD ,&rank);
 MPI_Comm_size(MPI_COMM_WORLD,&size);
 double start_time = MPI_Wtime();
 MPI_Bcast(v,n,MPI_INT,0,MPI_COMM_WORLD);
 MPI_Scatter(A,n,MPI_INT,local_row,n,MPI_INT,0,MPI_COMM_WORLD);

 //calculate local res 
    local_res = 0;
 for(int i = 0;i<n;i++)
 {
    local_res += local_row[i]*v[i];
 }
 
 MPI_Gather(&local_res , 1,MPI_INT,res,1,MPI_INT,0,MPI_COMM_WORLD);
  double end_time = MPI_Wtime(); // end timer
 if(rank == 0) {
        printf("Final Result Vector: ");
        for(int i=0; i<N; i++) {
            printf("%d ", res[i]);
        }
         printf("\nElapsed Time = %f seconds\n", end_time - start_time);
        printf("\n");
    }

 MPI_Finalize();

}