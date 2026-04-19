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

    printf("Número de procesadores disponibles: %d\n", num_procesadores); 
    printf("Ingrese un numero de personas: \n");
    scanf("%d", &resultado);
    d->N_pasajeros = resultado;


    printf("Ingrese un numero de filas para bussines: \n");
    scanf("%d", &resultado);
    while (resultado > num_procesadores-2|| resultado < 1) { // Reservamos 2 procesadores para las otras filas, 1 para economy, 1 para internacionales
        printf("El número de filas para bussines no puede ser mayor que el número de procesadores disponibles, abarcar todas las filas o ser menor que 1. Ingrese un número válido: \n");
        scanf("%d", &resultado);
    }
    d->M_filas_bussines = resultado;
    num_procesadores -= d->M_filas_bussines; // Restamos los procesadores usados para bussines


    printf("Ingrese un numero de filas para economy: \n");
    scanf("%d", &resultado);
        while(resultado > num_procesadores-1 || resultado < 1) { // Reservamos 1 procesador para la fila de internacionales
        printf("El número de filas para economy no puede ser mayor que el número de procesadores disponibles, abarcar todas las filas o ser menor que 1. Ingrese un número válido: \n");
        scanf("%d", &resultado);
    }
    d->M_filas_economy = resultado;
    num_procesadores -= d->M_filas_economy; // Restamos los procesadores usados para economy


    printf("Ingrese un numero de filas para internacionales: \n");
    scanf("%d", &resultado);
        while(resultado > num_procesadores || resultado < 1) { // No podemos reservar más procesadores de los disponibles
            printf("El número de filas para internacionales no puede ser mayor que el número de procesadores disponibles, abarcar todas las filas o ser menor que 1. Ingrese un número válido: \n");
            scanf("%d", &resultado);
        }
    d->M_filas_internacionales = resultado;
    num_procesadores -= d->M_filas_internacionales; // Restamos los procesadores usados para internacionales

    d->M_filas_total = d->M_filas_bussines + d->M_filas_economy + d->M_filas_internacionales;// Calculamos el total de filas


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
    d->colas_filas = (Cola*)malloc(d->M_filas_total * sizeof(Cola)); // Reservamos memoria para el array de colas
    for (int i = 0; i < d->M_filas_total; i++)   
        {
            cola_init(&d->colas_filas[i]); // Inicializamos cada cola, implicitamente los indices ya recopilados indican a que tipo de fila corresponde, por ejemplo, las primeras M_filas_bussines son para bussines, las siguientes M_filas_economy para economy y las últimas M_filas_internacionales para internacionales

        }
    return args;
}