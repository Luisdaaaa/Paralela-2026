// Librerías para obtener el número de procesadores dependiendo del sistema operativo
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <stdio.h>
#include "cola.h"
#ifndef CONFIG_H
#define CONFIG_H



typedef struct {
    int N_pasajeros;
    int M_filas_bussines;
    int M_filas_economy;
    int M_filas_internacionales;
    int M_filas_total;
    int Q_maximo_por_fila;
    int K_pasajeros_para_dormir_filal;
    int T_tiempo_abordaje_max_ejecutiva;
    Cola* colas_filas;      // Puntero al array de colas para cada fila

} datos;

typedef struct {
    int mi_indice;          // El número de este hilo (Ej: 0, 1, 2...)
    datos *info_compartida; // Puntero al struct grande con toda la info y colas
} ArgumentosHilo;

ArgumentosHilo obtener_datos(ArgumentosHilo* args);

#endif /* CONFIG_H */

