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

bool balancear(Cola* self, int limite){
    //Por hacer, en duda
}

bool adelantar (Cola* self, bool ejecutivo){
    //Por hacer, en duda
}