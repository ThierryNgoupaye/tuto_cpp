//
// Created by thierry on 10/05/2026.
//

#ifndef TUTO_CPP_COMPILATEUR_H
#define TUTO_CPP_COMPILATEUR_H


// Utile pour interfacer avec du code C !

// Sans extern "C" = nom manglé et donc un programme C ne peut pas appeler
//inline void maFonction(int x) { }
// transformé en _Z10maFonctioni qui est incompréhensible pour C !

// Avec extern "C", le nom de la fonction est non manglé
extern "C" inline void maFonction(int x) { }
// maFonction() peut etre appelée directement dans un programme C

// Cas pratique — bibliothèque utilisable en C et C++
#ifdef __cplusplus
    extern "C" {
#endif

void initialiser();
void traiter(int* data, int taille);
void terminer();

#ifdef __cplusplus
    }
#endif





// # AddressSanitizer → détecte accès mémoire invalides
// g++ -fsanitize=address -std=c++23 main.cpp
// ./a.out
//
// # UndefinedBehaviorSanitizer → détecte l'UB
// g++ -fsanitize=undefined -std=c++23 main.cpp
// ./a.out
//
// # Les deux ensemble → recommandé en développement
// g++ -fsanitize=address,undefined -g -std=c++23 main.cpp
// ./a.out
//
// # Dans CMakeLists.txt pour le mode Debug
// if(CMAKE_BUILD_TYPE STREQUAL "Debug")
//     target_compile_options(${PROJECT_NAME} PRIVATE
//         -fsanitize=address,undefined
//     )
//     target_link_options(${PROJECT_NAME} PRIVATE
//         -fsanitize=address,undefined
//     )
// endif()





// https://godbolt.org
// → site indispensable pour tout développeur C++ !
// → tu colles ton code
// → tu choisis GCC/Clang + flags
// → tu vois l'assembleur généré en temps réel
// → tu comprends exactement ce que fait le compilateur

#endif //TUTO_CPP_COMPILATEUR_H



