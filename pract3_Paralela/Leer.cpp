//
// Created by Windows on 02/05/2026.
//
#include "Leer.h"
#include <iostream>

void Leer::iniciar(double** matrizA, double** matrizB) {
    try {
        if (!file) {
            throw std::string("Error al abrir archivo");
        }
    } catch (std::string e) {
        std::cout << e << std::endl;
    }
    while (!file.eof()) {
        file >> N;

        matrizA = inicializaMatriz(N);
        matrizB = inicializaMatriz(N);

    }
}

double** Leer::inicializaMatriz(int n) {
    double** matriz = new double*[n];
    for (int i = 0; i < n; i++) {
        matriz[i] = new double[n] ;
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matriz[i][j] = 0;
        }
    }
    return matriz;
}

void Leer::liberar_matriz(double** matriz, int n){
    for (int i = 0; i < n; i++) {
        delete[] matriz[i];
    }
    delete[] matriz;
}

int Leer::setN() {
    return N;
}