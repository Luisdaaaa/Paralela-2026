//
// Created by Windows on 02/05/2026.
//

#include "AlgStrassen.h"

void AlgStrassen::sumar(double** A, double** B, double** C, int n) {
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

void AlgStrassen::restar(double** A, double** B, double** C, int n) {
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            C[i][j] = A[i][j] - B[i][j];
        }
    }
}

void AlgStrassen::strassen(double** A, double** B, double** C, int n) {

    if (n <= 64) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                C[i][j] = 0;
                for (int k = 0; k < n; k++){
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
        return;
    }

    int k = n / 2;

    double** A11 = leer.inicializaMatriz(k); double** A12 = leer.inicializaMatriz(k);
    double** A21 = leer.inicializaMatriz(k); double** A22 = leer.inicializaMatriz(k);
    double** B11 = leer.inicializaMatriz(k); double** B12 = leer.inicializaMatriz(k);
    double** B21 = leer.inicializaMatriz(k); double** B22 = leer.inicializaMatriz(k);

    double** M1 = leer.inicializaMatriz(k); double** M2 = leer.inicializaMatriz(k);
    double** M3 = leer.inicializaMatriz(k); double** M4 = leer.inicializaMatriz(k);
    double** M5 = leer.inicializaMatriz(k); double** M6 = leer.inicializaMatriz(k);
    double** M7 = leer.inicializaMatriz(k);


    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + k];
            A21[i][j] = A[i + k][j];
            A22[i][j] = A[i + k][j + k];

            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + k];
            B21[i][j] = B[i + k][j];
            B22[i][j] = B[i + k][j + k];
        }
    }


    #pragma omp parallel
    {
        #pragma omp single
        {
            #pragma omp task // M1 = (A11 + A22)(B11 + B22)
            {
                double** tA =  leer.inicializaMatriz(k);; double** tB =  leer.inicializaMatriz(k);;
                sumar(A11, A22, tA, k); sumar(B11, B22, tB, k);
                strassen(tA, tB, M1, k);
                leer.liberar_matriz(tA, k); leer.liberar_matriz(tB, k);
            }
            #pragma omp task // M2 = (A21 + A22)B11
            {
                double** tA =  leer.inicializaMatriz(k);;
                sumar(A21, A22, tA, k);
                strassen(tA, B11, M2, k);
                leer.liberar_matriz(tA, k);
            }
            #pragma omp task // M3 = A11(B12 - B22)
            {
                double** tB =  leer.inicializaMatriz(k);;
                restar(B12, B22, tB, k);
                strassen(A11, tB, M3, k);
                leer.liberar_matriz(tB, k);
            }
            #pragma omp task // M4 = A22(B21 - B11)
            {
                double** tB =  leer.inicializaMatriz(k);;
                restar(B21, B11, tB, k);
                strassen(A22, tB, M4, k);
                leer.liberar_matriz(tB, k);
            }
            #pragma omp task // M5 = (A11 + A12)B22
            {
                double** tA =  leer.inicializaMatriz(k);;
                sumar(A11, A12, tA, k);
                strassen(tA, B22, M5, k);
                leer.liberar_matriz(tA, k);
            }
            #pragma omp task // M6 = (A21 - A11)(B11 + B12)
            {
                double** tA =  leer.inicializaMatriz(k);; double** tB =  leer.inicializaMatriz(k);;
                restar(A21, A11, tA, k); sumar(B11, B12, tB, k);
                strassen(tA, tB, M6, k);
                leer.liberar_matriz(tA, k); leer.liberar_matriz(tB, k);
            }
            #pragma omp task // M7 = (A12 - A22)(B21 + B22)
            {
                double** tA =  leer.inicializaMatriz(k);; double** tB =  leer.inicializaMatriz(k);;
                restar(A12, A22, tA, k); sumar(B21, B22, tB, k);
                strassen(tA, tB, M7, k);
                leer.liberar_matriz(tA, k); leer.liberar_matriz(tB, k);
            }
            #pragma omp taskwait // Esperar que todas las tareas Mi terminen[cite: 1]
        }
    }

    // 4. Construcción final de C en paralelo[cite: 1]
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            C[i][j] = M1[i][j] + M4[i][j] - M5[i][j] + M7[i][j];           // C11
            C[i][j + k] = M3[i][j] + M5[i][j];                             // C12
            C[i + k][j] = M2[i][j] + M4[i][j];                             // C21
            C[i + k][j + k] = M1[i][j] - M2[i][j] + M3[i][j] + M6[i][j];   // C22
        }
    }

    // 5. Liberación de memoria local para evitar fugas en Kabré[cite: 1]
    leer.liberar_matriz(A11, k); leer.liberar_matriz(A12, k);
    leer.liberar_matriz(A21, k); leer.liberar_matriz(A22, k);
    leer.liberar_matriz(B11, k); leer.liberar_matriz(B12, k);
    leer.liberar_matriz(B21, k); leer.liberar_matriz(B22, k);
    leer.liberar_matriz(M1, k); leer.liberar_matriz(M2, k);
    leer.liberar_matriz(M3, k); leer.liberar_matriz(M4, k);
    leer.liberar_matriz(M5, k); leer.liberar_matriz(M6, k);
    leer.liberar_matriz(M7, k);
}