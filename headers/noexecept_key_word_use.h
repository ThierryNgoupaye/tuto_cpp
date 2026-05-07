//
// Created by thierry on 02/05/2026.
//

#ifndef TUTO_CPP_NOEXECEPT_KEY_WORD_USE_H
#define TUTO_CPP_NOEXECEPT_KEY_WORD_USE_H

#include <iostream>
#include <exception>


inline void noexecet_function() noexcept {
    std::cout << "Nous sommes dans la fonction noexcept" << std::endl;
    throw std::runtime_error("Erreur dans la fonction noexcept");
}


inline void normal_function() {
    std::cout << "Nous sommes dans la fonction normale" << std::endl;
    throw std::runtime_error("Erreur dans la fonction normal_function");
}


inline void apply() {

    try {
        noexecet_function();
    }
    catch (const std::exception& e) {
        std::cout << "Exception attrapée depuis la fonction  noexecept_function" << std::endl;
    }
    try {
        normal_function();
    }
    catch (const std::exception& e) {
        std::cout << "Exeption attrapée depuis fonction normal_function" << std::endl;
    }
}






#endif //TUTO_CPP_NOEXECEPT_KEY_WORD_USE_H
