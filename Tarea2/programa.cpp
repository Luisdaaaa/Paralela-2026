#include "programa.hpp"
#include <stdio.h>
#include <omp.h>

bool programa::Paraview_data() {
    // Implementation for creating Paraview data
    filename="./ParaviewData/paraview"+to_string(numArchive)+".vtk"; // Usar ruta relativa
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
    }else{
        cout<<"El archivo no se pudo abrir"<<endl;
        return false;
    }
    return true;
}
bool programa::calculate_steps_parallel() {
    omp_set_num_threads(4);
    double tiempo_inicio = omp_get_wtime();
    for(int p = 0; p< NUM_STEPS; p++){
        #pragma omp parallel for
        for(int i = 1; i< N-1; i++){
            for(int j = 1; j< N-1; j++){
                for(int k = 1; k < N-1; k++){
                    cube_new[i][j][k] = (cube_old[i+1][j][k] + cube_old[i-1][j][k] +
                    cube_old[i][j+1][k] + cube_old[i][j-1][k] +
                    cube_old[i][j][k+1] + cube_old[i][j][k-1]) / 6.0;
                }
            }
        }
        std::swap(cube_old, cube_new);
        if(p%50 == 0){
            Paraview_data();
        }
    }
    numArchive = 0;
    printf("En paralelo\n");
    double tiempo_final = omp_get_wtime();
    double tiempo_total = tiempo_final - tiempo_inicio;
    printf("Tiempo de ejecución: %f segundos\n", tiempo_total);
    return true;
}

bool programa::calculate_steps_serial() {
    double tiempo_inicio = omp_get_wtime();
    for(int p = 0; p< NUM_STEPS; p++){
        for(int i = 1; i< N-1; i++){
            for(int j = 1; j< N-1; j++){
                for(int k = 1; k < N-1; k++){
                    cube_new[i][j][k] = (cube_old[i+1][j][k] + cube_old[i-1][j][k] +
                    cube_old[i][j+1][k] + cube_old[i][j-1][k] +
                    cube_old[i][j][k+1] + cube_old[i][j][k-1]) / 6.0;
                }
            }
        }
        
        std::swap(cube_old, cube_new);
        if(p%50 == 0){
            Paraview_data();
        }
    }
    numArchive = 0;
    printf("En serial\n");
    double tiempo_final = omp_get_wtime();
    double tiempo_total = tiempo_final - tiempo_inicio;
    printf("Tiempo de ejecución: %f segundos\n", tiempo_total);
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


void programa::imprimir(){
    /*
    std::cout << "Cara Frontral" <<std::endl;
    for(int i =0; i < N; i++ ){
        for (int j = 0; j < N; j++){
            std::cout<< cube_new[i][j][0] << " ";
        }
        std::cout << std::endl;
    } 
    std::cout << "Cara Inferior" <<std::endl;
    for(int i =0; i < N; i++ ){
        for (int k = 0; k < N; k++){
            std::cout<< cube_new[i][0][k] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Cara Superior" <<std::endl;
    for(int i =0; i < N; i++ ){
        for (int k = 0; k < N; k++){
            std::cout<< cube_new[i][N-1][k] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Cara Izquierda" <<std::endl;
    for (int j = 0; j < N; j++) {
        for (int k = 0; k < N; k++) {
            std::cout<<cube_new[0][j][k] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Cara Derecha" <<std::endl;
    for (int j = 0; j < N; j++) {
        for (int k = 0; k < N; k++) {
            std::cout<<cube_new[N-1][j][k] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Cara Posterior" <<std::endl; 
    for(int i =0; i < N; i++ ){
        for (int j = 0; j < N; j++){
            std::cout<<cube_new[i][j][N-1] << " ";
        }
        std::cout << std::endl;
    }
    */
    std::cout << "Interior" <<std::endl; 
    for(int i = 1; i< N-1; i++){
            for(int j = 1; j< N-1; j++){
                for(int k = 1; k < N-1; k++){
                    std::cout<<cube_new[i][j][k] << " ";
                }
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
}