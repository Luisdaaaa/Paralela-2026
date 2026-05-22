#include "Menu.h"
#include <iostream>



std::vector<std::vector<int>> Menu::iniciar() {
    std::vector<std::vector<int>> matrices;
    try {
        if (!file.is_open()) {
            throw std::string("Error al abrir archivo");
        }
    } catch (std::string e) {
        std::cout << e << std::endl;
    }
    while (file >> N) {
        
        matrices.resize(N, std::vector<int>(N));
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                file >> matrices[i][j];
            }
        }
    }
     for (int i = 0; i < N; i++) {
          for (int j = 0; j < N; j++) {
            std::cout << matrices[i][j] << " ";
          }
          std::cout << std::endl;
        }
    return matrices;
}

Menu::Menu() {

    while(true){
    std::cout << "1.Matriz aleatoria" << std::endl
            << "2.Matriz por archivo" << std::endl
            << "Cualquier otro boton para salir" << std::endl;
    std::cin >> opciones;

    switch (opciones) {
      case 1: {

        std::cout << "Escribe el tamano de la matriz" << std::endl;
        std::cin >> N;
        shear.inicializar(N);
       
        break;
      }case 2: {
        std::vector<std::vector<int>> matriz = iniciar();
       
        shear.sort(matriz, N);
        
        break;
       
      }default:{ return;}
    }
}

}