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

bool balancear(Cola* self, int limite);// Balancea la cola para mantener un número máximo de pasajeros ejecutivos

bool adelantar (Cola* self, bool ejecutivo);// Adelanta un pasajero en la cola

#endif
