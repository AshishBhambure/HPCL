#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define BUFFER_SIZE 5
#define NUM_ITEMS   10

int buffer[BUFFER_SIZE];
int count = 0;    // number of items in buffer
int in = 0;       // write index
int out = 0;      // read index

int main() {
    #pragma omp parallel sections
    {
       
        #pragma omp section
        {
            for (int i = 1; i <= NUM_ITEMS; i++) {
                int produced = i; 

                while (count == BUFFER_SIZE) {
                    #pragma omp flush(count)  
                }

                #pragma omp critical
                {
                    buffer[in] = produced;
                    in = (in + 1) % BUFFER_SIZE;
                    count++;
                    printf("Producer produced: %d | Buffer count = %d\n", produced, count);
                }
                #pragma omp flush(count)
            }
        }

   
        #pragma omp section
        {
            for (int i = 1; i <= NUM_ITEMS; i++) {
                int consumed;

                while (count == 0) {
                    #pragma omp flush(count)
                }

                #pragma omp critical
                {
                    consumed = buffer[out];
                    out = (out + 1) % BUFFER_SIZE;
                    count--;
                    printf("Consumer consumed: %d | Buffer count = %d\n", consumed, count);
                }
                #pragma omp flush(count)
            }
        }
    }
    return 0;
}
