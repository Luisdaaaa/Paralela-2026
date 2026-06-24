#include <fstream>
#include <vector>
#include <string>
#include "particle.hpp"
class Export { 
public:
    static void export_to_vtk(const std::vector<Particle>& particles, int iteration) {
        std::ofstream file("particles_" + std::to_string(iteration) + ".vtk");
        
        file << "# vtk DataFile Version 3.0\n";
        file << "Particle data\n";
        file << "ASCII\n";
        file << "DATASET POLYDATA\n"; 
        file << "POINTS " << particles.size() << " float\n";
        
        for (const auto& p : particles) {
            file << p.x << " " << p.y << " " << p.z << "\n";
        }
        
        file << "VERTICES " << particles.size() << " " << 2 * particles.size() << "\n";
        for (size_t i = 0; i < particles.size(); ++i) {
            file << "1 " << i << "\n";
        }
        
        file.close();
    }
};