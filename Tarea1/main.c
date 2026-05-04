#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include "config.h"
#include <string.h>
#define COLOR_RESET   "\x1b[0m"
#define COLOR_ROJO    "\x1b[31m"
#define COLOR_VERDE   "\x1b[32m"
#define COLOR_AMARILLO "\x1b[33m"
#define COLOR_AZUL    "\x1b[34m"

void* atender_pasajeros(void* arg);
void* hiloSupervisor(void* arg);
void* hiloBalanceador(void* arg);

int main() {
    
    ArgumentosHilo args = obtener_datos();
    struct timespec inicioPrograma, finPrograma;
    clock_gettime(CLOCK_MONOTONIC, &inicioPrograma); // Medimos el tiempo de inicio del programa

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
    args.info_compartida->superadoTiempoMaximoEjecutiva = false;
    
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

    clock_gettime(CLOCK_MONOTONIC, &finPrograma);
    long long tiempo_total = (finPrograma.tv_sec * 1000LL + finPrograma.tv_nsec / 1000000) - (inicioPrograma.tv_sec * 1000LL + inicioPrograma.tv_nsec / 1000000);
    printf("Tiempo total del programa: %lld ms\n", tiempo_total);

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
        while (true) {
            bool hubo_pasajero= false; // Variable para saber si hubo un pasajero para atender o no

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
            Pasajero copia_pasajero;
            struct timespec fin;
            clock_gettime(CLOCK_MONOTONIC, &fin);
            long long tiempo_atencion = (fin.tv_sec - inicio.tv_sec)*1000LL + (fin.tv_nsec - inicio.tv_nsec) / 1000000.0; // Calculamos el tiempo que tardó en atender al pasajero en milisegundos
            if (p) {
                hubo_pasajero = true;
                p->tiempoAbordaje = tiempo_atencion; // Asignamos el tiempo de abordaje al pasajero
                strcpy(p->filaPerteneciente, "Ejecutivo");
                copia_pasajero = *p; // Hacemos una copia del pasajero para imprimir su información después de desbloquear el mutex
                pop(&info->colas_filas[1]);
            }
            
            if (tiempo_atencion > info->T_tiempo_abordaje_max_ejecutiva&&info->colas_filas[1].tamaño > 0) {// Si el tiempo de atención supera el tiempo máximo permitido para ejecutiva, se adelanta a international
                info->superadoTiempoMaximoEjecutiva = true; // Indicamos que se ha superado el tiempo máximo de abordaje para ejecutiva
            }

            pthread_mutex_unlock(&info->mutex[1]);
            if(hubo_pasajero){
                pasajero_print(&copia_pasajero); // Imprimir información del pasajero atendido
                fflush(stdout);
            }
        }
    } else if (mi_indice < info->M_counters_bussines + info->M_counters_economy) { // Si el indice es menor a la suma de M_counters_bussines y M_counters_economy, atiende a la fila de economy
        while (true) {
            bool hubo_pasajero= false; // Variable para saber si hubo un pasajero para atender o no
            pthread_mutex_lock(&info->mutex[0]); // Bloqueamos el mutex de la fila de economy
            
            clock_gettime(CLOCK_MONOTONIC, &HiloFin);
            long long tiempo_del_hilo = (HiloFin.tv_sec * 1000LL + HiloFin.tv_nsec / 1000000) - info->tiempo_hilos[mi_indice]; // Calculamos el tiempo en ms
            
            if(tiempo_del_hilo >= info->K_min){

                int valor_para_dormir = rand() % (info->k_max - info->K_min + 1) + info->K_min;

                if(tiempo_del_hilo >= valor_para_dormir||tiempo_del_hilo >= info->k_max){
                    info->hilos_dormidos[mi_indice] = true; // Marcamos el hilo como dormido
                    printf(COLOR_ROJO "El hilo %d se ha dormido\n" COLOR_RESET, mi_indice);
                    fflush(stdout);
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
            Pasajero copia_pasajero;
            if (p) {
                hubo_pasajero = true;
                struct timespec fin;
                clock_gettime(CLOCK_MONOTONIC, &fin);
                long long tiempo_atencion = (fin.tv_sec - inicio.tv_sec)*1000LL + (fin.tv_nsec - inicio.tv_nsec) / 1000000.0; // Calculamos el tiempo que tardó en atender al pasajero en milisegundos
                p->tiempoAbordaje = tiempo_atencion; // Asignamos el tiempo de abordaje al pasajero
                strcpy(p->filaPerteneciente, "Economy");
                copia_pasajero = *p; // Hacemos una copia del pasajero para imprimir su información después de desbloquear el mutex
                pop(&info->colas_filas[0]);
            }
            
            pthread_mutex_unlock(&info->mutex[0]);

            if(hubo_pasajero){
                pasajero_print(&copia_pasajero); // Imprimir información del pasajero atendido
                fflush(stdout);
            }
        }
    }else{
        while (true) {
            bool hubo_pasajero= false; // Variable para saber si hubo un pasajero 
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
            Pasajero copia_pasajero;
            if (p) {
                hubo_pasajero = true;
                struct timespec fin;
                clock_gettime(CLOCK_MONOTONIC, &fin);
                long long tiempo_atencion = (fin.tv_sec - inicio.tv_sec)*1000LL + (fin.tv_nsec - inicio.tv_nsec) / 1000000.0; // Calculamos el tiempo que tardó en atender al pasajero en milisegundos
                p->tiempoAbordaje = tiempo_atencion; // Asignamos el tiempo de abordaje al pasajero
                strcpy(p->filaPerteneciente, "Internacional");
                copia_pasajero = *p; // Hacemos una copia del pasajero para imprimir su información después de desbloquear el mutex
                pop(&info->colas_filas[2]);  
            }
            
            pthread_mutex_unlock(&info->mutex[2]);
            if(hubo_pasajero){
                pasajero_print(&copia_pasajero); // Imprimir información del pasajero atendido
                fflush(stdout);
            }
        }
    }
    return NULL;
}

void* hiloSupervisor(void* arg) {
    datos* info = (datos*)arg;
    while(info->programa_terminado == false) {
        int tiempo_para_dormir = rand() % (info->max_supervisor - info->min_supervisor + 1) + info->min_supervisor; // el supervisor duerme entre el tiempo elegido
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
                fflush(stdout);
            }
            pthread_mutex_unlock(&info->mutex[mutex_fila]); // Desbloqueamos el mutex de la fila correspondiente al hilo
        }
    }
    return NULL;
}

void* hiloBalanceador(void* arg) {
    datos* info = (datos*)arg;
    while(info->programa_terminado == false) {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_nsec += 160000000;

        if (ts.tv_nsec >= 1000000000) {
            ts.tv_sec += 1;
            ts.tv_nsec -= 1000000000;
        }
        pthread_mutex_lock(&info->mutexBalanceador);
        pthread_cond_timedwait(&info->cond_balanceador, &info->mutexBalanceador, &ts); // El balanceador espera un tiempo para hacer sus 2 tareas
        pthread_mutex_unlock(&info->mutexBalanceador);

        //Logica de adelantar pasajeros por tiempo de atención
        if(info->superadoTiempoMaximoEjecutiva){// Si se ha superado el tiempo máximo de abordaje para ejecutiva, se adelanta a international 30 personas de esa fila
            printf(COLOR_AMARILLO "Se adelanta los pasajeros de ejecutiva a internacionales por tiempo de atención!\n" COLOR_RESET);
            pthread_mutex_lock(&info->mutex[1]); // Bloqueamos el mutex de la fila de ejecutiva
            pthread_mutex_lock(&info->mutex[2]); // Bloqueamos el mutex de la fila de internacionales
            
            adelantar_pasajero(&info->colas_filas[1], &info->colas_filas[2]); // Adelantamos 30 pasajeros de ejecutiva a internacionales
            fflush(stdout);
            if(info->colas_filas[1].tamaño == 0) {
                printf(COLOR_ROJO "La fila de ejecutiva se ha quedado sin pasajeros!\n" COLOR_RESET);
                fflush(stdout);
                info->superadoTiempoMaximoEjecutiva = false; // Si la fila de ejecutiva se queda sin pasajeros, ya no es necesario adelantar por tiempo de atención
            }
            
            pthread_mutex_unlock(&info->mutex[2]); 
            pthread_mutex_unlock(&info->mutex[1]); 
        }

        //logica de balancear por cantidad de pasajeros en cada fila
        for(int i = 0; i < 2; i++) {
            int excesoFila= info->colas_filas[i].tamaño - info->Q_maximo_por_fila;
            int excesoInternacional= info->colas_filas[2].tamaño - info->Q_maximo_por_fila;
            if(info->colas_filas[i].tamaño > info->Q_maximo_por_fila && excesoFila >= excesoInternacional) { // Si alguna fila tiene más pasajeros que el máximo permitido por fila y el exceso de esa fila es mayor al exceso de la fila internacional, se balancead
                int ideal_a_mover = (excesoFila - excesoInternacional) / 2;
    
                // Se usa un limite para no congelar los counters
                int cantidad_real = (ideal_a_mover > 100) ? 100 : ideal_a_mover;

                if (cantidad_real > 0) {
                pthread_mutex_lock(&info->mutex[i]);
                pthread_mutex_lock(&info->mutex[2]);
        
                balancear(&info->colas_filas[i], &info->colas_filas[2], cantidad_real);

                pthread_mutex_unlock(&info->mutex[2]);
                pthread_mutex_unlock(&info->mutex[i]);
                printf(COLOR_AZUL "Se balancean los pasajeros de la fila %d a internacionales por cantidad de pasajeros!\n" COLOR_RESET, i);
                fflush(stdout);
                }
            }
        }
    }
    return NULL;
}
