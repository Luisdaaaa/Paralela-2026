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
    info->tiempo_hilos[mi_indice] = time(NULL); // empezamos a contar el tiempo desde que el hilo inicia su trabajo, para decidir si el hilo se duerme o no
    // Business
    if (mi_indice < info->M_counters_bussines) {
        while (true) {
            time_t tiempo_abordaje = time(NULL); //calcula el tiempo de abordaje para el pasajero actual, para decidir si se adelanta a los business o no

            pthread_mutex_lock(&info->mutex[1]);

            time_t tiempo_del_hilo = time(NULL) - info->tiempo_hilos[mi_indice]; // Calculamos el tiempo que ha pasado desde que el hilo empezó a atender pasajeros
            
            if(tiempo_del_hilo >= info->K_min){

                int valor_para_dormir = rand() % (info->k_max - info->K_min + 1) + info->K_min;

                if(tiempo_del_hilo <= valor_para_dormir||tiempo_del_hilo >= info->k_max){
                    info->hilos_dormidos[mi_indice] = true; // Marcamos el hilo como dormido
                }

                while(info->hilos_dormidos[mi_indice]){ // si el tiempo del hilo es menor o igual al numero aleatorio en el rango, el hilo se duerme
                pthread_cond_wait(&info->cond[mi_indice], &info->mutex[1]);
                }
            }
            
            // Si ya no hay pasajeros, terminamos el hilo
            if (info->colas_filas[1].tamaño <= 0) {
                pthread_mutex_unlock(&info->mutex[1]);
                break; 
            }

            // Atender pasajero
            Pasajero* p = frente(&info->colas_filas[1]);
            time_t tiempo_atencion = time(NULL) - tiempo_abordaje; // Calculamos el tiempo que tardó en atender al pasajero
            if (p) {
                p->tiempoAbordaje = tiempo_atencion; // Asignamos el tiempo de abordaje al pasajero
                strcpy(p->filaPerteneciente, "Ejecutivo");
                pasajero_print(p); // Imprimir información del pasajero atendido
                pop(&info->colas_filas[1]);
            }
            
            if (tiempo_atencion > info->T_tiempo_abordaje_max_ejecutiva) {// Si el tiempo de atención supera el tiempo máximo permitido para ejecutiva, se adelanta a international
                pthread_mutex_lock(&info->mutex[2]); // Bloqueamos el mutex de la fila de internacionales
                printf("Se adelanta los pasajeros de ejecutiva a internacionales por tiempo de atención!\n");
                pthread_mutex_unlock(&info->mutex[2]);
            }

            pthread_mutex_unlock(&info->mutex[1]);
        }
    } else if (mi_indice < info->M_counters_bussines + info->M_counters_economy) { // Si el indice es menor a la suma de M_counters_bussines y M_counters_economy, atiende a la fila de economy
        while (true) {
            pthread_mutex_lock(&info->mutex[0]); // Bloqueamos el mutex de la fila de economy
            
            time_t tiempo_del_hilo = time(NULL) - info->tiempo_hilos[mi_indice]; // Calculamos el tiempo que ha pasado desde que el hilo empezó a atender pasajeros
            
            if(tiempo_del_hilo >= info->K_min){

                int valor_para_dormir = rand() % (info->k_max - info->K_min + 1) + info->K_min;

                if(tiempo_del_hilo <= valor_para_dormir||tiempo_del_hilo >= info->k_max){
                    info->hilos_dormidos[mi_indice] = true; // Marcamos el hilo como dormido
                }

                while(info->hilos_dormidos[mi_indice]){ // si el tiempo del hilo es menor o igual al numero aleatorio en el rango, el hilo se duerme
                pthread_cond_wait(&info->cond[mi_indice], &info->mutex[0]);
                }
            }

            // Si ya no hay pasajeros, terminamos el hilo
            if (info->colas_filas[0].tamaño <= 0) {
                pthread_mutex_unlock(&info->mutex[0]);
                break; 
            }

            // Atender pasajero
            Pasajero* p = frente(&info->colas_filas[0]);
            if (p) {
                strcpy(p->filaPerteneciente, "Economy");
                pasajero_print(p); // Imprimir información del pasajero atendido
                pop(&info->colas_filas[0]);
            }
            
            pthread_mutex_unlock(&info->mutex[0]);
        }
    }else{
        while (true) {
            pthread_mutex_lock(&info->mutex[2]); // Bloqueamos el mutex de la fila de internacionales
            
            time_t tiempo_del_hilo = time(NULL) - info->tiempo_hilos[mi_indice]; // Calculamos el tiempo que ha pasado desde que el hilo empezó a atender pasajeros
            
            if(tiempo_del_hilo >= info->K_min){

                int valor_para_dormir = rand() % (info->k_max - info->K_min + 1) + info->K_min;

                if(tiempo_del_hilo <= valor_para_dormir||tiempo_del_hilo >= info->k_max){
                    info->hilos_dormidos[mi_indice] = true; // Marcamos el hilo como dormido
                }

                while(info->hilos_dormidos[mi_indice]){ // si el tiempo del hilo es menor o igual al numero aleatorio en el rango, el hilo se duerme
                pthread_cond_wait(&info->cond[mi_indice], &info->mutex[2]);
                }
            }

            // Si ya no hay pasajeros, terminamos el hilo
            if (info->colas_filas[2].tamaño <= 0) {
                pthread_mutex_unlock(&info->mutex[2]);
                break; 
            }

            // Atender pasajero
            Pasajero* p = frente(&info->colas_filas[2]);
            if (p) {
                strcpy(p->filaPerteneciente, "Internacional");
                pasajero_print(p); // Imprimir información del pasajero atendido
                pop(&info->colas_filas[2]);
            }
            
            pthread_mutex_unlock(&info->mutex[2]);
        }
    }
    return NULL;
}

void* hiloSupervisor(void* arg) {

    while(1) {

    }
    return NULL;
}

