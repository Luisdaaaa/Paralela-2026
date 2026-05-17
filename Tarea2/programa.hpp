#define N 300
#define NUM_STEPS 1000
#include <iostream>
#include <fstream>
#include <vector>


using namespace std;

class programa{
    public:
        bool initialize_cube();
        bool Paraview_data();
        bool calculate_steps_serial();
        bool calculate_steps_parallel();
        void imprimir();
        //se debe calcular los tiempos para serial y paralelo
    private:
        vector<vector<vector<double>>> cube_old;
        vector<vector<vector<double>>> cube_new;
        int numArchive=0;
        int N_TOTAL=N*N*N;
        string  filename;
};

