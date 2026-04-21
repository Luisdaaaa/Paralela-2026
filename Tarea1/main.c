#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include "config.h"



int main() {
    ArgumentosHilo args = obtener_datos();

    if (args.info_compartida->N_pasajeros == 0) {
        // Si se retornó una estructura vacía, significa que hubo un error al obtener los datos
        return -1; // Salir del programa con error
    }
    
    return 0;
}