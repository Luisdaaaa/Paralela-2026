#include <stdio.h>
#include <omp.h>

int main() {
    #pragma omp parallel
    {
        #pragma omp single
        {
            printf("A ");

            #pragma omp task
            {
                printf("race ");
            }

            #pragma omp task
            {
                printf("car ");
            }

            #pragma omp taskwait
            printf("\n");
        }
    }
    return 0;
}