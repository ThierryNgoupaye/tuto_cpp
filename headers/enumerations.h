//
// Created by thierry on 02/05/2026.
//

#ifndef TUTO_CPP_ENUMERATIONS_H
#define TUTO_CPP_ENUMERATIONS_H
#include <iostream>


// on y accède directement
enum LogLevel {
    INFO  ,
    WARNING,
    ERROR,
    FATAL,
    EXCEPTION
};


// s'accède via portée (Mois::JANVIER)
enum class Mois {
    JANVIER = 1,
    FEVRIER,
    MARS,
    AVRIL,
    MAI,
    JUIN,
    JUILLET,
    AOUT,
    SEPTEMBRE,
    OCTOBRE,
    NOVEMBRE,
    DECEMBRE
};


inline void displayMessage(const int& code) {
    switch (code) {
        case 0:
            std::cout << "Ceci est une info" << std::endl;
            break;
        case 1:
            std::cout << "Ceci est un warning" << std::endl;
            break;
        case 2:
            std::cout << "Ceci est une erreur" << std::endl;
            break;
        default:
            std::cout << "Inconnu" << std::endl;
            break;
    }
}

#endif //TUTO_CPP_ENUMERATIONS_H
