# include <mpi.h>
# include <stdio.h>
# include <stdlib.h>
#include <iostream>
int main(int argc, char* argv[]) {

    MPI_Init(&argc, &argv);

    int rank;
    int size; 

    int contador = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    while (contador < 1000) {
        if(rank == 0){
            // El hilo 0 aumenta primero, imprime y envía
            contador++;
            std::cout << "Aumentando el contador desde el hilo  0\n" << contador << std::endl;
            MPI_Send(&contador, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            
            // Recibe la respuesta del hilo 1 para estar sincronizados
            MPI_Recv(&contador, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
        } else {
            // El hilo 1 espera el valor, aumenta, imprime y lo devuelve
            MPI_Recv(&contador, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            contador++;
            std::cout << "Aumentando el contador desde el hilo  1\n" << contador << std::endl;
            MPI_Send(&contador, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }
    
    MPI_Finalize();
    return 0;
}