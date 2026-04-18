#ifndef COLA_H
#define COLA_H
#include "pasajero.h"

typedef struct Nodo {
    Pasajero pasajero;
    struct Nodo* siguiente;
    struct Nodo* anterior;
} Nodo;

typedef struct {
    Nodo* cabeza=NULL;
    Nodo* cola=NULL;
    int tamaño;
} Cola;

bool cola_vacia(const Cola* this );

bool push(Cola* this, Pasajero pasajero);

Pasajero pop(Cola* this);

bool 

#endif
