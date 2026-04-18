#include "cola.h"
#include <stdlib.h>

void cola_init(Cola* self, bool ejecutiva) {
    self->cabeza = NULL;
    self->cola = NULL;
    self->tamaño = 0;
    self->ejecutiva = ejecutiva;
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

Pasajero pop (Cola* self){

    if (cola_vacia(self)){
        Pasajero invalido = {0, 0, 0};
        return invalido;
    }

    // 1. Guardamos una referencia al nodo que vamos a eliminar
    Nodo* nodo_a_eliminar = self->cabeza;
    
    // 2. Extraemos el pasajero del nodo ANTES de liberarlo
    Pasajero pasajero_a_devolver = nodo_a_eliminar->pasajero;

    // 3. Ajustamos los punteros de la cola
    if(self->tamaño == 1){
        self->cabeza = NULL;
        self->cola = NULL;
    }
    else{
        self->cabeza = self->cabeza->siguiente;
        self->cabeza->anterior = NULL;
    }

    // 4. Liberamos la memoria del nodo y reducimos el tamaño
    free(nodo_a_eliminar);
    self->tamaño--;

    // 5. Devolvemos el pasajero que habíamos guardado
    return pasajero_a_devolver;
}