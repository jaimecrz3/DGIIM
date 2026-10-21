#include <iostream>
#include "letters_bag.h"
#include "letters_set.h"
#include <fstream>

ostream & operator<<(ostream & os, const vector<char> & v){
    int tamanio = v.size();

    for (int i = 0; i < tamanio; ++i){
        if(i != 0){
            os << endl;
        }
        os << v[i];
    }

    return os;

}


int main (int argc, char *argv[]){
    // Comprobar validez de la llamada
    if (argc != 2){
        cerr << "Error: Numero incorrecto de parametros.\n";

        exit (1);
    }
    // Obtener argumentos
    ifstream archivo_letras;
    LettersSet conjunto_letras;

    archivo_letras.open(argv[1]);
    if(!archivo_letras.is_open()){
        cerr << "Error al abrir " << argv[1] << endl;
        exit (1);;
    }

    //Pasamos la informacion al LettersSet
    archivo_letras >> conjunto_letras;

    // Creamos el LettersBag
    LettersBag bolsa_letras(conjunto_letras);

    vector<char> letras = bolsa_letras.extractLetters(bolsa_letras.size());

    cout << letras << endl;

    return 0;
}
