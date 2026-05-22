#include "menu.hpp"

void menu::display_menu() {
    //Menu del programa
    programa Program;
    cout << "Menu del programa:" << endl;
    cout << "1. Calcular serial" << endl;
    cout << "2. Calcular paralelo" << endl;
    cout << "3. Ver datos" << endl;
    cout << "4. Salir" << endl;
    cout << "Ingrese una opcion: ";
    cin >> option;
    if(option == 1){
        Program.initialize_cube();
        Program.calculate_steps_serial();
    }
    else if(option == 2){
        Program.initialize_cube();
        Program.calculate_steps_parallel();
    }
    else if(option == 3){
        string filename;
        cout << "Ingrese el nombre del archivo: ";
        cin >> filename;
        Program.Paraview_data(filename);
    }
    else if(option == 4){
        cout << "Saliendo del programa..." << endl;
    }
    else{
        cout << "Opcion no valida" << endl;
    }
}