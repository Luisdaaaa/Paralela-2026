#include "particle_sim.hpp"

ParticleSimulator::ParticleSimulator(int N, int iterations, int bandera_imp, int INI, int rank, int size) : rank(rank), size(size) {
    this->N = N;
    this->iterations = iterations;
    this->bandera_imp = bandera_imp;
    this->INI = INI;
    this->size = size;
    this->rank = rank;
}

void ParticleSimulator::initialize_particles( int rank) {
    srand(rank * 12345);
    int vector_length = this->N; 
    local_particles.resize(vector_length);
    remote_particles.resize(vector_length);
    for (int i = 0; i < vector_length; i++) {
        local_particles[i].x = static_cast<double>(rand()) / 500.0;
        local_particles[i].y = static_cast<double>(rand()) / 500.0;
        local_particles[i].z = static_cast<double>(rand()) / 500.0;
        local_particles[i].vx = static_cast<double>(rand()) / 500.0;
        local_particles[i].vy = static_cast<double>(rand()) / 500.0;
        local_particles[i].vz = static_cast<double>(rand()) / 500.0;
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
    return true;
}

void ParticleSimulator::execute_simulation() {
    int destino = (rank + 1) % size;          
    int origen  = (rank - 1 + size) % size;
    MPI_Datatype MPI_PARTICLE = this->particle_Create();

    std::vector<Particle> total_particles;
    if (rank == 0) {
        total_particles.resize(N * size); 
    }
    

    std::vector<Particle> remotas_entrantes(N);
    std::vector<Particle> locales_devueltas(N);

    for (int iter = 0; iter < iterations; iter++) {
        if (rank == 0) {
            std::cout << "Starting iteration " << iter + 1 << std::endl;
        }
        MPI_Sendrecv(
            local_particles.data(),  N, MPI_PARTICLE, destino, 0,
            remote_particles.data(), N, MPI_PARTICLE, origen,  0,
            MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for(int p = 0; p < (size - 1)/2; p++) {
            
            evolve(local_particles, remote_particles);

            if(p < (size - 1)/2 - 1) {
                MPI_Sendrecv(remote_particles.data(),  N, MPI_PARTICLE, destino, 0,
                remotas_entrantes.data(), N, MPI_PARTICLE, origen,  0,
                MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                
                remote_particles.swap(remotas_entrantes);
            }
        }
        
        int owner = (rank - (size - 1)/2 + size) % size;
        int my_particles = (rank + (size - 1)/2) % size;

        MPI_Sendrecv(
            remote_particles.data(),  N, MPI_PARTICLE, owner, 0,
            locales_devueltas.data(), N, MPI_PARTICLE, my_particles,0,
            MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        merge(locales_devueltas, local_particles);
        evolve(local_particles, local_particles);
        update_Properties();

        if (bandera_imp && iter % 50 == 0) {
            MPI_Gather(local_particles.data(), N, MPI_PARTICLE, 
            total_particles.data(), N, MPI_PARTICLE, 0, MPI_COMM_WORLD);
            
            if (rank == 0) {
                std::cout << "Exporting data for iteration " << iter  << std::endl;
                Export::export_to_vtk(total_particles, iter );
                std::cout << "Export completed for iteration " << iter  << std::endl;
            }
        }
    }
    MPI_Type_free(&MPI_PARTICLE);
}

MPI_Datatype ParticleSimulator::particle_Create() {
    MPI_Datatype MPI_PARTICLE;

    int count = 5; 
    int blocklengths[5] = {3, 3, 3, 3, 1}; 

    MPI_Datatype types[5] = {MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE};

    
    MPI_Aint displacements[5];
    displacements[0] = offsetof(Particle, x);
    displacements[1] = offsetof(Particle, vx);
    displacements[2] = offsetof(Particle, fx);
    displacements[3] = offsetof(Particle, ax);
    displacements[4] = offsetof(Particle, mass);

    
    MPI_Type_create_struct(count, blocklengths, displacements, types, &MPI_PARTICLE);
    MPI_Type_commit(&MPI_PARTICLE);

    return MPI_PARTICLE;
}