#include <stdio.h>
#include <omp.h>

int main() {
    #pragma omp parallel for ordered
    for (int i = 0; i < 5; i++) {
        printf("Thread %d is processing iteration %d\n", omp_get_thread_num(), i);

        
        #pragma omp ordered
        printf("Ordered output: i = %d \n", i);
    }
    return 0;
}
