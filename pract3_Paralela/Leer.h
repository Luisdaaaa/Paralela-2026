//
// Created by Windows on 02/05/2026.
//

#ifndef LEER_H
#define LEER_H
#include <fstream>


class Leer {
    std::ifstream file {"C:/Users/Windows/CLionProjects/Paralela-2026/pract3_Paralela/Matrices.txt",
        std::ios::in | std::ios::out};


    int N;
public:

    Leer() = default;
    ~Leer() = default;
    double** inicializaMatriz( int );
    void iniciar(double** matrizA, double** matrizB );
    void liberar_matriz(double** matriz, int n);
    int setN();

};



#endif //LEER_H
