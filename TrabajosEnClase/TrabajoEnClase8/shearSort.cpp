#include "shearSort.hpp"

void shearSort::sort(std::vector<std::vector<int>> matrix, int N) {
     for (int i = 0; i < N; i++) {
          for (int j = 0; j < N; j++) {
            std::cout << matrix[i][j] << " ";
          }
          std::cout << std::endl;
        }
    while(true) {
        #pragma omp parallel for
        for(int i = 0; i < N; i++){
            if(i % 2 == 0) {
            // Filas pares
                std::sort(matrix[i].begin(), matrix[i].end());
            } else {
            // Filas impares
                std::sort(matrix[i].rbegin(), matrix[i].rend());
            }
        }
        if(verificar(matrix)){return;}
        #pragma omp parallel for
        for(int j = 0; j < N; j++){
            std::vector<int> col_temp(N);
            // Copiar
         for(int i = 0; i < N; i++) col_temp[i] = matrix[i][j];
        
            std::sort(col_temp.begin(), col_temp.end());
        
            // Devolver
            for(int i = 0; i < N; i++) matrix[i][j] = col_temp[i];
        }

        if(verificar(matrix)){return;}
   

    }
    std::cout << "Matriz ordenada:" << std::endl;
        for (int i = 0; i < N; i++) {
          for (int j = 0; j < N; j++) {
            std::cout << matrix[i][j] << " ";
          }
          std::cout << std::endl;
        }
        for (int i = 0; i < N; i++) {
          for (int j = 0; j < N; j++) {
            std::cout << matrix[i][j] << " ";
          }
          std::cout << std::endl;
        }
}

void shearSort::inicializar(int N){
    this->N = N;
    matrix.resize(N, std::vector<int>(N));
    for(int i=0; i<N; i++){
            for(int j=0; j<N; j++){
                matrix[i][j]=rand()%100;    
            }
        }
       sort(matrix, N);
}

bool shearSort::verificar(std::vector<std::vector<int>> matrix){
    for (int i = 0; i < N; i++) {
          for (int j = 0; j < N; j++) {
            std::cout << matrix[i][j] << " ";
          }
          std::cout << std::endl;
        }
    int valorAnterior = matrix[0][0];
    for(int i=0; i<N; i++){
        for(int j=1; j<N; j++){
            if(valorAnterior>matrix[i][j]){
                return false;
            }
            valorAnterior = matrix[i][j];
        }
    }
    return true;
}