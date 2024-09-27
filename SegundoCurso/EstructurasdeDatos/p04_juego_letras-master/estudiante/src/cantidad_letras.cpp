#include <fstream>
#include <iostream>

#include "dictionary.h"
#include "letters_set.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "Los parametros son: " << endl;
        cout << "1.- El fichero con el diccionario" << endl;
        cout << "2.- El fichero con las letras" << endl;
        return 1;
    }
    // Obtener argumentos
    ifstream f_diccionario(argv[1]);

    if (!f_diccionario) {
        cout << "No puedo abrir el fichero " << argv[1] << endl;
        return 0;
    }

    ifstream archivo_letras(argv[2]);

    if(!archivo_letras.is_open()){
        cerr << "Error al abrir " << argv[1] << endl;
        exit (1);;
    }

    // Pasamos la informacion
    Dictionary diccionario;
    f_diccionario >> diccionario;

    LettersSet conjunto_letras;
    archivo_letras >> conjunto_letras;

    cout << "Letra\tUsos\tOcurrencias" << endl;
    for (LettersSet::iterator it = conjunto_letras.begin(); it != conjunto_letras.end(); ++it){
        int ocurrencias = diccionario.getOccurrences(tolower((*it).first));
        int usos = diccionario.getTotalUsages(tolower((*it).first));;
        cout << (*it).first << "\t" << usos << "\t" << ocurrencias << endl;
    }

}