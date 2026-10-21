#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include "dictionary.h"
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
    Dictionary dictionary;
    ifstream fichero_in1;

    fichero_in1.open(argv[1]);
    if(!fichero_in1.is_open()){
        cerr << "Error al abrir " << argv[1] << endl;
        exit (1);;
    }

    fichero_in1 >> dictionary;

    ifstream fichero_in2;

    fichero_in2.open(argv[2]);
    if(!fichero_in2.is_open()){
        cerr << "Error al abrir " << argv[1] << endl;
        exit (1);;
    }

    fichero_in2 >> letterset;


    //Calculamos y mostramos los resultados
    cout << "Letra" << "\t" << "FAbs." << "\t" << "Frel." << endl;

    int total_letras = dictionary.getTotalLetters();

    for (LettersSet::iterator it = letterset.begin(); it != letterset.end(); ++it){
        char letra = (*it).first;
        int num_ocurrencias = dictionary.getOcurrences(letra);
        double frec_relativa = num_ocurrencias/(total_letras*1.0);

        cout << letra << "\t" << num_ocurrencias << "\t" << frec_relativa << endl;
    }


    return 0;
}

