//
// Created by thierry on 02/05/2026.
//

#ifndef TUTO_CPP_VALUE_TYPE_USE_H
#define TUTO_CPP_VALUE_TYPE_USE_H

#include <iostream>

// passage par lvalue reference (en fait accepte tout type de paramètre)
inline int rvalue_sum(const int& a, const int& b) {
    return a+b;
}

// passage par rvalue reference (n'accepte que des variables temporaires ou des xvalues)
inline int rvalue_sum(const int&& a, const int&& b) {
    std::cout << "passage par rvalue reference" << std::endl;
    return a + b;
}


// passage par lvalue reference (n'accepte que des réferences vers des variables persistentes)
inline void process_value(const int& a) {
    std::cout << "je suis une lvalue reference" << std::endl;
}


inline void process_value(int&& a) {
    std::cout << "je suis une rvalue reference" << std::endl;
}


// Perfect forwading (accepte tout type d'objet .... je crois)
template<class T>
void choose_type(T&& arg) {
    process_value(std::forward<T>(arg));
    std::cout << "classe template" << std::endl;
}



#endif //TUTO_CPP_VALUE_TYPE_USE_H
