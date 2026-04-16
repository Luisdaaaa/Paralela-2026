#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define hilos_fumadores 3

typedef enum{
    FUMANDO,
    CREANDO,
    ESPERANDO
} State;

State state[hilos_fumadores];

pthread_mutex_t mutex;
pthread_cond_t condFumadores[hilos_fumadores];
pthread_cond_t condAgente;
int ingredientes_en_mesa[2];

void* agente(void* arg);
void* fumador(void* arg);
bool proveer_ingredientes(void);
bool crear_cigarro(int hilo);
void fumar(int hilo);
void esperar(int hilo);


int main(){
    srand(time(NULL));
    ingredientes_en_mesa[0] = -1;
    ingredientes_en_mesa[1] = -1;

    // Inicialización de mutex y condiciones
    pthread_mutex_init(&mutex, NULL);
    for(int i = 0; i < hilos_fumadores; i++){
        pthread_cond_init(&condFumadores[i], NULL);
    }
    pthread_cond_init(&condAgente, NULL);

    // Crear hilo del agente
    pthread_t agente_thread;
    pthread_create(&agente_thread, NULL, agente, NULL);

    // Crear hilos de los fumadores
    pthread_t fumadores_threads[hilos_fumadores];
    int ids[hilos_fumadores];
    for(int i = 0; i < hilos_fumadores; i++){
        ids[i] = i;
        pthread_create(&fumadores_threads[i], NULL, fumador, &ids[i]);
        state[i] = ESPERANDO; // Inicializar el estado de los fumadores
    }

    pthread_join(agente_thread, NULL);
    for(int i = 0; i < hilos_fumadores; i++){
        pthread_join(fumadores_threads[i], NULL);
    }

    return 0;
}

void* agente(void* arg){
    (void)arg;
    while(1){
        proveer_ingredientes();// Proveer ingredientes a la mesa
    }

    return NULL;
}

void* fumador(void* arg){
    int id = *((int*)arg);
    while(1){
        esperar(id);
        crear_cigarro(id);
        fumar(id);
    }

    return NULL;
}

bool proveer_ingredientes(void){
    // Lógica para proveer ingredientes a la mesa, con un random y llenando el array
    //sin repetir ingredientes
    pthread_mutex_lock(&mutex);
    while(ingredientes_en_mesa[0]!=-1 || ingredientes_en_mesa[1]!=-1){
        // Espera a que la mesa esté vacía antes de proveer nuevos ingredientes
        pthread_cond_wait(&condAgente, &mutex);
    }
    int ingrediente1 = rand() % hilos_fumadores;
    int ingrediente2;
    do {
        ingrediente2 = rand() % hilos_fumadores;
    } while (ingrediente2 == ingrediente1);
    ingredientes_en_mesa[0] = ingrediente1;
    ingredientes_en_mesa[1] = ingrediente2;
    for(int i = 0; i < hilos_fumadores; i++){
        pthread_cond_signal(&condFumadores[i]);
    }
    pthread_mutex_unlock(&mutex);
    return true; // Retorna true si se proveyeron los ingredientes correctamente
}

bool crear_cigarro(int hilo) {
    pthread_mutex_lock(&mutex);

    while (ingredientes_en_mesa[0] == hilo || ingredientes_en_mesa[1] == hilo) {
        // Si mi ingrediente infinito está en la mesa, NO puedo armar cigarro.
        // Me duermo hasta que el agente cambie la mesa.
        pthread_cond_wait(&condFumadores[hilo], &mutex);
    }

    state[hilo] = CREANDO;
    printf("Fumador %d creando cigarro\n", hilo);
    sleep(1); // Simula el tiempo de crear el cigarro
    // Limpiar la mesa después de crear el cigarro
    ingredientes_en_mesa[0] = -1;
    ingredientes_en_mesa[1] = -1;
    pthread_cond_signal(&condAgente);

    pthread_mutex_unlock(&mutex);
    return true;
}

void fumar(int hilo)
{   // Lógica para simular el acto de fumar, cambia el estado y imprime, sleep para simular el tiempo de fumar
    state[hilo] = FUMANDO;
    printf("Fumador %d fumando\n", hilo);
    sleep(2); // Simula el tiempo de fumar
}

void esperar(int hilo)
{   // Lógica para simular el acto de esperar, cambia el estado y imprime, sleep para simular el tiempo de espera, no confundir con un dormir al hilo, es solo para simular el tiempo de espera
    state[hilo] = ESPERANDO;
    printf("Fumador %d esperando\n", hilo);
    sleep(1); // Simula el tiempo de espera
}

