#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <time.h>
int fib(int n) {
    if (n <= 1) return 1;

    int x, y;

    #pragma omp task  shared(x)
    x = fib(n - 1);

    #pragma omp task  shared(y)
    y = fib(n - 2);

    #pragma omp taskwait
    return x + y;
}
void main()
{
   int n = 3, ans;

    #pragma omp parallel
    {
        #pragma omp single
        ans = fib(n);
    }
   printf("%d\n",ans);

}