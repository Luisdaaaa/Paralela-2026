#include <fstream>

/*class export {
    public:
        static void export_to_vtk(const std::vector<Particle>& particles, int iteration) {
            std::ofstream file("particles_" + std::to_string(iteration) + ".vtk");
            file << "# vtk DataFile Version 3.0\n";
            file << "Particle data\n";
            file << "ASCII\n";
            file << "DATASET UNSTRUCTURED_GRID\n";
            file << "POINTS " << particles.size() << " float\n";
            for (const auto& p : particles) {
                file << p.x << " " << p.y << " " << p.z << "\n";
            }
            file.close();
        }
};*/