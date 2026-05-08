#include <algorithm>
#include <format>
#include <functional>
#include <iostream>
#include <chrono>



#include "advanced_concepts/advanced_template/metaprogramming.h"
#include "advanced_concepts/advanced_template/template_specialisation.h"
#include "headers/template_variadique_use.h"


// constante (définition dépréciée)
#define DEFINE 15

// Macro fonction
#define MIN(x,y) ((x) > (y) ? (y) : (x))

using string = std::string;
using std::array;
using std::vector;
using std::array;


template <typename T>
void print(const T& t ) {
    std::cout << t << std::endl;
}

// utilisation de fonctions définies dans des fichiers d'en tete
// de maniere indépendante (voir fichier Inline.cpp)
extern int add(const int& a, const int& b);
extern float nb_virgule;


// argc = nombre de parametres passés au programme à son éxécution
// argv, listes des parametres passés en argument au programme
int main(const int argc, char* argv[]) {


    




    exit(0);
}