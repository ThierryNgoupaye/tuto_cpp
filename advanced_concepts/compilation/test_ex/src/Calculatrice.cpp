//
// Created by thierry on 15/05/2026.
//

#include "../headers/Calculatrice.h"
#include <stdexcept>


int Calculatrice::additionner(const int a, const int b) {
    return a + b;
}

int Calculatrice::soustraire(const int a, const int b) {
    return a - b;
}

double Calculatrice::diviser(const double a, const double b) {
    if (b == 0) throw std::invalid_argument("Division by zero");
    return a / b;
}

bool Calculatrice::estPair(const int n) {
    return n % 2 == 0;
}





