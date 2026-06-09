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
    int num_Aleatorio = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    srand(time(NULL) + rank);
    int p = 20;
    num_Aleatorio = rand()%p;
    std::cout << "Numero: " << num_Aleatorio << " En proceso:" << rank << std::endl;
    MPI_Reduce(&num_Aleatorio, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
              
    MPI_Bcast(&total, 1, MPI_INT, 0, MPI_COMM_WORLD);
    std::cout << "El total es: " << total << " Desde el hilo " << rank << std::endl;
        
    
    MPI_Finalize();
    return 0;
}