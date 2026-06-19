#include "matriz.hpp"

void matriz::llenarMatriz() {
    int n;
    std::cout << "Ingrese el tamanio de la matriz cuadrada (N): ";
    std::cin >> n;

    matriz.resize(n, std::vector<int>(n));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matriz[i][j] = rand() % 100; // Llenar con números aleatorios entre 0 y 99
        }
    }
}

void matriz::imprimirMatriz() {
    for (const auto& fila : matriz) {
        for (const auto& elemento : fila) {
            std::cout << elemento << " ";
        }
        std::cout << std::endl;
    }
}