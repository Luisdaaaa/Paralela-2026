#include <stdio.h>
#include <unistd.h>

int main(void) {
    printf("Hola Mundo\n");

    long num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    printf("Cores: %ld\n", num_cores);
    return 0;
}
