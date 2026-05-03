//
// Created by Windows on 29/04/2026.
//

#include "Leer.h"
#include <iostream>

Leer::Leer() {
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

int** Leer::inicializaMatriz(int n) {
  int** matriz = new int*[n];
  for (int i = 0; i < n; i++) {
    matriz[i] = new int[n] ;
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      matriz[i][j] = 0;
    }
  }
  return matriz;
}


