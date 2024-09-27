#include <string>
#include <iostream>
#include <vector>
#include <utility>
#include "dictionary.h"
#include "letters_set.h"
#include "letters_bag.h"
#include "solver.h"

///////////////////////////////////////////////////////////////////////////////
//                             Private functions                             //
///////////////////////////////////////////////////////////////////////////////

bool Solver::esValida(string palabra, vector<char> &available_letters) {

    int i=0;
    bool es_valida=true;

    while(i<palabra.length() && es_valida){

        vector<char>::iterator it;
        bool encontrada=false;
        //Compruebo si el caracter i-ésimo de palabra esta
        //en el vector de letras disponibles
        for(it=available_letters.begin(); it!=available_letters.end() && !encontrada; ++it){

            if(toupper(palabra.at(i))==*it){

                encontrada=true;
                available_letters.erase(it);

            }

        }

        if(!encontrada){

            es_valida=false;

        }

        i++;

    }

    return(es_valida);

}

pair<vector<string>, int> Solver::getSolutionsScore(const vector<char> &available_letters){

    vector<string> soluciones;
    int puntuacion=0;
    Dictionary::iterator it;
    for(it=diccionario.begin(); it!=diccionario.end(); ++it){

        vector<char> letras_disponibles=available_letters;
        string palabra=(*it);

        //Comprobamos que la palabra es correcta con las letras dadas
        if(esValida(palabra, letras_disponibles)){

            //Si la puntuacion es mayor se borran las soluciones anteriores y se añade la nueva
            //Si es igual simplemtente añadimos la palabra al conjunto de soluciones
            if(letras.getScore(palabra)>puntuacion){

                soluciones.clear();
                soluciones.push_back(palabra);
                puntuacion=letras.getScore(palabra);

            }else if(letras.getScore(palabra)==puntuacion){

                soluciones.push_back(palabra);

            }


        }

    }

    return(pair<vector<string>, int>(soluciones,puntuacion));


}

pair<vector<string>, int> Solver::getSolutionsLength(const vector<char> &available_letters){

    vector<string> soluciones;
    int longitud=0;
    Dictionary::iterator it;
    for(it=diccionario.begin(); it!=diccionario.end(); ++it){

        vector<char> letras_disponibles=available_letters;
        string palabra=(*it);

        //Comprobamos que la palabra es correcta con las letras dadas
        if(esValida(palabra, letras_disponibles)){

            //Si la puntuacion es mayor se borran las soluciones anteriores y se añade la nueva
            //Si es igual simplemtente añadimos la palabra al conjunto de soluciones
            if(palabra.length()>longitud){

                soluciones.clear();
                soluciones.push_back(palabra);
                longitud=palabra.length();

            }else if(palabra.length()==longitud){

                soluciones.push_back(palabra);

            }


        }

    }

    return(pair<vector<string>, int>(soluciones,longitud));

}

pair<vector<string>, int> Solver::getSolutionsScoreEficiente(const vector<char> &available_letters){

    vector<string> soluciones;
    int puntuacion=0;
    Dictionary::possible_words_iterator it;
    for(it=diccionario.possible_words_begin(available_letters); it!=diccionario.possible_words_end(); ++it){

        vector<char> letras_disponibles=available_letters;
        string palabra=(*it);


        //Si la puntuacion es mayor se borran las soluciones anteriores y se añade la nueva
        //Si es igual simplemtente añadimos la palabra al conjunto de soluciones
        if (letras.getScore(palabra) > puntuacion) {

            soluciones.clear();
            soluciones.push_back(palabra);
            puntuacion = letras.getScore(palabra);

        } else if(letras.getScore(palabra)==puntuacion) {

            soluciones.push_back(palabra);

        }

    }

    return(pair<vector<string>, int> (soluciones, puntuacion));


}

pair<vector<string>, int> Solver::getSolutionsLengthEficiente(const vector<char> &available_letters){

    vector<string> soluciones;
    int longitud = 0;

    for(Dictionary::possible_words_iterator it = this->diccionario.possible_words_begin(available_letters); it != diccionario.possible_words_end(); ++it){
        vector<char> letras_disponibles=available_letters;
        string palabra=(*it);

        //Si la puntuacion es mayor se borran las soluciones anteriores y se añade la nueva
        //Si es igual simplemtente añadimos la palabra al conjunto de soluciones
        if(palabra.length()>longitud){
            soluciones.clear();
            soluciones.push_back(palabra);
            longitud=palabra.length();

        }else if(palabra.length()==longitud){

            soluciones.push_back(palabra);

        }

    }

    return(pair<vector<string>, int>(soluciones,longitud));

}


///////////////////////////////////////////////////////////////////////////////
//                             Public functions                             //
///////////////////////////////////////////////////////////////////////////////

Solver::Solver(const Solver &other) {

    diccionario=other.diccionario;
    letras=other.letras;

}

Solver::Solver(const Dictionary &dic, const LettersSet &letters_set) {

    diccionario=dic;
    letras=letters_set;

}

pair<vector<string>, int> Solver::getSolutions(const vector<char> &available_letters, bool score_game) {

   pair <vector<string>, int> solucion;
   if(score_game){

       solucion= getSolutionsScore(available_letters);

   }else{

       solucion= getSolutionsLength(available_letters);

   }

   return(solucion);


}

pair<vector<string>, int> Solver::getSolutionsEficiente(const vector<char> &available_letters, bool score_game) {

    pair <vector<string>, int> solucion;
    if(score_game){

        solucion = getSolutionsScoreEficiente(available_letters);

    }else{

        solucion = getSolutionsLengthEficiente(available_letters);

    }

    return(solucion);


}
