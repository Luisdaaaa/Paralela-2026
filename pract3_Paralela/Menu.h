//
// Created by Windows on 03/05/2026.
//

#ifndef MENU_H
#define MENU_H
#include "AlgStrassen.h"


class Menu {
    double** matrizA;
    double** matrizB;
    Leer leer;
    AlgStrassen algS;
    int n;
    public:
      Menu();
      ~Menu() = default;
      void crearMatrizAleatoria(int n);
};



#endif //MENU_H
