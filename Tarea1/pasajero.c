#include "pasajero.h"


void pasajero_init(Pasajero* self, int numeroPasajero, int numFila, bool ejecutivo, double tiempoAbordaje) {
    self->numeroPasajero = numeroPasajero;
    self->numFila = numFila;
    self->ejecutivo = ejecutivo;
    self->tiempoAbordaje = tiempoAbordaje;
}

void pasajero_print(const Pasajero* this) {
    printf("Pasajero %d: Fila %d, Ejecutivo: %s, Tiempo de abordaje: %.2f\n",
        this->numeroPasajero, this->numFila, this->ejecutivo ? "Ejecutivo" : "Economy", this->tiempoAbordaje);
}