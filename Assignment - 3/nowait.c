#include <stdio.h>
#include <omp.h>

int main() {
    #pragma omp parallel
    {
        #pragma omp for nowait
        for (int i = 0; i < 5; i++) {
            printf("Thread %d finished loop iteration %d\n", omp_get_thread_num(), i);
        }

        
        printf("Thread %d reached after loop (with barrier)\n", omp_get_thread_num());
    }
    return 0;
}
