#include <algorithm>
#include <format>
#include <functional>
#include <iostream>
#include <chrono>





#include "advanced_concepts/advanced_template/metaprogramming.h"
#include "advanced_concepts/advanced_template/template_specialisation.h"
#include "advanced_concepts/concurrency_parallelism/asynchrone.h"
#include "advanced_concepts/concurrency_parallelism/atomic_op.h"
#include "advanced_concepts/concurrency_parallelism/condition_variables.h"
#include "advanced_concepts/concurrency_parallelism/coroutines.h"
#include "advanced_concepts/concurrency_parallelism/mutex_concepts.h"
#include "advanced_concepts/design_pattern_architectures/behavior_pattern.h"
#include "advanced_concepts/design_pattern_architectures/cpp_pattern.h"
#include "advanced_concepts/design_pattern_architectures/pattern_creationnels.h"
#include "advanced_concepts/modern_cpp/cpp_17.h"
#include "advanced_concepts/modern_cpp/cpp_23.h"
#include "advanced_concepts/optimization/advanced_preprocessing.h"
#include "advanced_concepts/optimization/low_level.h"
#include "advanced_concepts/optimization/optimization.h"

#include "headers/template_variadique_use.h"


// constante (définition dépréciée)
#define DEFINE 15

// Macro fonction
#define MIN(x,y) ((x) > (y) ? (y) : (x))

using string = std::string;
using std::array;
using std::vector;
using std::array;


// utilisation de fonctions définies dans des fichiers d'en tete
// de maniere indépendante (voir fichier Inline.cpp)
extern int add(const int& a, const int& b);
extern float nb_virgule;


// argc = nombre de parametres passés au programme à son éxécution
// argv, listes des parametres passés en argument au programme
int main(const int argc, char* argv[]) {

    ranges_to_use();

    exit(0);
}