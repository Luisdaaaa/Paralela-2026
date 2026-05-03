//
// Created by Windows on 29/04/2026.
//

#ifndef LEER_H
#define LEER_H
#include <fstream>



class Leer {
  public:
    std::ifstream file
    int** matrizA;
    int** matrizB;
    int N;

    Leer();
    // ~Leer();
    int** inicializaMatriz( int );
};



#endif //LEER_H
