#include "programa.hpp"

bool programa::Paraview_data(string filename) {
    // Implementation for creating Paraview data
    filename="./ParaviewData/"+filename+to_string(numArchive)+".vtk"; // Usar ruta relativa
    numArchive++;
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
    cube_old.resize(N, vector<vector<double>>(N, vector<double>(N, 0.0)));
    cube_new.resize(N, vector<vector<double>>(N, vector<double>(N, 0.0)));
    for (int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            for(int k=0; k<N; k++){
                if(k == 0){
                    cube_old[i][j][k]=0.0; //if its the bottom face, we set the temperature to 0
                    cube_new[i][j][k]=0.0;
                }
                else if(i==0 ||i==N-1 || j==0 || j==N-1 || k==N-1){
                    cube_old[i][j][k]=100.0; //if we are in the border, we set the temperature to 100
                    cube_new[i][j][k]=100.0;
                }
                else{
                    cube_old[i][j][k]=(100.0+100.0+100.0+100.0+100.0+0.0)/6.0; //else, we set the temperature to the average of the 6 faces of the cube
                    cube_new[i][j][k]=0.0;

                }
            }
        }
    }
    
    return true;
}