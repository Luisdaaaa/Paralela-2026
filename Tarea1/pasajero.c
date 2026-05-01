#include "pasajero.h"


void pasajero_init(Pasajero* this, int numeroPasajero, int numFila, int tipoPasajero, double tiempoAbordaje) {
    this->numeroPasajero = numeroPasajero;
    this->numFila = numFila;
    this->tipoPasajero = tipoPasajero;
    this->tiempoAbordaje = tiempoAbordaje;
}

void pasajero_print(const Pasajero* this) {
    char* strTipoPasajero;
    if (this->tipoPasajero == 0) {
        strTipoPasajero = "Economy";
    } else if (this->tipoPasajero == 1) {
        strTipoPasajero = "Ejecutivo";
    } else {
        strTipoPasajero = "Internacional";
    }
    printf("Pasajero %d: Posición: %d, Tipo: %s, Fila: %s, Tiempo de abordaje: %.2f\n",
        this->numeroPasajero, this->numFila, strTipoPasajero, this->filaPerteneciente, this->tiempoAbordaje);
}