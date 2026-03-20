#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int num1=0;
int num2=0;
void* mi_funcion(void* argumento){
    for (int i=0; i<1000;i++){
      num1++;
    }
    return NULL;
}
int main(void) {
    int rc;
    long num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    pthread_t hilos[num_cores];
    long t; 

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
      printf("Valor de num1: %d\n", num1);
    }

    

    

