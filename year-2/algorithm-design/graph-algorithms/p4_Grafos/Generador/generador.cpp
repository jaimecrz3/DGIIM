#include <iostream>
#include <vector>
#include <cstdlib>
#include <random>
#include <fstream>
#include <iomanip>

using namespace std;
/*
 * El generador de casos crea la matriz de conveniencias entre todos los invitados a sentar. Es una matriz donde todos
 * los elementos de la diagonal serán 0.
 *
 * Se pasará como parámetro el número de invitados y la cota a utilizar
 */

int main(int argc, char* argv[]) {

    if (argc != 3) {
        cerr << "Formato " << argv[0] << "<Cota a usar: {1,2,3}> <num_inivitados>" << endl;
        return -1;
    }

    int cota = atoi(argv[1]);
    int num_invitados = atoi(argv[2]);

    if (cota != 1 && cota != 2 && cota != 3) {
        cout << "ERROR: Cota a usar debe ser: 1, 2, 3" << endl;
        return -1;
    }

    if (num_invitados <= 0) {
        cout << "ERROR: el numero de invitados ha de ser un entero mayor estricto que 0" << endl;
        return -1;
    }

    ofstream fo;
    fo.open("ficheroDeSalida.txt");
    if (fo){
        // Mandar el número de cota
        fo << cota << endl;
        // Mandamos el número de invitados
        fo << num_invitados << endl;

        //Generamos las conveniencias aleatoriamente
        srandom(time(0));

        vector<vector<double>> matriz;
        vector<double> aux;

        for (int i = 0; i < num_invitados; i++) {
            aux.push_back(0);
        }
        for (int i = 0; i < num_invitados; i++) {
            matriz.push_back(aux);
        }

        int conveniencia;
        const int MAX = 1001;
        for (int i = 0; i < num_invitados; i++) {
            for (int j = 0; j < num_invitados; j++) {
                if (i != j) {
                    conveniencia = rand()%MAX;
                    matriz.at(i).at(j) = conveniencia;
                }

                fo << setw(10) << matriz.at(i).at(j) << " ";
            }

            fo << endl;
        }

        fo << endl;
        fo.close();
    }

    return 0;
}
