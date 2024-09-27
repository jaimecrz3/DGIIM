//
// Created by fluque1995 on 8/9/21.
//

#include <string>
#include <vector>
#include <utility>
#include <set>
#include "dictionary.h"
#include <cmath>

///////////////////////////////////////////////////////////////////////////////
//                             Private functions                             //
///////////////////////////////////////////////////////////////////////////////

Dictionary::node Dictionary::findLowerBoundChildNode(char character, Dictionary::node current) {
  node prev_child, curr_child = current.left_child();

  for (; !curr_child.is_null() && (*curr_child).character <= character; curr_child = curr_child.right_sibling()){
    prev_child = curr_child;
    if ((*curr_child).character == character) {
      return curr_child;
    }
  }
  if (!prev_child.is_null()) {
    return prev_child;
  }
  return current;
}

Dictionary::node Dictionary::insertCharacter(char character, Dictionary::node current) {
  node insertion_position = findLowerBoundChildNode(character, current);
  if (insertion_position == current){
    this->words.insert_left_child(current, char_info(character));
    return insertion_position.left_child();
  } else if ((*insertion_position).character != character){
    this->words.insert_right_sibling(insertion_position, char_info(character));
    return insertion_position.right_sibling();
  } else {
    return insertion_position;
  }
}

int Dictionary::getOccurrences(node curr_node, char c){
    int total = 0;

    if ((*curr_node).character == c) {
        ++total;
    }

    if (!curr_node.left_child().is_null()){
        total += getOccurrences(curr_node.left_child(), c);
    }

    if (!curr_node.right_sibling().is_null()){
        total += getOccurrences(curr_node.right_sibling(), c);
    }

    return (total);
}

std::pair<int, int> Dictionary::getTotalUsages(node curr_node, char c){
    int num_usos = 0;
    int num_palabras = 0;

    pair<int, int> aux;

    if (!curr_node.left_child().is_null()){
        aux = getTotalUsages(curr_node.left_child(), c);

        num_usos += aux.first;
        num_palabras += aux.second;

        if ((*curr_node).character == c){
            num_usos += aux.second;
        }
    }

    if (!curr_node.right_sibling().is_null()){
        aux = getTotalUsages(curr_node.right_sibling(), c);

        num_usos += aux.first;
        num_palabras += aux.second;
    }

    if ((*curr_node).valid_word) {
        ++num_palabras;
        if ((*curr_node).character == c){
            ++num_usos;
        }
    }

    pair<int,int> resultado(num_usos, num_palabras);

    return (resultado);

}

///////////////////////////////////////////////////////////////////////////////
//                              Public functions                             //
///////////////////////////////////////////////////////////////////////////////

Dictionary::Dictionary() {
  this->words.set_root(char_info());
  this->total_words = 0;
}

Dictionary::Dictionary(const Dictionary &other) {
  this->words = other.words;
  this->total_words = other.total_words;
}

Dictionary::~Dictionary() {
  this->words.clear();
}

bool Dictionary::exists(const std::string & word) {
  node current = this->words.get_root();
  for (int i = 0; i < word.size(); ++i){
    current = this->findLowerBoundChildNode(word[i], current);
    if ((*current).character != word[i]) {
      return false;
    }
  }

  return (*current).valid_word;
}

bool Dictionary::insert(const std::string &word) {
  node current = this->words.get_root();
  for (int i = 0; i < word.size(); ++i){
    current = this->insertCharacter(word[i], current);
  }

  if (!(*current).valid_word) {
    (*current).valid_word = true;
    this->total_words++;
    return true;
  }

  return false;
}

bool Dictionary::erase(const std::string & val){
  node current = this->words.get_root();
  for (int i = 0; i < val.size(); ++i){
    current = this->findLowerBoundChildNode(val[i], current);
    if ((*current).character != val[i]) {
      return false;
    }
  }
  if ((*current).valid_word){
    (*current).valid_word = false;
    this->total_words--;
    return true;
  }
  return false;
}

Dictionary &Dictionary::operator=(const Dictionary &dic){
  if (this != &dic){
    this->words.clear();
    this->words = dic.words;
    this->total_words = dic.total_words;
  }
  return *this;
}

///////////////////////////////////////////////////////////////////////////////
//                               I/O overload                                //
///////////////////////////////////////////////////////////////////////////////

/*std::ostream& operator<<(std::ostream &os, const Dictionary &dict){
  for(Dictionary::iterator it = dict.begin();it != dict.end(); ++it){
    os << *it << std::endl;
  }
  return os;
}*/

std::istream& operator>>(std::istream &is, Dictionary &dict){
  std::string curr_word;
  while (getline(is, curr_word)){
    dict.insert(curr_word);
  }
  return is;
}

///////////////////////////////////////////////////////////////////////////////
//                            Recursive counters                             //
///////////////////////////////////////////////////////////////////////////////

int Dictionary::getOccurrences(const char c){
    return (this->getOccurrences(words.get_root(), c));
}

int Dictionary::getTotalUsages(const char c){
    return (this->getTotalUsages(words.get_root(), c).first);
}

///////////////////////////////////////////////////////////////////////////////
//                                 Iterator                                  //
///////////////////////////////////////////////////////////////////////////////

Dictionary::iterator::iterator(){
    tree<char_info>::const_preorder_iterator otro;

    this->iter = otro;
    this->curr_word = "";
}

Dictionary::iterator::iterator(tree<char_info>::const_preorder_iterator iter) {

    this->iter=iter;

}

Dictionary::iterator::iterator (const Dictionary::iterator & otro){
    this->iter = otro.iter;
    this->curr_word = otro.curr_word;

}

std::string Dictionary::iterator::operator*() {

    return(curr_word);

}

Dictionary::iterator &Dictionary::iterator::operator++() {
    do {
        int l = iter.get_level();
        ++iter;

        if (iter.get_level() != 0){
            if(this->iter.get_level()>l){

                curr_word.push_back(iter.operator*().character);

            } else if(this->iter.get_level()==l){

                curr_word.pop_back();
                curr_word.push_back((*iter).character);

            } else {

                int n = l - this->iter.get_level();
                curr_word = curr_word.substr(0, curr_word.length()-n-1);
                curr_word.push_back((*iter).character);
            }

        }

    } while(iter.get_level() != 0 && !((*iter).valid_word));

    return *this;

}

bool Dictionary::iterator::operator==(const iterator &other) {

    return (this->iter==other.iter);

}

bool Dictionary::iterator::operator!=(const iterator &other) {

    return (this->iter!=other.iter);

}

Dictionary::iterator Dictionary::begin() const {
    Dictionary::iterator it(words.cbegin_preorder());
    ++it;

    return (it);
}

Dictionary::iterator Dictionary::end() const {
    Dictionary::iterator it(words.cend_preorder());

    return (it);
}

///////////////////////////////////////////////////////////////////////////////
//                            Letters Iterator                               //
///////////////////////////////////////////////////////////////////////////////


Dictionary::possible_words_iterator Dictionary::possible_words_begin(vector<char> available_characters) const {
    Dictionary::possible_words_iterator iterador(this->words.get_root(), available_characters);

    ++iterador;

    return (iterador);
}

Dictionary::possible_words_iterator Dictionary::possible_words_end() const {
    return (Dictionary::possible_words_iterator (node(), vector<char>()));
}

Dictionary::possible_words_iterator::possible_words_iterator() {
    level = 0;
    current_word = "";
}

Dictionary::possible_words_iterator::possible_words_iterator(node current_node, vector<char> available_letters){
    this->current_node = current_node;
    level = 0;
    this->current_word = "";

    for (int i = 0; i != available_letters.size(); ++i){
        this->available_letters.insert(tolower(available_letters[i]));
    }

}

Dictionary::possible_words_iterator::possible_words_iterator(const possible_words_iterator &other) : level(0) {
    if (&other != this){
        (*this) = other;
    }
}

Dictionary::possible_words_iterator &Dictionary::possible_words_iterator::operator=(const Dictionary::possible_words_iterator &other) {
    this->current_word = other.current_word;
    this->current_node = other.current_node;
    this->level = other.level;
    this->available_letters = other.available_letters;

    return (*this);
}

bool Dictionary::possible_words_iterator::operator==(const Dictionary::possible_words_iterator &other) const {
    bool son_iguales = false;

    if (is_equal(*this, other)){
        son_iguales = true;
    }

    return (son_iguales);
}

bool Dictionary::possible_words_iterator::operator!=(const Dictionary::possible_words_iterator &other) const {
    bool son_distintos = true;

    if (is_equal(*this, other)){
        son_distintos = false;
    }

    return (son_distintos);
}

Dictionary::possible_words_iterator &Dictionary::possible_words_iterator::operator++() {
    do{
        bool buscar_en_padres = false;

        node aux;

        // BUSCAR EN HIJOS
        if (!current_node.left_child().is_null()){
            aux = find_in_childs(current_node);

            if (!aux.is_null()){ // ENCONTRADO HIJO VALIDO
                current_node = aux;
                ++level;
                current_word.push_back(current_node.operator*().character);
                available_letters.erase(available_letters.find(current_node.operator*().character));

            }

        }

        // BUSCAR EN HERMANOS
        if (!current_node.right_sibling().is_null() && aux.is_null()){     // El nodo no tiene hijos o ninguno es valido
            aux = find_in_siblings(this->current_node);

            if (!aux.is_null()){     // ENCONTRADO HERMANO VALIDO
                //Insertamos el anterior caracter usado de nuevo a los caracteres disponibles
                available_letters.insert(current_node.operator*().character);

                // Movemos el nodo actual al nuevo nodo valido encontrado
                current_node = aux;

                // Eliminamos el anterior caracter e insertamos el nuevo
                current_word.pop_back();
                current_word.push_back(current_node.operator*().character);
                available_letters.erase(available_letters.find(current_node.operator*().character));
            }
        }

        // BUSCAR EN HERMANOS DEL PADRE
        if(aux.is_null()){      // El nodo no tenia hermanos o ninguno es valido

            do {
                // Subimos un nivel y comprobamos los hermanos
                available_letters.insert(current_node.operator*().character);
                current_word.pop_back();

                current_node = current_node.parent();

                --level;

                aux = find_in_siblings(current_node);
            } while (!current_node.parent().is_null() && aux.is_null() );

            if (current_node.parent().is_null()){   //HEMOS VUELTO AL NODO RAIZ, ENVIAMOS NODO END
                current_node = node();
                level = 0;
                current_word = "";
                available_letters.clear();

            } else {
                // ELIMINAMOS LA LETRA Y AÑADIMOS EL HERMANO ENCONTRADO
                available_letters.insert(current_node.operator*().character);
                current_word.pop_back();

                current_node = aux;
                current_word.push_back(current_node.operator*().character);
                available_letters.erase(available_letters.find(current_node.operator*().character));

            }

        }

    } while (level != 0 && !(current_node.operator*().valid_word));

    return (*this);
}

std::string Dictionary::possible_words_iterator::operator*() const {
    return(this->current_word);
}

bool Dictionary::possible_words_iterator::is_equal (const Dictionary::possible_words_iterator & first, const Dictionary::possible_words_iterator & second) const{
    bool son_iguales = true;

    if (first.current_word != second.current_word){
        son_iguales = false;

    } else if (first.current_node != second.current_node){
        son_iguales = false;

    } else if (first.level != second.level){
        son_iguales = false;

    } else if (first.available_letters != second.available_letters){
        son_iguales = false;

    }

    return (son_iguales);


}

bool Dictionary::possible_words_iterator::is_valid(Dictionary::node nodo) {
    bool encontrada = true;
    char letra = (*nodo).character;

    if (available_letters.find(letra) == available_letters.end()){  // NO ENCONTRADA
        encontrada = false;
    }

    return (encontrada);

}

Dictionary::node Dictionary::possible_words_iterator::find_in_siblings (Dictionary::node nodo){
    node result;
    bool encontrado = false;


    while (!nodo.is_null() && !nodo.right_sibling().is_null() && !encontrado){
        nodo = nodo.right_sibling();

        encontrado = is_valid(nodo);
    }

    if (!encontrado){
        result = node();
    } else {
        result = nodo;
    }

    return (result);

}

Dictionary::node Dictionary::possible_words_iterator::find_in_childs (Dictionary::node nodo) {
    node aux;

    if (is_valid(nodo.left_child())){
        aux = current_node.left_child();
    } else {
        aux = find_in_siblings(nodo.left_child());
    }

    return (aux);
}