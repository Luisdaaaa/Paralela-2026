#include <iostream>
#include "matriz.hpp"
#include <algorithm>

int main() {
    matriz m;
    m.llenarMatriz();
    m.imprimirMatriz();
    std::cout << "Ordenando la matriz..." << std::endl;
    std::vector<std::vector<int>> matriz = m.getMatriz();
    int iteracion = 0;
    while(true){
        bool ordenamiento_realizado=false;
        #pragma omp parallel for schedule(static)
        for(size_t i=0; i < matriz.size(); i++) {
            bool fila_cambiada = false;
            if(i%2==0){
                if(!std::is_sorted(matriz[i].begin(), matriz[i].end())) {
                    std::sort(matriz[i].begin(), matriz[i].end());
                    fila_cambiada = true;
                }
            }
            else{
                if(!std::is_sorted(matriz[i].begin(), matriz[i].end(), std::greater<int>())) {
                    std::sort(matriz[i].begin(), matriz[i].end(), std::greater<int>());
                    fila_cambiada = true;
                }
            }
            if(fila_cambiada) {
                #pragma omp critical
                {
                    ordenamiento_realizado = true;
                }
            }
        }
        if(ordenamiento_realizado == false && iteracion > 0){
            std::cout << "\nMatriz Ordenada:\n";
            for(size_t i=0; i < matriz.size(); i++){
                for(size_t j=0; j < matriz[i].size(); j++){
                    std::cout << matriz[i][j] << " ";
                }
                std::cout << std::endl;
            }
            break;
        }
        else{
            std::vector<std::vector<int>> matriz_transpuesta=matriz;
            #pragma omp parallel for schedule(static)
            for(size_t i=0; i < matriz.size(); i++) {
                for(size_t j=0; j < matriz[i].size(); j++) {
                    matriz_transpuesta[j][i] = matriz[i][j];
                }
            }
            #pragma omp parallel for schedule(static)
            for(size_t i=0; i < matriz.size(); i++) {
                    std::sort(matriz_transpuesta[i].begin(), matriz_transpuesta[i].end());
            }
            #pragma omp parallel for schedule(static)
            for(size_t i=0; i < matriz.size(); i++) {
                for(size_t j=0; j < matriz[i].size(); j++) {
                    matriz[i][j] = matriz_transpuesta[j][i];
                }
            }
            iteracion++;
        }
    }
    return 0;
}