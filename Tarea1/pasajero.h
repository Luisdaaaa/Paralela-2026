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

void pasajero_init(Pasajero* self, int numeroPasajero, int numFila, bool ejecutivo, double tiempoAbordaje);
void pasajero_print(const Pasajero* self);

#endif /* PASAJERO_H */
