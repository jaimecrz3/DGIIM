#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include "dictionary.h"

using namespace std;

int main(int argc, char *argv[]){

    // Comprobar validez de la llamada
    if (argc != 3){
        cerr << "Error: Numero incorrecto de parametros.\n";

        exit (1);
    }

    // Obtener argumentos
    Dictionary dictionary;
    ifstream fichero_in;
    int longitud;

    fichero_in.open(argv[1]);
    if(!fichero_in.is_open()){
        cerr << "Error al abrir " << argv[1] << endl;
        exit (1);;
    }
    longitud = stoi(argv[2]);

    fichero_in >> dictionary;

    //Obtenemos las palabras que tienen tamaño=longitud
    vector<string> palabras= dictionary.wordsOfLength(longitud);

    //Mostrar resultado
    cout << "Palabras de longitud " << longitud << endl;

    for(int i=0; i<palabras.size(); i++){

        cout << palabras[i] << endl;

    }

    return 0;
}
