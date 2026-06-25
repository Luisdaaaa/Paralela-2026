#include <iostream>
#include <omp.h>
#include "particle_sim.hpp"

int main(int argc, char** argv) {
    if (argc < 5) {
        std::cerr << "Usage: " << argv[0] << " <number_of_particles> <number_of_iterations> <print_flag> <initialization_flag>" << std::endl;
        return 1;
    }

    MPI_Init(&argc, &argv); 

    int rank , size ;
    
    MPI_Comm_rank ( MPI_COMM_WORLD , & rank ) ;
    MPI_Comm_size ( MPI_COMM_WORLD , & size ) ;
    if (size %2 == 0) {
        
        std::cerr << "Error: The number of processes must be odd." << std::endl;
        MPI_Finalize();

        return 1;
    }
    int num_particles = std::stoi(argv[1]);
    int num_iterations = std::stoi(argv[2]);
    int print_flag = std::stoi(argv[3]);
    int init_flag = std::stoi(argv[4]);

    ParticleSimulator sim(num_particles, num_iterations, print_flag, init_flag, rank, size);
    sim.initialize_particles(rank);
    sim.execute_simulation();

    MPI_Finalize();
    return 0;
}

