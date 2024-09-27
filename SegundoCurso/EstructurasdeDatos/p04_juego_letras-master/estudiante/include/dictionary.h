//
// Created by fluque1995 on 8/9/21.
//

#ifndef TREE_DICTIONARY_HPP
#define TREE_DICTIONARY_HPP

#include "tree.h"
#include <string>
#include <iostream>
#include <vector>
#include <utility>
#include <set>

using namespace std;

/**
 * @brief TDA Dictionary
 * @details Almacena las palabras de un fichero de texto y permite iterar sobre ellas
 *
 */
class Dictionary {
private:
    /**
     * @brief Struct to represent a character inside the tree.
     *
     * The struct contains information about the character it that node, and
     * information marking if a valid word of the dictionary finishes in
     * that character
     */
    struct char_info {
        char character;
        bool valid_word;

        /**
         * Default constructor
         */
        inline char_info() : character(0), valid_word(false) {}

        /**
         * @brief Parameters constructor
         *
         * @param character Character to be inserted
         * @param valid Marker for word validity
         */
        inline char_info(char character, bool valid = false) : character(character), valid_word(valid) {}

        inline bool operator==(const char_info &rhs) {
          return this->character == rhs.character && this->valid_word == rhs.valid_word;
        }

        inline bool operator!=(const char_info &rhs) { return !(*this == rhs); }

        inline bool operator*() { return this->character; }

        inline friend std::ostream &operator<<(std::ostream &os, const char_info &other) {
          os << other.character << (other.valid_word ? "✓" : "×");
          return os;
        }
    };

    tree<char_info> words;
    int total_words;
    typedef tree<char_info>::node node;

    /**
     * @brief Find the lower bound of a character among the children of the current node
     *
     * If the character exists among the children of the current node, that node is returned. If not,
     * the returned node is the one with the biggest character (in terms of alphabetical order) smaller
     * than the searched one
     *
     * @param character Character to be found
     * @param current Reference node, the one whose children are going to be explored
     * @return Lower bound node for the seeked character
     */
    node findLowerBoundChildNode(char character, node current);

    /**
     * @brief Insert character as children of the current node
     *
     * This method tries to insert a new character in the tree, as a child of the current node.
     * If there already exists a node with that character, instead of inserting a new node, the
     * already existing node with the character is returned
     *
     * @param character Character to be inserted
     * @param current Reference node, the one that will be parent of the new inserted node
     * @return Node corresponding to the inserted character
     */
    node insertCharacter(char character, node current);

    /**
     * @brief Funcion que cuenta las ocurrencias de la letra en el subarbol que cuelga del nodo curr_node
     * @param curr_node Nodo a partir del cual se haran las comprobaciones
     * @param c Caracter a comprobar
     * @return Numero de ocurrencias del caracter c en el subarbol de raiz curr_node
     */
    int getOccurrences(node curr_node, char c);

    /**
     * @brief Funcion que devuelve el numero de ocurrencias del caracter en los nodos que cuelgan del nodo actual
     * y el numero de palabras que terminan por debajo del nodo actual
     * @param curr_node Nodo a tomar como raiz del subarbol
     * @param c Caracter a buscar
     * @return Pareja de valores formada por el numero de ocurrencias y el numero de palabras que terminan
     * por debajo del nodo actual
     */
    std::pair<int, int> getTotalUsages(node curr_node, char c);

public:

    /**
     * @brief Constructor por defecto
     *
     * Crea un Dictionary vacío
     */
    Dictionary();

    /**
     * @brief Constructor de copia
     *
     * Crea un Dictionary con el mismo contenido que el que se pasa como argumento
     *
     * @param other Dictionary que se quiere copiar
     */
    Dictionary(const Dictionary &other);

    /**
     * @brief Destructor
     *
     * Limpia todos los elementos del Dictionary antes de borrarlo
     */
    ~Dictionary();

    /**
     *  \brief Limpia el Dictionary
     *
     *  Elimina todas las palabras contenidas en el conjunto
     */
    void clear() { this->words.clear(); this->total_words = 0; }

    /**
     * @brief Tamaño del diccionario
     *
     * @return Número de palabras guardadas en el diccionario
     */
    unsigned int size() const { return this->total_words; }

    /**
     * @brief Comprueba si el diccionario está vacio.
     * @return true si el diccionario está vacío, false en caso contrario
     */
    bool empty() const { return this->total_words == 0; }

    /**
     * @brief Indica si una palabra esta en el diccionario o no.
     *
     * Este método comprueba si una determinada palabra se encuentra o no en el dicccionario
     *
     * @param palabra: la palabra que se quiere buscar.
     * @return Booleano indicando si la palabra existe o no en el diccionario
     */
    bool exists(const string &val);

    /**
     * @brief Inserta una palabra en el diccionario
     *
     * @param val palaba a insertar en el diccionario
     * @return Booleano que indica si la inserción ha tenido éxito. Una palabra se inserta
     * con éxito si no existía previamente en el diccionario
     */
    bool insert(const string &val);

    /**
     * @brief Elimina una palabra del diccionario
     *
     * @param val Palabra a borrar del diccionario
     *
     * @return Booleano que indica si la palabra se ha borrado del diccionario
     */
    bool erase(const string &val);

    /**
     * @brief Sobrecarga del operador de asignación
     */
    Dictionary &operator=(const Dictionary &dic);

    /**
     * @brief Sobrecarga del operador de entrada
     *
     * Permite leer las palabras de un fichero de texto e introducirlas en el
     * diccionario
     *
     * @param is Flujo de entrada
     * @param dic Diccionario a rellenar
     * @return Flujo de entrada para poder encadenar el operador
     */
    friend istream &operator>>(istream &is, Dictionary &dic);

    /**
     * @brief Sobrecarga del operador de salida
     *
     * Permite imprimir el diccionario completo a un flujo de salida
     *
     * @param os Flujo de salida, donde imprimir el diccionario
     * @param dic Diccionario a imprimir
     * @return Flujo de salida, para poder encadenar el operador
     */
    friend ostream &operator<<(ostream &os, const Dictionary &dic);

    //////////////////////////////////////////////// Recursive functions ///////////////////////////////////////////////
    /**
     * @brief Funcion que permite conocer el numero de ocurrencias de un caracter que se pasa como argumento en nuestro
     * arbol
     * @param c Caracter a comprobar
     * @return Devuelve el numero de ocurrencias del caracter
     */
    int getOccurrences(const char c);

    /**
     * @brief Funcion que permite conocer el numero de usos de la letra para formar palabras en el diccionario
     * @param c Caracter a comprobar
     * @return Devuelve el numero de usos totales de la letra
     */
    int getTotalUsages(const char c);

    ///////////////////////////////////////////////////// Iterator ////////////////////////////////////////////////////
    /**
     * @brief Iterador basico de la clase diccionario.
     * @details Permite iterar por todas las palabras del diccionario
     */
    class iterator {
    private:
        std::string curr_word;
        tree<char_info>::const_preorder_iterator iter;

    public:

        iterator();

        iterator(tree<char_info>::const_preorder_iterator iter);

        iterator(const Dictionary::iterator & otro);

        std::string operator*();

        iterator &operator++();

        bool operator==(const iterator &other);

        bool operator!=(const iterator &other);

    };

    /**
     * @brief Permite acceder a la primera palabra de nuestro diccionario
     * @return Iterador a la primera palabra del diccionario
     */
    iterator begin() const;

    /**
     * @brief Iterador a la siguiente palabra a la ultima de nuestro diccionario
     * @return Iterador a una palabra nula
     */
    iterator end() const;

    ///////////////////////////////////////////////// Letters Iterator /////////////////////////////////////////////////
    /**
     * @brief Iterador de palabras válidas
     * @details Permite iterar solo por las palabras del diccionario que sean validas segun un conjunto de letras disponibles
     */
    class possible_words_iterator {
    public:
        possible_words_iterator();

        possible_words_iterator(node current_node, vector<char> available_letters);

        possible_words_iterator(const possible_words_iterator &other);

        possible_words_iterator &operator=(const possible_words_iterator &other);

        //usar funcion is_equal
        bool operator==(const possible_words_iterator &other) const;

        bool operator!=(const possible_words_iterator &other) const;

        //do{next_node
        //
        //}while(curr_node.valid_word)
        //next_node: Buscar:
        //1. Buscar hijos
        //2. Buscar hermanos
        //3. Buscar hermanos padre
        // Usar bool find_in_siblings(node)
        // bool is_valid(node) Mira si la letra del nodo esta en las letras disponibles
        // add_letter(char)
        // extract_letter(
        possible_words_iterator &operator++();

        std::string operator*() const;

    private:
        multiset<char> available_letters;
        int level;
        node current_node;
        string current_word;

    private:
        bool is_equal (const Dictionary::possible_words_iterator & first, const Dictionary::possible_words_iterator & second) const;
        bool is_valid (node current_node);
        node find_in_siblings (node nodo);
        node find_in_childs (node nodo);
    };

    /**
     * @brief Permite acceder a la primera palabra de nuestro diccionario que se pueda formar con unas
     * letras dadas
     * @param available_characters Conjunto de letras disponibles para formar palabras
     * @return Primera palabra valida con las letras que tenemos
     */
    possible_words_iterator possible_words_begin(vector<char> available_characters) const;

    /**
     * @brief Iterador a la siguiente palabra valida a la ultima de nuestro diccionario
     * @return Iterador a palabra nula
     */
    possible_words_iterator possible_words_end() const;
};

#endif //TREE_DICTIONARY_HPP
