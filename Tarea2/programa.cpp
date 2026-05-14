#include "programa.hpp"

bool programa::Paraview_data(string filename) {
    int N_TOTAL=N*N*N;
    // Implementation for creating Paraview data
    filename="./ParaviewData/"+filename+".vtk"; // Usar ruta relativa
    std::ofstream paraview_archive(filename);
    if (paraview_archive.is_open()){
        //Header
        paraview_archive<<"# vtk DataFile Version 3.0\n";
        paraview_archive<<"Distribucion de Calor 3D\n";
        paraview_archive<<"ASCII\n";
        paraview_archive<<"DATASET STRUCTURED_POINTS\n";
        paraview_archive<<"DIMENSIONS "<<N<<" "<<N<<" "<<N<<"\n";
        paraview_archive<<"ORIGIN 0.0 0.0 0.0\n";
        paraview_archive<<"SPACING 1.0 1.0 1.0\n";
        paraview_archive<<"POINT_DATA "<<N_TOTAL<<"\n";
        paraview_archive<<"SCALARS Temperature double\n";
        paraview_archive<<"LOOKUP_TABLE default\n";
        
        //Temperatures
        for (int k=0; k<N; k++){
            for(int j=0; j<N; j++){
                for(int i=0; i<N; i++){
                    paraview_archive<<cube_old[i][j][k]<<"\n";
                }
            }
        }
        paraview_archive.close();
        cout<<"Archivo guardado con exito"<<endl;
    }
    else{
        cout<<"El archivo no se pudo abrir"<<endl;
        return false;
    }
    return true;
}

bool programa::calculate_steps_serial() {
    // Implementation for calculating steps in serial
    return true;
}

bool programa::calculate_steps_parallel() {
    // Implementation for calculating steps in parallel
    return true;
}
bool programa::initialize_cube() {
    // Implementation for initializing the cube
    return true;
}