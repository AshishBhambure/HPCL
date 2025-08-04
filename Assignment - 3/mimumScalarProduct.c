#include<stdio.h>
#include<omp.h>
#include<stdlib.h>

// Merge Sort 
void merge(int* a, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++) L[i] = a[l + i];
    for (int i = 0; i < n2; i++) R[i] = a[m + 1 + i];

    int i = 0, j = 0, k = l;

    while (i < n1 && j < n2)
        a[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];

    while (i < n1) a[k++] = L[i++];
    while (j < n2) a[k++] = R[j++];

    free(L);
    free(R);
}

void parallel_merge_sort(int* a, int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            parallel_merge_sort(a, l, m);

            #pragma omp section
            parallel_merge_sort(a, m + 1, r);
        }

        merge(a, l, m, r);
    }
}


void main(){
    int mod = 1e9 + 7;
    int n = 1e6;
    int *arr1 = (int *) malloc(n*(sizeof(int)));
    int *arr2 = (int *) malloc(n*(sizeof(int)));
     for(int i = 0;i<n;i++){
        arr1[i] = i;
        arr2[i] = i+2;
     }
//    arr1[0] = 10;
//    arr1[1] = 20;
//    arr1[2] = 30;
//    arr1[3] = 40;

//     arr2[0] = 1;
//     arr2[1] = 2;
//     arr2[2] = 4;
//     arr2[3] = 5;
     #pragma omp parallel sections
     {
     #pragma omp section
     parallel_merge_sort(arr1,0,n-1);
     #pragma omp section
     parallel_merge_sort(arr2,0,n-1);
     }

     // Reversing
     for (int i = 0; i < n / 2; i++) {
        int temp = arr2[i];
        arr2[i] = arr2[n - i - 1];
        arr2[n - i - 1] = temp;
    }

    //  for(int i = 0;i<n;i++){
    //     printf("%d ",arr1[i]);
    //  }
    //  printf("\n\n");

    //  for(int i = 0;i<n;i++){
    //     printf("%d ",arr2[i]);
    //  }
    //  printf("\n");

    
     long ans = 0;
   
     double tin = omp_get_wtime();
     
        #pragma omp parallel for reduction(+:ans)
         for(int i = 0;i<n;i++){
            // printf("Mul  %d,  %d \n",arr1[i],arr2[i]);
            ans = (ans +  (arr1[i]*arr2[i])%mod)%mod;

         }
     double tout = omp_get_wtime();

     printf(" Time Req %0.9f \n",tout-tin);
     

     printf("Minimum Scalar Product :: %ld\n" ,ans);

}