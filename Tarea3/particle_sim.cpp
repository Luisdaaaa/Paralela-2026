#include "particle_sim.hpp"

ParticleSimulator::ParticleSimulator(int N, int iterations, bool bandera_imp, bool INI) {
    this->N = N;
    this->iterations = iterations;
    this->bandera_imp = bandera_imp;
    this->INI = INI;
}

void ParticleSimulator::initialize_particles(int total_processes, int rank) {
    srand(rank * 12345);
    int vector_length = this->N; 
    local_particles.resize(vector_length);
    remote_particles.resize(vector_length);
    for (int i = 0; i < vector_length; i++) {
        local_particles[i].x = static_cast<double>(rand()) / RAND_MAX;
        local_particles[i].y = static_cast<double>(rand()) / RAND_MAX;
        local_particles[i].z = static_cast<double>(rand()) / RAND_MAX;
        local_particles[i].vx = static_cast<double>(rand()) / RAND_MAX;
        local_particles[i].vy = static_cast<double>(rand()) / RAND_MAX;
        local_particles[i].vz = static_cast<double>(rand()) / RAND_MAX;
        local_particles[i].ax = 0.0;
        local_particles[i].ay = 0.0;
        local_particles[i].az = 0.0;
        local_particles[i].fx = 0.0;
        local_particles[i].fy = 0.0;
        local_particles[i].fz = 0.0;
    }
}

bool ParticleSimulator::evolve(vector<Particle>& local_particles, vector<Particle>& remote_particles) {
    #pragma omp parallel for schedule(static)
    for (size_t i = 0; i < local_particles.size(); i++) {
        for (size_t j = 0; j < remote_particles.size(); j++) {

            double rx = local_particles[i].x - remote_particles[j].x;
            double ry = local_particles[i].y - remote_particles[j].y;
            double rz = local_particles[i].z - remote_particles[j].z;

            double r = sqrt(rx * rx + ry * ry + rz * rz);
            
            if (r <= 1e-10){
                continue;
            }

            double r2 = r * r;
            double r6 = r2 * r2 * r2;
            double r12 = r6 * r6;
            double f = (1.0 / r12) - (2.0 / r6);

            double fx = f * rx / r;
            double fy = f * ry / r;
            double fz = f * rz / r;

            local_particles[i].fx += fx;
            local_particles[i].fy += fy;
            local_particles[i].fz += fz;

            #pragma omp atomic
            remote_particles[j].fx -= fx;
            #pragma omp atomic
            remote_particles[j].fy -= fy;
            #pragma omp atomic
            remote_particles[j].fz -= fz;
            }
        }
    return true;
}

bool ParticleSimulator::merge(vector<Particle>& returned_particles, vector<Particle>& local_particles) {
     #pragma omp parallel for schedule(static)
    for (size_t i = 0; i < local_particles.size(); i++) {

        local_particles[i].fx += returned_particles[i].fx;
        local_particles[i].fy += returned_particles[i].fy;
        local_particles[i].fz += returned_particles[i].fz;
    }
    return true;
}

bool ParticleSimulator::update_Properties() {
    #pragma omp parallel for schedule(static)
    for (size_t i = 0; i < local_particles.size(); i++){
        local_particles[i].ax = local_particles[i].fx / local_particles[i].mass;
        local_particles[i].ay = local_particles[i].fy / local_particles[i].mass;
        local_particles[i].az = local_particles[i].fz / local_particles[i].mass;

        local_particles[i].vx += local_particles[i].ax * 0.1;
        local_particles[i].vy += local_particles[i].ay * 0.1;
        local_particles[i].vz += local_particles[i].az * 0.1;

        local_particles[i].x += local_particles[i].vx * 0.1;
        local_particles[i].y += local_particles[i].vy * 0.1;
        local_particles[i].z += local_particles[i].vz *0.1;

        local_particles[i].fx = 0.0 ;
        local_particles[i].fy = 0.0 ;
        local_particles[i].fz = 0.0 ;
    }
}