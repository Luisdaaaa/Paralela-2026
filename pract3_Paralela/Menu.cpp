//
// Created by Windows on 03/05/2026.
//

#include "Menu.h"
#include <iostream>

void Menu::crearMatrizAleatoria(int n){
  matrizA = leer.inicializaMatriz(n);
  matrizB = leer.inicializaMatriz(n);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      matrizA[i][j] = std::rand() %100;
    }
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      matrizA[i][j] = std::rand() %100;
    }
  }
}

Menu::Menu(){
  int opciones;
  double** matrizC = nullptr;
  while(true){
    std::cout << "1.Matriz aleatoria" << std::endl
        << "2.Matriz por archivo" << std::endl
        << "Cualquier otro boton para salir" << std::endl;
    std::cin >> opciones;

    switch (opciones) {
      case 1: {

        std::cout << "Escribe el tamano de la matriz" << std::endl;
        std::cin >> n;
        matrizC = leer.inicializaMatriz(n);
        crearMatrizAleatoria(n); algS.strassen(matrizA,matrizB,matrizC,n); break;
      }case 2: {

        leer.iniciar(matrizA,matrizB);
        n = leer.setN();
        matrizC = leer.inicializaMatriz(n);
        algS.strassen(matrizA,matrizB,matrizC,n);break;
      }default:{ return;}
    }

    std::cout << "Nueva matriz:" << std::endl;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        std::cout << matrizC[i][j] << " ";
      }
      std::cout << std::endl;
    }
  }
}

