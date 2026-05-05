#include "cola.h"
#include <stdlib.h>

void cola_init(Cola* self) {
    self->cabeza = NULL;
    self->cola = NULL;
    self->tamaño = 0;
}

bool cola_vacia(const Cola* self) {
    return self->tamaño == 0;
}

bool push(Cola* self, Pasajero pasajero){

    Nodo* nuevo_nodo= (Nodo*)malloc(sizeof(Nodo));
    if (nuevo_nodo==NULL){
        return false;
    }

    nuevo_nodo->pasajero=pasajero;
    nuevo_nodo->siguiente=NULL;
    nuevo_nodo->anterior=NULL;

    if (cola_vacia(self)){
        self->cabeza=nuevo_nodo;
        self->cola=nuevo_nodo;
    } else {
        self->cola->siguiente=nuevo_nodo;
        nuevo_nodo->anterior=self->cola;
        self->cola=nuevo_nodo;
    }
    self->tamaño++;
    return true;
}


bool pop (Cola* self){
    if (cola_vacia(self)){
        return false;
    }
    else{
        if(self->tamaño==1){
            free(self->cabeza);
            self->cabeza=NULL;
            self->cola=NULL;
        }
        else{
            Nodo* nodo_a_eliminar=self->cabeza;
        self->cabeza=self->cabeza->siguiente;
        self->cabeza->anterior=NULL;
        free(nodo_a_eliminar);
        }
    }
    self->tamaño--;
    return true;
}

Pasajero* frente(const Cola* self){
    if (cola_vacia(self)){
        return NULL;
    }
    return &(self->cabeza->pasajero);
}

bool adelantar_pasajero(Cola* self, Cola* destino) {
    if (cola_vacia(self) || destino == NULL || self == destino) {
        return false; // No hay pasajeros o parámetros inválidos
    }

    Nodo* nodo_a_adelantar = self->cabeza; // Tomamos el primer nodo de la cola original
    Nodo* cabezaOriginal = self->cabeza; // Guardamos la nueva cabeza de la cola original, que será la nueva cabeza después de adelantar
    int movidos = 1;

    for(int i = 0; i <30; i++) {
        
        if (nodo_a_adelantar == NULL) {
            return false; // No hay más pasajeros para revisar
        }
        if (nodo_a_adelantar->siguiente == NULL) {
            break; // Hemos llegado al final de la cola 
        }
        movidos++;

        nodo_a_adelantar = nodo_a_adelantar->siguiente; // Avanzamos al siguiente nodo

    }
    if(nodo_a_adelantar->siguiente != NULL) {
        self->cabeza = nodo_a_adelantar->siguiente; // La nueva cabeza de la cola original será el nodo siguiente al nodo a adelantar
        self->cabeza->anterior = NULL; // El nodo anterior del primer nodo de la cola original ahora es NULL
        nodo_a_adelantar->siguiente = NULL; // El nodo a adelantar siguiente ahora es NULL, para disvincularlo de la cola original
    } else {
        self->cola= NULL; // Si el nodo a adelantar es el último nodo, entonces la cola original quedará vacía
        self->cabeza = NULL; // La cabeza de la cola original también se establece en NULL
    }
    if (destino->cabeza != NULL) {
        nodo_a_adelantar->siguiente = destino->cabeza; // El nodo siguiente del nodo a adelantar ahora es el siguiente al nodo adelantar
        destino->cabeza->anterior = nodo_a_adelantar; // El nodo anterior del primer nodo de la cola destino ahora es el nodo a adelantar
        destino->cabeza = cabezaOriginal;
    } else {
        destino->cabeza = cabezaOriginal; // Si la cola destino está vacía, la cabeza de la cola original se convierte en la cabeza de la cola destino
        destino->cola = nodo_a_adelantar; // El nodo a adelantar se convierte en la cola de la cola destino
    }
    destino->tamaño += movidos; // Incrementamos el tamaño de la cola destino
    self->tamaño -= movidos; // Decrementamos el tamaño de la cola original
    return true; // Pasajero adelantado exitosamente
}

bool balancear(Cola* self, Cola* destino, int cantidad) {
    if (cola_vacia(self) || destino == NULL || self == destino || cantidad <= 0) {
        return false;
    }

    int movidos = 1;
    Nodo* nodo_a_mover = self->cola; // Tomamos la cola de la cola original
    Nodo* colaOriginal = self->cola; // Guardamos el nodo que será la nueva cola de la destino[cite: 1]

    //Identificar el bloque de atrás hacia adelante
    while (movidos < cantidad && nodo_a_mover->anterior != NULL) {
        nodo_a_mover = nodo_a_mover->anterior; // Avanzamos al nodo anterior[cite: 1]
        movidos++; 
    }

    // Desvinculación del bloque de la cola original
    if (nodo_a_mover->anterior != NULL) {
        
        self->cola = nodo_a_mover->anterior;
        self->cola->siguiente = NULL; 
    } else {
        
        self->cabeza = NULL;
        self->cola = NULL;
    }
    nodo_a_mover->anterior = NULL;

    // Vinculación a la cola destino
    if (!cola_vacia(destino)) {
        destino->cola->siguiente = nodo_a_mover;
        nodo_a_mover->anterior = destino->cola; 
        destino->cola = colaOriginal; 
    } else {
        destino->cabeza = nodo_a_mover;
        destino->cola = colaOriginal;
    }
    colaOriginal->siguiente = NULL;

    //Actualización de tamaños
    destino->tamaño += movidos;
    self->tamaño -= movidos;

    return true;
}
