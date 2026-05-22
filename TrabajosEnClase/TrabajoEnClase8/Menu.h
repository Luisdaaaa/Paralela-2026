#include <fstream>
#include "shearSort.hpp"


class Menu {
    std::ifstream file {"Matrices.txt", std::ios::in};
    int opciones;
    int N;
    shearSort shear;
public:

    Menu();
    ~Menu() = default;

    std::vector<std::vector<int>>iniciar();
};
