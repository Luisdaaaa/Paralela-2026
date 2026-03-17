#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Hola Mundo\n");

    long num_cores= sysconf(_SC_NPROCESSORS_ONLN);
    printf("Cores: ", num_cores);
    return 0;

    
}
