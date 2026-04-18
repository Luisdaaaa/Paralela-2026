#include "pasajero.h"


void pasajero_init(Pasajero* this, int numeroPasajero, int numFila, bool ejecutivo, double tiempoAbordaje) {
    this->numeroPasajero = numeroPasajero;
    this->numFila = numFila;
    this->ejecutivo = ejecutivo;
    this->tiempoAbordaje = tiempoAbordaje;
}

void pasajero_print(const Pasajero* this) {
    printf("Pasajero %d: Fila %d, Ejecutivo: %s, Tiempo de abordaje: %.2f\n",
        this->numeroPasajero, this->numFila, this->ejecutivo ? "Ejecutivo" : "Economy", this->tiempoAbordaje);
}