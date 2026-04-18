#ifndef PASAJERO_H
#define PASAJERO_H
#include <stdbool.h>
#include <stdio.h>

typedef struct {
    int numeroPasajero;
    int numFila;
    bool ejecutivo;
    double tiempoAbordaje;
} Pasajero;

void pasajero_init(Pasajero* this, int numeroPasajero, int numFila, bool ejecutivo, double tiempoAbordaje);
void pasajero_print(const Pasajero* this);

#endif /* PASAJERO_H */
