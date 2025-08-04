#include <stdio.h>
#include <omp.h>

int main(void)
{
    omp_set_num_threads(4);
    #pragma omp parallel
    {
     printf("Hello, world.\n");
    printf("Ashish Bro !! \n");
    for(int i = 0;i<4;i++) printf("%d ",i);
    printf(" \n");
     }
    return 0;
}
