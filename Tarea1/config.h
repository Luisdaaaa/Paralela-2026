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
    int K_pasajeros_para_dormir_filal;
    int T_tiempo_abordaje_max_ejecutiva;
    Cola* colas_filas;      // Puntero al array de colas para cada fila
    time_t* tiempo_hilos;   // Puntero al array de tiempos para cada hilo
    pthread_mutex_t mutex[3];   // Mutex para proteger el acceso a los tiempos
    pthread_cond_t* cond;     // Condiciones para cada fila, para despertar a los hilos cuando se cumplan las condiciones de dormirse o despertar
} datos;

typedef struct {
    int mi_indice;          // El número de este hilo (Ej: 0, 1, 2...)
    datos *info_compartida; // Puntero al struct grande con toda la info y colas
} ArgumentosHilo;

ArgumentosHilo obtener_datos();
bool llenar_colas(ArgumentosHilo args);



#endif /* CONFIG_H */

