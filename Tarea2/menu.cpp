#include "menu.hpp"

void menu::display_menu() {
    while(true){
        cout << "Menu del programa:" << endl;
        cout << "1. Calcular serial" << endl;
        cout << "2. Calcular paralelo" << endl;
        cout << "Cualquier otro para salir " << endl;
        cin >> option;
        switch (option){
        case 1 :
            prog.initialize_cube();
            prog.calculate_steps_serial();
            break;
        case 2:
            prog.initialize_cube();
            prog.calculate_steps_parallel();
            break;
        default:
            return;
        }

    }
    programa Program;
    
}