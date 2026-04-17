#include <omp.h>
#include <stdio.h>

// Número de pasos (subintervalos) para la aproximación numérica de pi
static long num_steps = 100000;
// Ancho de cada subintervalo
double step;

int main() {
    double time= omp_get_wtime(); // Captura el tiempo inicial en segundos

    omp_set_num_threads(8); // Establece el número de hilos a usar en la región paralela

    printf("Número de hilos: %d\n", omp_get_num_threads()); // Imprime el número de hilos que se usarán en la región paralela
    int i;
    // x: punto de evaluación, pi: valor aproximado de pi, sum: acumulador de la sumatoria
    double x, pi, sum = 0.0;
    
    // Calcula el ancho de cada paso dividiendo la unidad entre el número de pasos
    step = 1.0 / (double)num_steps;

    // Captura el tiempo inicial en segundos para medir el tiempo de ejecución
    double tdata = omp_get_wtime();
    
    // Loop que itera sobre cada subintervalo
    for (i = 0; i < num_steps; i++) {
        #pragma omp parallel
        // Calcula el punto medio del i-ésimo subintervalo
        x = (i + 0.5) * step;

        // Suma el valor de 4/(1+x²) para aproximar el área bajo la curva (integral de 0 a 1)
        sum = sum + 4.0 / (1.0 + x * x);
    }
    
    // Multiplica la suma por el ancho del paso para obtener la aproximación de pi
    pi = step * sum;

    // Calcula el tiempo transcurrido desde el inicio
    tdata = omp_get_wtime() - tdata;
    
    // Muestra el valor aproximado de pi y el tiempo de ejecución en segundos
    printf("pi = %f in %f secs\n", pi, tdata);
    
    return 0;
}
