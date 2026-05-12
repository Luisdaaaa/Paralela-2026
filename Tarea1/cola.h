#ifndef COLA_H
#define COLA_H
#include "pasajero.h"

typedef struct Nodo {
    Pasajero pasajero;
    struct Nodo* siguiente;
    struct Nodo* anterior;
} Nodo;

typedef struct {
    Nodo* cabeza;
    Nodo* cola;
    int tamaño;
} Cola;

void cola_init(Cola* self);// Inicializa la cola

bool cola_vacia(const Cola* self );// Verifica si la cola está vacía

bool push(Cola* self, Pasajero pasajero);// Agrega un pasajero a la cola

bool pop(Cola* self);// Elimina y libera el primer pasajero de la cola

Pasajero* frente(const Cola* self);// Devuelve un puntero al primer pasajero de la cola sin eliminarlo

bool adelantar_pasajero(Cola* self, Cola* destino);//adelanta pasajeros a internaiconal por tiempo de atención 

bool balancear(Cola* self, Cola* destino, int cantidad); //balancea pasajeros a internaiconal

#endif
