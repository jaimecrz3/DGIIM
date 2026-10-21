#include <iostream>
#include <chrono>
#include <vector>
#include <stdlib.h>
#include <image.h>
#include <fstream>

using namespace std;


void chrono_experiment(int rows, int cols, int repetitions) {
    // Inicializamos el experimento antes del bucle de conteo de tiempo para no afectar al análisis
    Image image(rows, cols, 0);


    // Medimos el tiempo antes de iniciar el expermiento
    chrono::high_resolution_clock::time_point start_time = chrono::high_resolution_clock::now();

    // Realizamos el experimento tantas veces como indique repetitions, para tener cierta robustez
    // en el resultado (no todas las ejecuciones tardan lo mismo)
    for (int k = 0; k < repetitions; ++k){
        image.ShuffleRows();
    }

    // Medimos el tiempo al finalizar el experimento
    chrono::high_resolution_clock::time_point finish_time = chrono::high_resolution_clock::now();

    // Calculamos la diferencia entre el inicio y el final
    chrono::duration<double> total_duration = chrono::duration_cast<chrono::duration<double>>(finish_time - start_time);

    // Dividimos el tiempo total entre el número de repeticiones para obtener el tiempo medio por ejecución
    cout << rows << "\t" << cols << "\t" << total_duration.count()/repetitions << endl;

    ofstream file;
    file.open("../analisis.txt", ios::app);

    file << rows << "\t" << cols << "\t" << total_duration.count()/repetitions << endl;
    file.close();
}
int main() {
    for (int i = 100; i <= 3000; i += 100){
        chrono_experiment(i,100, 30);
    }
    return 0;
}
