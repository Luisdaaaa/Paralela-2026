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

    while (contador<1000){
        if(rank==0){
            MPI_Send( &contador, 1 , MPI_INT , 1 , 0 , MPI_Comm MPI_COMM_WORLD );
            MPI_Recv( &contador, 1 , MPI_INT , 1 , 0 , MPI_Comm MPI_COMM_WORLD , NULL);
            contador++;
            std::cout << "Aumentando el contador desde el hilo  0" << std::endl;
            std::cout << contador << std::endl;


        }else{
            MPI_Send( &contador, 1 , MPI_INT , 0, 0 , MPI_Comm MPI_COMM_WORLD );
            MPI_Recv( &contador, 1 , MPI_INT , 0, 0 , MPI_Comm MPI_COMM_WORLD , NULL);
            contador++;
            std::cout << "Aumentando el contador desde el hilo  1" << std::endl;
            std::cout << contador << std::endl;

        }
    }
    
     MPI_Finalize();
    return 0;
}