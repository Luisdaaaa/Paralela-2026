#define N 100
#define NUM_STEPS 1000
#include <iostream>
#include <vector>

using namespace std;

class programa{
    public:
        bool initialize_cube();
        bool print_cube();
        bool Paraview_data(int n, int steps, string filename);
        bool calculate_steps_serial();
        bool calculate_steps_parallel();
        //se debe calcular los tiempos para serial y paralelo
    private:
        vector<vector<vector<double>>> matrix_old;
        vector<vector<vector<double>>> matrix_new;
};

