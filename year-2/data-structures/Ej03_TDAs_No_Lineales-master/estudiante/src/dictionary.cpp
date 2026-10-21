#include "dictionary.h"
#include <iostream>

Dictionary::Dictionary(const Dictionary &other) {

    words=other.words;

}

bool Dictionary::exists(const std::string &val) const {

    return(words.count(val));

}

bool Dictionary::insert(const std::string &val) {

    return(words.insert(val).second);

}

bool Dictionary::erase(const std::string &val) {

    return(words.erase(val));

}

void Dictionary::clear() {

    words.clear();

}

unsigned int Dictionary::size() const {

    return(words.size());

}

bool Dictionary::empty() const {

    return(words.empty());

}

int Dictionary::getOcurrences(const char c) {

    Dictionary::iterator it;
    int ocurrencias=0;

    //Recorremos el diccionario
    for(it = words.begin(); it != words.end(); ++it){

        string w = (*it);
        //Recorremos la palabra
        for(int i = 0; i < w.length(); ++i){
            if(toupper(w.at(i)) == c){
                ocurrencias++;

            }

        }


    }

    return(ocurrencias);

}

int Dictionary::getTotalLetters() {

    Dictionary::iterator it;
    int ocurrencias = 0;

    //Recorremos el diccionario
    for(it=words.begin(); it!=words.end(); ++it){

        string w = (*it);

        ocurrencias += w.length();

    }

    return(ocurrencias);

}

vector<string> Dictionary::wordsOfLength(int length) {

    Dictionary::iterator it;
    vector<string> palabras;

    //Recorremos el diccionario
    for(it=words.begin(); it!=words.end(); ++it){

        string w=*it;

        if(w.length() == length){

            palabras.push_back(w);

        }

    }

    return(palabras);

}

ostream & operator<<(ostream & os, const Dictionary & cl){

    Dictionary::iterator it;

    for(it=cl.begin(); it != cl.end(); ++it){

        os << *it << endl;

    }

    return os;

}

istream & operator>>(istream & is, Dictionary & cl){

    cl.clear();
    string w;
    while(is){

        is >> w;
        cl.insert(w);

    }

    return is;

}



