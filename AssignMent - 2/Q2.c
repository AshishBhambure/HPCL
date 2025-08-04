#include<stdio.h>
#include<omp.h>
#include<float.h>

void main(){
    
  
    long long n = 1e2;

    // #pragma omp parallel for reduction(+:pi)
    // for(long long i = 0;i<n;i++){
    //        double term = (i % 2 == 0 ? 1.0 : -1.0) / (2.0 * i + 1);
    //        pi+= term;
    //  }
     
   double minTime = DBL_MAX;
   int threadsForMinTime = 0;
    for(int t = 1;t<=16;t++){
      double pi  = 0.0;
    omp_set_num_threads(t);
    double tin = omp_get_wtime();
    
     #pragma omp parallel
     {
        double currThreadRes = 0.0;
        #pragma omp for
        for(long long i = 0;i<n;i++){
           double term = (i % 2 == 0 ? 1.0 : -1.0) / (2.0 * i + 1);
           currThreadRes += term;
        }
        #pragma omp atomic
        pi += currThreadRes;
     }

     double tout = omp_get_wtime();


    pi*=4;
    
    printf("PII:: %0.20f \n",pi);

    printf("Time %0.8f | Threads %d \n",tout-tin,t);
    if(tout-tin < minTime){
        minTime = tout-tin;
        threadsForMinTime = t;
    }

    }

    printf("Executed \n  Min Time got For Threads = %d" , threadsForMinTime);
   
}