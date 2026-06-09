# include <mpi.h>
# include <stdio.h>
# include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
int main(int argc, char* argv[]) {

    MPI_Init(&argc, &argv);

    int rank;
    int size; 
    int total = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    srand(time(NULL) + rank);
    int random_value = rand()%size;
        if(rank == 0){
            total += random_value;
            std::cout << "Sumando desde el proceso " << rank << " Total: " << total << std::endl;
            std::cout << "Enviando al proceso " << rank+1 << std::endl;
            MPI_Send(&total, 1, MPI_INT, (rank+1), 0, MPI_COMM_WORLD);
            MPI_Recv(&total, 1, MPI_INT, (size-1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "Recibiendo del proceso " << size-1 << " Total: " << total << std::endl;
            
        } else {
            // El hilo 1 espera el valor, aumenta, imprime y lo devuelve
            MPI_Recv(&total, 1, MPI_INT, (rank-1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total += random_value;
            if(rank < size-1){
                std::cout << "Sumando desde el proceso " << rank << " Total: " << total << std::endl;
                std::cout << "Enviando al proceso " << rank+1 << std::endl;
                MPI_Send(&total, 1, MPI_INT, (rank+1), 0, MPI_COMM_WORLD);
            }
            else {
                std::cout << "Sumando desde el proceso " << rank << " Total: " << total << std::endl;
                std::cout << "Enviando al proceso " << 0 << std::endl;
                MPI_Send(&total, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }
    
    MPI_Finalize();
    return 0;
}