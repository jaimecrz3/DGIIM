#include <fstream>
#include <iostream>
#include <cstring>

#include "dictionary.h"
#include "letters_set.h"
#include "solver.h"
#include "letters_bag.h"

using namespace std;

int main(int argc, char *argv[]) {
    // Inicializamos el generador de números aleatorios
    srand(time(nullptr));

    if (argc != 5) {
        cout << "Los parametros son: " << endl;
        cout << "1.- El fichero con el diccionario" << endl;
        cout << "2.- El fichero con las letras" << endl;
        cout << "3.- Modo de juego" << endl;
        cout << "4.- Cantidad de letras" << endl;
        return 1;
    }
    // Obtener argumentos
    ifstream f_diccionario(argv[1]);

    if (!f_diccionario) {
        cerr << "Error al abrir " << argv[1] << endl;
        exit (1);;
    }

    ifstream archivo_letras(argv[2]);

    if(!archivo_letras.is_open()){
        cerr << "Error al abrir " << argv[2] << endl;
        exit (1);;
    }

    // Pasamos la informacion
    Dictionary diccionario;
    f_diccionario >> diccionario;

    LettersSet conjunto_letras;
    archivo_letras >> conjunto_letras;

    bool modo_juego;
    if(strcmp(argv[3], "L")==0){

        modo_juego=false;

    }else if(strcmp(argv[3], "P")==0){
        
        modo_juego=true;

    } else{

        cout << "Modos de juego, L=Longitud, P=Puntuacion" << endl;
        return 1;

    }

    int cantidad_letras=stoi(argv[4]);

    //Obtenemos la solucion

    Solver solver(diccionario, conjunto_letras);
    LettersBag bolsa_letras(conjunto_letras);

    const vector<char> letras_partida=bolsa_letras.extractLetters(cantidad_letras);

    pair<vector<string>, int> solucion=solver.getSolutions(letras_partida,modo_juego);

    //Mostramos la solucion
    cout<<"LETRAS DISPONIBLES:"<<endl;

    for(int i=0; i<letras_partida.size();i++){

        cout << letras_partida[i] <<  "\t" ;

    }

    cout<< endl <<"SOLUCIONES:"<<endl;
    for(int i=0; i<solucion.first.size();i++){

        cout << solucion.first[i] << "\n" ;

    }
    cout<<"PUNTUACION:"<<endl;
    cout<<solucion.second;



}
