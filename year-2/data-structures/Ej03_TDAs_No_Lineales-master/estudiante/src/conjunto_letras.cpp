#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include "letters_set.h"

using namespace std;

int main(int argc, char *argv[]){

    // Comprobar validez de la llamada
    if (argc != 3){
        cerr << "Error: Numero incorrecto de parametros.\n";

        exit (1);
    }

    // Obtener argumentos
    LettersSet letterset;
    ifstream fichero_in;
    string word;

    fichero_in.open(argv[1]);
    if(!fichero_in.is_open()){
        cerr << "Error al abrir " << argv[1] << endl;
        exit (1);;
    }
    word = argv[2];

    fichero_in >> letterset;

    //Obtenemos la puntuacion
    int puntuacion = letterset.getScore(word);

    //Mostrar resultado
    //cout << "La puntuacion de la palabra " << word << " es: " << puntuacion;
    cout << puntuacion << endl;

    return 0;
}
