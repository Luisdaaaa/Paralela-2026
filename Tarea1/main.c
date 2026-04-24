#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include "config.h"

void* atender_pasajeros(void* arg);

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
    for (int i = 0; i < 3; i++) {
        pthread_mutex_init(&args.info_compartida->mutex[i], NULL); // Inicializamos los mutex para cada fila
    }

    for(int i = 0; i < args.info_compartida->M_counters_total; i++) {
        args_counters_array[i].mi_indice = i;
        args_counters_array[i].info_compartida = args.info_compartida;
        pthread_create(&counters[i], NULL, atender_pasajeros, &args_counters_array[i]);
    }

    // Esperar a que todos los hilos terminen
    for(int i = 0; i < args.info_compartida->M_counters_total; i++) {
        pthread_join(counters[i], NULL);
    }

    free(args.info_compartida->tiempo_hilos);
    free(args.info_compartida->colas_filas);
        for(int i = 0; i < args.info_compartida->M_counters_total; i++) {
            pthread_cond_destroy(&args.info_compartida->cond[i]);
        }
    free(args.info_compartida->cond);

    
    return 0;
}

void* atender_pasajeros(void* arg) {
    ArgumentosHilo* args = (ArgumentosHilo*)arg;
    int mi_indice = args->mi_indice;
    datos* info = args->info_compartida;
    // Business
    if (mi_indice < info->M_counters_bussines) {
        while (true) {
            pthread_mutex_lock(&info->mutex[1]);
            
            // Preguntar si el hilo debe dormir por orden del hilo control
            while(info->tiempo_hilos[mi_indice] == -1){
                pthread_cond_wait(&info->cond[mi_indice], &info->mutex[1]);
            }

            // Si ya no hay pasajeros, terminamos el hilo
            if (info->colas_filas[1].tamaño <= 0) {
                pthread_mutex_unlock(&info->mutex[1]);
                break; 
            }

            // Atender pasajero
            Pasajero* p = frente(&info->colas_filas[1]);
            if (p) {
                pasajero_print(p); // Imprimir información del pasajero atendido
                pop(&info->colas_filas[1]);
            }
            
            pthread_mutex_unlock(&info->mutex[1]);
            sleep(1); // Simular tiempo de atención al pasajero

        }
    } else if (mi_indice <= info->M_counters_bussines + info->M_counters_economy) { // Si el indice es menor a la suma de M_counters_bussines y M_counters_economy, atiende a la fila de economy
        while (true) {
            pthread_mutex_lock(&info->mutex[0]); // Bloqueamos el mutex de la fila de economy
            
            // Preguntar si el hilo debe dormir por orden del hilo control
            while(info->tiempo_hilos[mi_indice] == -1){
                pthread_cond_wait(&info->cond[mi_indice], &info->mutex[1]);
            }

            // Si ya no hay pasajeros, terminamos el hilo
            if (info->colas_filas[0].tamaño <= 0) {
                pthread_mutex_unlock(&info->mutex[0]);
                break; 
            }

            // Atender pasajero
            Pasajero* p = frente(&info->colas_filas[0]);
            if (p) {
                pasajero_print(p); // Imprimir información del pasajero atendido
                pop(&info->colas_filas[0]);
            }
            
            pthread_mutex_unlock(&info->mutex[0]);
            sleep(1); // Simular tiempo de atención al pasajero
        }
    }else{
        while (true) {
            pthread_mutex_lock(&info->mutex[2]); // Bloqueamos el mutex de la fila de internacionales
            
            // Preguntar si el hilo debe dormir por orden del hilo control
            while(info->tiempo_hilos[mi_indice] == -1){
                pthread_cond_wait(&info->cond[mi_indice], &info->mutex[2]);
            }

            // Si ya no hay pasajeros, terminamos el hilo
            if (info->colas_filas[2].tamaño <= 0) {
                pthread_mutex_unlock(&info->mutex[2]);
                break; 
            }

            // Atender pasajero
            Pasajero* p = frente(&info->colas_filas[2]);
            if (p) {
                pasajero_print(p); // Imprimir información del pasajero atendido
                pop(&info->colas_filas[2]);
            }
            
            pthread_mutex_unlock(&info->mutex[2]);
            sleep(1); // Simular tiempo de atención al pasajero

        }
    }
    return NULL;
}

