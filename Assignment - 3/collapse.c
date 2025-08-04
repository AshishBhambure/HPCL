#include <stdio.h>
#include <omp.h>

int main() {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("Thread %d processing i = %d, j = %d\n", omp_get_thread_num(), i, j);
        }
    }
    return 0;
}
