#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void* mi_funcion(void* argumento){
    printf("Hola Mundo\n");
    return NULL;
}
int main(void) {
    int rc;
    int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    printf("Cores: %ld\n", num_cores);
    pthread_t hilos [num_cores];

    for(int t = 0; t < num_cores; t++) {
        printf("En main: creando el hilo %ld\n", t);
        rc = pthread_create(&hilos[t], NULL, mi_funcion, (void*)t);
    }
    for(int t = 0; t < num_cores; t++) {
        pthread_join(hilos[t], NULL);
    }
    return 0;
}
