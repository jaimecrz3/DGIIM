#include "letters_bag.h"
#include <iostream>

void LettersBag::insertLetter (const char & I){
    letters.add(I);
}

char LettersBag::extractLetter (){
    return (letters.get());
}


vector<char> LettersBag::extractLetters (int num){
    vector<char> vector;
    for (int i = 0; i < num; i++){
        vector.push_back(letters.get());
    }

    return (vector);
}

void LettersBag::clear () {
    letters.clear();
}

unsigned int LettersBag::size () const {
    return(letters.size());
}

LettersBag & LettersBag::operator= (const LettersBag & other) {
    if (this != &other){
        this->clear();
        letters = other.letters;
    }

    return (*this);
}


void LettersBag::llenaBolsa (const LettersSet & lettersSet) {
    for (LettersSet::iterator it = lettersSet.begin(); it != lettersSet.end(); ++it){
        for (int j = 0; j < (*it).second.repetitions; ++j){
            letters.add((*it).first);
        }

    }


}

