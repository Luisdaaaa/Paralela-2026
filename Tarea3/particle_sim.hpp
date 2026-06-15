#include "particle.hpp"
#include <vector>
#include <cstdlib>
#include <cmath>
using namespace std;

class ParticleSimulator {
    private:
        vector<Particle> local_particles;
        vector<Particle> remote_particles;
        int N; // Number of particles
        int iterations; // Number of iterations
        bool bandera_imp; // Flag for printing vtk files
        bool INI; //Particle static initialization
    public:
        ParticleSimulator(int N, int iterations, bool bandera_imp, bool INI);
        void initialize_particles(int total_processes, int rank);
        bool execute_simulation();
        bool update_Properties();
        bool merge(vector<Particle>& returned_particles, vector<Particle>& local_particles);
        bool evolve(vector<Particle>& local_particles, vector<Particle>& remote_particles);
};