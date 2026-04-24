#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include "config.h"



int main() {
    ArgumentosHilo args = obtener_datos();

    if (args.info_compartida == NULL) return -1;
    pthread_t counters[args.info_compartida->M_counters_total];
    ArgumentosHilo args_counters_array[args.info_compartida->M_counters_total];
    bool colas_llenadas = llenar_colas(args);
    if (!colas_llenadas) {
        fprintf(stderr, "Error al llenar las colas\n");
        return -1;
    }

    for(int i = 0; i < args.info_compartida->M_counters_total; i++) {
        args_counters_array[i].mi_indice = i;
        args_counters_array[i].info_compartida = args.info_compartida;
        pthread_create(&counters[i], NULL, atender_pasajeros, &args_counters_array[i]);
    }
    
    return 0;
}

void* atender_pasajeros(void* arg) {
    ArgumentosHilo* args = (ArgumentosHilo*)arg;
    int mi_indice = args->mi_indice;
    datos* info = args->info_compartida;

    // Aquí iría la lógica para que cada hilo atienda a los pasajeros de su fila correspondiente
    // Esto incluiría el manejo de las colas, el tiempo de abordaje, y las condiciones para dormir o despertar los hilos

    return NULL;
}