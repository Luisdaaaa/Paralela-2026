#include <iostream>
#include <vector>
#include <random>
class matriz {
    private:
        std::vector<std::vector<int>> matriz;
    public:

        void llenarMatriz();
        void imprimirMatriz();

        std::vector<std::vector<int>>& getMatriz() {
            return matriz;
        }
};