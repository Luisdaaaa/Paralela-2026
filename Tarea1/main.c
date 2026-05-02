#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include "config.h"
#include <string.h>
#define COLOR_RESET   "\x1b[0m"
#define COLOR_ROJO    "\x1b[31m"
#define COLOR_VERDE   "\x1b[32m"
#define COLOR_AMARILLO "\x1b[33m"

void* atender_pasajeros(void* arg);
void* hiloSupervisor(void* arg);
void* hiloBalanceador(void* arg);

int main() {

    ArgumentosHilo args = obtener_datos();

    if (args.info_compartida == NULL) return -1;
    pthread_t counters[args.info_compartida->M_counters_total];
    pthread_t supervisor;
    pthread_t balanceador;
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
    
    
    args.info_compartida->programa_terminado = false; 
    
    pthread_create(&supervisor, NULL, hiloSupervisor, args.info_compartida);
    pthread_create(&balanceador, NULL, hiloBalanceador, args.info_compartida);

    // Esperar a que todos los hilos terminen
    for(int i = 0; i < args.info_compartida->M_counters_total; i++) {
        pthread_join(counters[i], NULL);
    }
    
    pthread_mutex_lock(&args.info_compartida->mutex_supervisor);
    args.info_compartida->programa_terminado = true; // Indicamos al supervisor que el programa ha terminado
    pthread_cond_signal(&args.info_compartida->cond_supervisor);
    pthread_mutex_unlock(&args.info_compartida->mutex_supervisor);

    pthread_mutex_lock(&args.info_compartida->mutexBalanceador);
    pthread_cond_signal(&args.info_compartida->cond_balanceador);
    pthread_mutex_unlock(&args.info_compartida->mutexBalanceador);
    
    pthread_join(supervisor, NULL); 

    free(args.info_compartida->tiempo_hilos);
    free(args.info_compartida->colas_filas);
    for(int i = 0; i < args.info_compartida->M_counters_total; i++) {
        pthread_cond_destroy(&args.info_compartida->cond[i]);
    }
    free(args.info_compartida->cond);
    pthread_mutex_destroy(&args.info_compartida->mutex_supervisor);
    pthread_cond_destroy(&args.info_compartida->cond_supervisor);

    pthread_mutex_destroy(&args.info_compartida->mutexBalanceador);
    pthread_cond_destroy(&args.info_compartida->cond_balanceador);
    pthread_join(balanceador, NULL);

    return 0;
}

void* atender_pasajeros(void* arg) {
    ArgumentosHilo* args = (ArgumentosHilo*)arg;
    int mi_indice = args->mi_indice;
    datos* info = args->info_compartida;
    struct timespec hiloInicio, HiloFin; // Variables para medir el tiempo del hilo
    struct timespec inicio, fin;// Variables para medir el tiempo de atención a cada pasajero
    clock_gettime(CLOCK_MONOTONIC, &hiloInicio);
    info->tiempo_hilos[mi_indice] = hiloInicio.tv_sec * 1000LL + hiloInicio.tv_nsec / 1000000; // Guardamos en milisegundos
    

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    // Business
    if (mi_indice < info->M_counters_bussines) {
        info->llamado_a_balanceador = false;
        while (true) {

            pthread_mutex_lock(&info->mutex[1]);

            clock_gettime(CLOCK_MONOTONIC, &HiloFin);
            long long tiempo_del_hilo = (HiloFin.tv_sec * 1000LL + HiloFin.tv_nsec / 1000000) - info->tiempo_hilos[mi_indice]; // Calculamos el tiempo en ms
            
            if(tiempo_del_hilo >= info->K_min){

                int valor_para_dormir = rand() % (info->k_max - info->K_min + 1) + info->K_min;

                if(tiempo_del_hilo >= valor_para_dormir||tiempo_del_hilo >= info->k_max){
                    info->hilos_dormidos[mi_indice] = true; // Marcamos el hilo como dormido
                    printf(COLOR_ROJO "El hilo %d se ha dormido\n" COLOR_RESET, mi_indice);
                    fflush(stdout);
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
            struct timespec fin;
            clock_gettime(CLOCK_MONOTONIC, &fin);
            long long tiempo_atencion = (fin.tv_sec - inicio.tv_sec)*1000LL + (fin.tv_nsec - inicio.tv_nsec) / 1000000.0; // Calculamos el tiempo que tardó en atender al pasajero en milisegundos
            if (p) {
                p->tiempoAbordaje = tiempo_atencion; // Asignamos el tiempo de abordaje al pasajero
                strcpy(p->filaPerteneciente, "Ejecutivo");
                pasajero_print(p); // Imprimir información del pasajero atendido
                pop(&info->colas_filas[1]);
            }
            
            if (tiempo_atencion > info->T_tiempo_abordaje_max_ejecutiva&& !info->llamado_a_balanceador) {// Si el tiempo de atención supera el tiempo máximo permitido para ejecutiva, se adelanta a international
                pthread_mutex_lock(&info->mutexBalanceador); // Bloqueamos el mutex de la fila de internacionales
                printf(COLOR_AMARILLO "Se adelanta los pasajeros de ejecutiva a internacionales por tiempo de atención!\n" COLOR_RESET);
                fflush(stdout);
                info->tareaBalanceador = 1; // Indicamos al balanceador que debe balancear por tiempo de atención
                pthread_cond_signal(&info->cond_balanceador); // Despertamos al balanceador 
                pthread_mutex_unlock(&info->mutexBalanceador);
                info->llamado_a_balanceador = true; // Indicamos que ya se hizo el llamado al balanceador para que no se vuelva a llamar por el mismo motivo
            }

            pthread_mutex_unlock(&info->mutex[1]);
        }
    } else if (mi_indice < info->M_counters_bussines + info->M_counters_economy) { // Si el indice es menor a la suma de M_counters_bussines y M_counters_economy, atiende a la fila de economy
        while (true) {
            pthread_mutex_lock(&info->mutex[0]); // Bloqueamos el mutex de la fila de economy
            
            clock_gettime(CLOCK_MONOTONIC, &HiloFin);
            long long tiempo_del_hilo = (HiloFin.tv_sec * 1000LL + HiloFin.tv_nsec / 1000000) - info->tiempo_hilos[mi_indice]; // Calculamos el tiempo en ms
            
            if(tiempo_del_hilo >= info->K_min){

                int valor_para_dormir = rand() % (info->k_max - info->K_min + 1) + info->K_min;

                if(tiempo_del_hilo >= valor_para_dormir||tiempo_del_hilo >= info->k_max){
                    info->hilos_dormidos[mi_indice] = true; // Marcamos el hilo como dormido
                    printf(COLOR_ROJO "El hilo %d se ha dormido\n" COLOR_RESET, mi_indice);
                    fflush(stdout);
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
                struct timespec fin;
                clock_gettime(CLOCK_MONOTONIC, &fin);
                long long tiempo_atencion = (fin.tv_sec - inicio.tv_sec)*1000LL + (fin.tv_nsec - inicio.tv_nsec) / 1000000.0; // Calculamos el tiempo que tardó en atender al pasajero en milisegundos
                p->tiempoAbordaje = tiempo_atencion; // Asignamos el tiempo de abordaje al pasajero
                strcpy(p->filaPerteneciente, "Economy");
                pasajero_print(p); // Imprimir información del pasajero atendido
                pop(&info->colas_filas[0]);
            }
            
            pthread_mutex_unlock(&info->mutex[0]);
        }
    }else{
        while (true) {
            pthread_mutex_lock(&info->mutex[2]); // Bloqueamos el mutex de la fila de internacionales
            
            clock_gettime(CLOCK_MONOTONIC, &HiloFin);
            long long tiempo_del_hilo = (HiloFin.tv_sec * 1000LL + HiloFin.tv_nsec / 1000000) - info->tiempo_hilos[mi_indice]; // Calculamos el tiempo en ms
            
            if(tiempo_del_hilo >= info->K_min){

                int valor_para_dormir = rand() % (info->k_max - info->K_min + 1) + info->K_min;

                if(tiempo_del_hilo >= valor_para_dormir||tiempo_del_hilo >= info->k_max){
                    info->hilos_dormidos[mi_indice] = true; // Marcamos el hilo como dormido
                    printf(COLOR_ROJO "El hilo %d se ha dormido\n" COLOR_RESET, mi_indice);
                    fflush(stdout);
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
                struct timespec fin;
                clock_gettime(CLOCK_MONOTONIC, &fin);
                long long tiempo_atencion = (fin.tv_sec - inicio.tv_sec)*1000LL + (fin.tv_nsec - inicio.tv_nsec) / 1000000.0; // Calculamos el tiempo que tardó en atender al pasajero en milisegundos
                p->tiempoAbordaje = tiempo_atencion; // Asignamos el tiempo de abordaje al pasajero
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
    datos* info = (datos*)arg;
    while(info->programa_terminado == false) {
        int tiempo_para_dormir = rand() % 500 + 1500; // el supervisor duerme entre 500ms y 2000ms antes de revisar los hilos nuevamente
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);        
        long long total_nsec = ts.tv_nsec + (tiempo_para_dormir * 1000000LL); 
        ts.tv_sec += total_nsec / 1000000000LL;
        ts.tv_nsec = total_nsec % 1000000000LL;
        
        pthread_mutex_lock(&info->mutex_supervisor);
        pthread_cond_timedwait(&info->cond_supervisor, &info->mutex_supervisor, &ts);
        pthread_mutex_unlock(&info->mutex_supervisor);
        
        for (int i = 0; i < info->M_counters_total; i++) {
            int mutex_fila = (i < info->M_counters_bussines) ? 1 : (i < info->M_counters_bussines + info->M_counters_economy) ? 0 : 2;
            
            pthread_mutex_lock(&info->mutex[mutex_fila]); // Bloqueamos el mutex de la fila correspondiente al hilo
            
            if (info->hilos_dormidos[i]) { 
                info->hilos_dormidos[i] = false; 
                struct timespec ahora;
                clock_gettime(CLOCK_MONOTONIC, &ahora);
                info->tiempo_hilos[i] = ahora.tv_sec * 1000LL + ahora.tv_nsec / 1000000; 
                
                pthread_cond_signal(&info->cond[i]); 
                printf(COLOR_VERDE "El supervisor ha despertado al hilo %d\n" COLOR_RESET, i);
            }
            
            pthread_mutex_unlock(&info->mutex[mutex_fila]); // Desbloqueamos el mutex de la fila correspondiente al hilo
        }
    }
    return NULL;
}

void* hiloBalanceador(void* arg) {
    datos* info = (datos*)arg;
    while(info->programa_terminado == false) {
        pthread_mutex_lock(&info->mutexBalanceador);
        pthread_cond_wait(&info->cond_balanceador, &info->mutexBalanceador); // El balanceador espera a que lo llammen para hacer una de sus 2 tareas
        pthread_mutex_unlock(&info->mutexBalanceador);
        if(info->tareaBalanceador == 0){
            datos* info = (datos*)arg;
            
        }
        else if(info->tareaBalanceador == 1){//adelantar pasajeros de ejecutiva a internacionales por tiempo de atención
            pthread_mutex_lock(&info->mutex[1]); // Bloqueamos el mutex de la fila de ejecutiva
            pthread_mutex_lock(&info->mutex[2]); // Bloqueamos el mutex de la fila de internacionales
            
            Cola* colaTemporal = (Cola*)malloc(sizeof(Cola)); 
            cola_init(colaTemporal); // OBLIGATORIO: Inicializar la cola para que cabeza y tamaño sean válidos

            while(info->colas_filas[2].tamaño > 0) {
                Pasajero* pInternational = frente(&info->colas_filas[2]);
                if (pInternational) {
                    push(colaTemporal, *pInternational);
                    pop(&info->colas_filas[2]);
                }
            }
            while(info->colas_filas[1].tamaño > 0) {
                Pasajero* pEjecutivo = frente(&info->colas_filas[1]);
                if (pEjecutivo) {
                    push(&info->colas_filas[2], *pEjecutivo); // Adelantamos al pasajero de ejecutiva a internacionales
                    pop(&info->colas_filas[1]);
                }
            }
            while(colaTemporal->tamaño > 0) { // Volvemos a poner a los pasajeros internacionales que estaban en la cola temporal de vuelta a la cola de internacionales
                Pasajero* pInternational = frente(colaTemporal);
                if (pInternational) {
                    push(&info->colas_filas[2], *pInternational);
                    pop(colaTemporal);
                }
            }
            
            free(colaTemporal); 
            info->tareaBalanceador = -1; 

            pthread_mutex_unlock(&info->mutex[2]); 
            pthread_mutex_unlock(&info->mutex[1]); 
        }  
    return NULL; 
    }
}



