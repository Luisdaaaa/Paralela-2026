#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void* mi_funcion(void* argumento){
    long id = (long)argumento; 
    printf("Hola Mundo desde el hilo #%ld\n", id);
    return NULL;
}

int main(void) {
    int rc;
    long num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    pthread_t hilos[num_cores];
    long t; // Declarada aquí para evitar errores de C99 y redefinición

    printf("Cores: %ld\n", num_cores);

    // Primer bucle: Creación
    for(t = 0; t < num_cores; t++) {
        printf("En main: creando el hilo %ld\n", t);
        rc = pthread_create(&hilos[t], NULL, mi_funcion, (void*)t);
        if (rc) {
            printf("Error: %d\n", rc);
        }
    }

    // Segundo bucle: Espera
    for(t = 0; t < num_cores; t++) {
        pthread_join(hilos[t], NULL);
    }

    return 0;
}
