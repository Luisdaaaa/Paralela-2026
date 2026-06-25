#include <vector>
#include <cstdlib>
#include <cmath>
#include <mpi.h>
#include "export.hpp"
using namespace std;

class ParticleSimulator {
    private:
        vector<Particle> local_particles;
        vector<Particle> remote_particles;
        int N; // Number of particles
        int iterations; // Number of iterations
        int bandera_imp; // Flag for printing vtk files
        int INI; //Particle static initialization
        int rank; // Rank of the process
        int size; // Total number of processes
    public:
        ParticleSimulator(int N, int iterations, int bandera_imp, int INI, int rank, int size);
        void initialize_particles( int rank);
        void execute_simulation();
        bool update_Properties();
        bool merge(vector<Particle>& returned_particles, vector<Particle>& local_particles);
        bool evolve(vector<Particle>& local_particles, vector<Particle>& remote_particles);
        MPI_Datatype particle_Create();
};