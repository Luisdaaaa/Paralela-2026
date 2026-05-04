//
// Created by Windows on 02/05/2026.
//

#ifndef ALGSTRASSEN_H
#define ALGSTRASSEN_H
#include "Leer.h"


class AlgStrassen {
    Leer leer;

  public:
    AlgStrassen(){};
    void sumar(double** A, double** B, double** C, int n);
    void restar(double** A, double** B, double** C, int n);

    void strassen(double** A, double** B, double** C, int n);
};



#endif //ALGSTRASSEN_H
