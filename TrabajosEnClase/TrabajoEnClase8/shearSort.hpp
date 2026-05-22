
#include <vector>
#include <algorithm>
#include <iostream>
class shearSort {
    std::vector<std::vector<int>> matrix;

    public:
        void sort(std::vector<std::vector<int>>, int N); 
        void inicializar(int N);
        bool verificar( std::vector<std::vector<int>> );
    private:
    
        int N;
        

        
};