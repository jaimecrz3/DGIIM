#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "letters_set.h"

using namespace std;

LettersSet::LettersSet() = default;

LettersSet::LettersSet(const LettersSet & other) {

    letters = other.letters;

}

bool LettersSet::insert(const pair<char, LetterInfo> &val) {

    return(letters.insert(val).second);

}

bool LettersSet::erase(const char &key) {

    return(letters.erase(key));

}

void LettersSet::clear() {

    letters.clear();

}

bool LettersSet::empty() const {

    return(letters.empty());

}

unsigned int LettersSet::size() const{

    return(letters.size());

}

int LettersSet::getScore (string word){
    int puntuacion=0;

    //cout << word.length();
    for(int i=0; i<word.length(); i++){
        map<char, LetterInfo>::iterator letra_buscada = letters.find(toupper(word.at(i)));

        if (letra_buscada != letters.end()){
            puntuacion += letra_buscada->second.score;
        }

    }

    return(puntuacion);


}

LettersSet & LettersSet::operator=(const LettersSet &cl) {

    if(&cl!=this){

        letters.clear();
        letters=cl.letters;   //Uso el operador de asignacion del map

    }

    return *this;

}

LetterInfo & LettersSet::operator[](const char &val) {

    return(letters[val]);

}

const LetterInfo & LettersSet::operator[](const char &val) const {

    return(letters.at(val));

}

ostream & operator<<(ostream & os, const LettersSet & cl){

    os << "letra" << setw(10) << "Cantidad" << setw(7) << "Puntos" << endl;

    LettersSet::iterator it;

    for(it=cl.letters.begin(); it!=cl.letters.end(); ++it){

        os << (*it).first << setw(8) << (*it).second.repetitions << setw(9) << (*it).second.score << endl;

    }

    return os;

}

istream & operator>>(istream & is, LettersSet & cl){

    string s1,s2,s3;
    is >> s1 >> s2 >> s3;

    cl.clear();
    char key;
    LetterInfo info;
    while(is){

        is >> key >> info.repetitions >> info.score;
        pair<char, LetterInfo> aux(toupper(key), info);
        cl.insert(aux);

    }

    return is;

}

LettersSet::iterator LettersSet::begin() const {
    LettersSet::iterator it;

    it = letters.begin();

    return (it);
}

LettersSet::iterator LettersSet::end() const{
    LettersSet::iterator it;

    it = letters.end();

    return (it);
}
