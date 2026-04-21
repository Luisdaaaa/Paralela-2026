#include "config.h"

ArgumentosHilo obtener_datos() {
    // Obtener el número de procesadores disponibles
    int num_procesadores;

#ifdef _WIN32
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    num_procesadores = sysinfo.dwNumberOfProcessors;
#else
    num_procesadores = sysconf(_SC_NPROCESSORS_ONLN);
#endif

if (num_procesadores < 5) {
        printf("Error: Este programa requiere al menos 5 procesadores para funcionar (2 especiales + 3 filas minimas).\n");
        printf("Procesadores detectados: %d\n", num_procesadores);
        return (ArgumentosHilo){0}; // Retorna una estructura vacía para indicar el error
    }

    datos* d = (datos*)malloc(sizeof(datos));
    ArgumentosHilo args; 
    int resultado;
    num_procesadores-=2; // Reservamos 2 procesadores para el balanceo y adelantar
    num_procesadores/=3; // Dividimos entre 3 para obtener el número máximo de counters por fila, ya que cada fila tiene la misma cantidad de hilos atendiendo

    printf("Número de procesadores disponibles para cada fila: %d\n", num_procesadores); 
    printf("Ingrese un numero de personas: \n");
    scanf("%d", &resultado);
    d->N_pasajeros = resultado;

    printf("Ingrese un numero de counters por fila: \n");
    scanf("%d", &resultado);
    while(resultado > num_procesadores) { // Como son 3 filas con la misma cantidad de hilos atendiendo, se divide entre 3 el número de procesadores disponibles para obtener el máximo número de counters por fila
        printf("El número de counters por fila no puede ser mayor que el número de procesadores disponibles. Ingrese un número válido: \n");
        scanf("%d", &resultado);
    }
    d->M_counters_por_fila = resultado;


    printf("Ingrese un numero maximo de personas por fila: \n");
    scanf("%d", &resultado);
    while(resultado > d->N_pasajeros) { // No podemos tener más personas por fila que el total de pasajeros
        printf("El número máximo de personas por fila no puede ser mayor que el número total de pasajeros. Ingrese un número válido: \n");
        scanf("%d", &resultado);
    }
    d->Q_maximo_por_fila = resultado;


    printf("Ingrese un numero de personas para dormir la fila: \n");
    scanf("%d", &resultado);
    //Preguntar al profesor como se calcula esto, si es un número fijo o un porcentaje del total de pasajeros
    d->K_pasajeros_para_dormir_filal = resultado;

    printf("Ingrese un numero de tiempo maximo en mili de abordaje para ejecutiva: \n");
    scanf("%d", &resultado);
    d->T_tiempo_abordaje_max_ejecutiva = resultado;

    args.info_compartida = d; // Asignamos la dirección de la estructura de datos al campo info_compartida de ArgumentosHilo
    d->colas_filas = (Cola*)malloc(3 * sizeof(Cola)); // Reservamos memoria para el array de colas
    for (int i = 0; i < 3; i++)   
        {
            cola_init(&d->colas_filas[i]); 

        }
    return args;
}

bool llenar_colas(ArgumentosHilo args) {
    for(int i = 0; i < args.info_compartida->N_pasajeros; i++) {
        Pasajero p;
        int tipoPasajero = rand() % 100; // Genera un número aleatorio entre 0 y 2 para determinar el tipo de pasajero
        if(tipoPasajero<70){
            pasajero_init(&p, i, args.info_compartida->colas_filas[0].tamaño, 0, 0.0); // Asigna el número de pasajero, la fila (i % 3 para distribuirlos en las 3 filas), el tipo de pasajero y el tiempo de abordaje
            push(&args.info_compartida->colas_filas[0], p);
        }
        else if(tipoPasajero>=70&&tipoPasajero<=90){
            pasajero_init(&p, i, args.info_compartida->colas_filas[1].tamaño, 1, 0.0); // Asigna el número de pasajero, la fila (i % 3 para distribuirlos en las 3 filas), el tipo de pasajero y el tiempo de abordaje
            push(&args.info_compartida->colas_filas[1], p);
        }
        else if(tipoPasajero>90){
            pasajero_init(&p, i, args.info_compartida->colas_filas[2].tamaño, 2, 0.0); // Asigna el número de pasajero, la fila (i % 3 para distribuirlos en las 3 filas), el tipo de pasajero y el tiempo de abordaje
            push(&args.info_compartida->colas_filas[2], p);
        }
    }
    return true; 
}