// Librerías para obtener el número de procesadores dependiendo del sistema operativo
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cola.h"
#ifndef CONFIG_H
#define CONFIG_H



typedef struct {
    int N_pasajeros;
    int M_counters_bussines;
    int M_counters_economy;
    int M_counters_internacionales;
    int M_counters_total;
    int Q_maximo_por_fila;
    long long K_min;
    long long k_max;
    long long T_tiempo_abordaje_max_ejecutiva;
    long long* tiempo_hilos;   // Puntero al array de tiempos para cada hilo
    Cola* colas_filas;      // Puntero al array de colas para cada fila
    pthread_mutex_t mutex[3];   // Mutex para proteger el acceso a los tiempos
    pthread_mutex_t mutex_supervisor; // Mutex para proteger el acceso a la condición del supervisor
    pthread_mutex_t mutexBalanceador; // Mutex para proteger el acceso a la condición del balanceador
    pthread_cond_t* cond;     // Condiciones para cada fila, para despertar a los hilos cuando se cumplan las condiciones de dormirse o despertar
    pthread_cond_t cond_supervisor;
    pthread_cond_t cond_balanceador;
    bool* hilos_dormidos; // Array de booleanos para saber si un hilo está dormido o no
    bool programa_terminado; // Variable para indicar al supervisor que el programa ha terminado y ya no es necesario que despierte a los hilos
    int tareaBalanceador; //tipo de tarea que realiza el balanceador, 0 para balancear por cantidad de pasajeros en cada fila, 1 para balancear por tiempo de atención de cada fila
    bool llamado_a_balanceador; // Variable para saber si el hilo ha llamado al balanceador por tiempo de atención
} datos;

typedef struct {
    int mi_indice;          // El número de este hilo (Ej: 0, 1, 2...)
    datos *info_compartida; // Puntero al struct grande con toda la info y colas
} ArgumentosHilo;

ArgumentosHilo obtener_datos();
bool llenar_colas(ArgumentosHilo args);



#endif /* CONFIG_H */

