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
    printf("Número de procesadores disponibles para repartir entre todos los counters: %d\n", num_procesadores); 
    printf("Ingrese un numero de personas: \n");
    scanf("%d", &resultado);
    d->N_pasajeros = resultado;

    printf("Ingrese un numero counters para bussines: \n");
    scanf("%d", &resultado);
    while (resultado > num_procesadores-2|| resultado < 1) { // Reservamos 2 procesadores para las otras filas, 1 para economy, 1 para internacionales
        printf("El número de filas para bussines no puede ser mayor que el número de procesadores disponibles, abarcar todas las filas o ser menor que 1. Ingrese un número válido: \n");
        scanf("%d", &resultado);
    }
    d->M_counters_bussines = resultado;
    num_procesadores -= d->M_counters_bussines; // Restamos los procesadores usados para bussines


    printf("Ingrese un numero de counters para economy: \n");
    scanf("%d", &resultado);
        while(resultado > num_procesadores-1 || resultado < 1) { // Reservamos 1 procesador para la fila de internacionales
        printf("El número de filas para economy no puede ser mayor que el número de procesadores disponibles, abarcar todas las filas o ser menor que 1. Ingrese un número válido: \n");
        scanf("%d", &resultado);
    }
    d->M_counters_economy = resultado;
    num_procesadores -= d->M_counters_economy; // Restamos los procesadores usados para economy


    printf("Ingrese un numero de counters para internacionales: \n");
    scanf("%d", &resultado);
        while(resultado > num_procesadores || resultado < 1) { // No podemos reservar más procesadores de los disponibles
            printf("El número de filas para internacionales no puede ser mayor que el número de procesadores disponibles, abarcar todas las filas o ser menor que 1. Ingrese un número válido: \n");
            scanf("%d", &resultado);
        }
    d->M_counters_internacionales = resultado;
    num_procesadores -= d->M_counters_internacionales; // Restamos los procesadores usados para internacionales
    d->M_counters_total = d->M_counters_bussines + d->M_counters_economy + d->M_counters_internacionales; // Calculamos el total de counters usados


    printf("Ingrese un numero maximo de personas por fila: \n");
    scanf("%d", &resultado);
    while(resultado > d->N_pasajeros) { // No podemos tener más personas por fila que el total de pasajeros
        printf("El número máximo de personas por fila no puede ser mayor que el número total de pasajeros. Ingrese un número válido: \n");
        scanf("%d", &resultado);
    }
    d->Q_maximo_por_fila = resultado;


    printf("Ingrese un numero de tiempo para dormir la fila minimo en ms: \n");
    scanf("%d", &resultado);
    //Preguntar al profesor como se calcula esto, si es un número fijo o un porcentaje del total de pasajeros
    d->K_min = resultado;
    printf("Ingrese un numero de tiempo para dormir la fila maximo en ms: \n");
    scanf("%d", &resultado);
    d->k_max = resultado;

    printf("Ingrese un numero de tiempo minimo en ms para que el supervisor despierte a los hilos: \n");
    scanf("%d", &resultado);
    d->min_supervisor = resultado;
    
    printf("Ingrese un numero de tiempo maximo en ms para que el supervisor despierte a los hilos: \n");
    scanf("%d", &resultado);
    d->max_supervisor = resultado;

    printf("Ingrese un numero de tiempo maximo en ms de abordaje para ejecutiva: \n");
    scanf("%d", &resultado);
    d->T_tiempo_abordaje_max_ejecutiva = resultado;

    d->tiempo_hilos = (long long*)malloc(d->M_counters_total * sizeof(long long)); // Reservamos memoria para el array de tiempos
    for(int i = 0; i < d->M_counters_total; i++) {
        d->tiempo_hilos[i] = 0; // Inicializamos los tiempos en 0
    }

    d->hilos_dormidos = (bool*)malloc(d->M_counters_total * sizeof(bool)); // Reservamos memoria para el array de booleanos
    for(int i = 0; i < d->M_counters_total; i++) {
        d->hilos_dormidos[i] = false; // Inicializamos los booleanos en false, indicando que ningún hilo está dormido al inicio
    }

    args.info_compartida = d; // Asignamos la dirección de la estructura de datos al campo info_compartida de ArgumentosHilo
    d->colas_filas = (Cola*)malloc(3 * sizeof(Cola)); // Reservamos memoria para el array de colas
    for (int i = 0; i < 3; i++)   
        {
            cola_init(&d->colas_filas[i]); 

        }
    
    for(int i = 0; i < 3; i++) {
        pthread_mutex_init(&d->mutex[i], NULL); // Inicializamos los mutex para cada fila
    }
    
    d->cond = (pthread_cond_t*)malloc(d->M_counters_total * sizeof(pthread_cond_t));
    for(int i =0; i<d->M_counters_total; i++) {
        pthread_cond_init(&d->cond[i], NULL); // Inicializamos las condiciones para cada hilo
    }
    pthread_cond_init(&d->cond_supervisor, NULL); // Inicializamos la condición para el supervisor
    pthread_mutex_init(&d->mutex_supervisor, NULL); // Inicializamos el mutex para el supervisor
    pthread_mutex_init(&d->mutexBalanceador, NULL); // Inicializamos el mutex para el balanceador
    pthread_cond_init(&d->cond_balanceador, NULL); // Inicializamos la condición para el balanceador

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