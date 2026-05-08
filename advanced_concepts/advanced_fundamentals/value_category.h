//
// Created by thierry on 05/05/2026.
//

#ifndef TUTO_CPP_VALUE_CATEGORY_H
#define TUTO_CPP_VALUE_CATEGORY_H

#include <iostream>
#include <type_traits>

template <typename T>
void category(T&& t) {
    if constexpr (std::is_lvalue_reference_v<T>) {
        std::cout << t << std::endl;
        std::cout << "lvalue reference" << std::endl;
    }
    else {
        std::cout << t << std::endl;
        std::cout << "rvalue (xvalue ou prvalue)" << std::endl;
    }
}


template <typename T>
void wrapper(T&& x) {

    category(x);  // x passé comme un lvalue car x a un nom
    category(std::move(x));  // passé comme xvalue

    category(std::forward<T>(x));   // préserve la catégorie originale de la variable x
}



class Voiture {
    std::string m_marque;
    public:
        template <typename T>
        explicit Voiture(T&& marque) : m_marque(std::forward<T>(marque)) {}

        void afficher() const {
            std::cout << "Voiture : " << m_marque << std::endl;
        }

};


inline void constructor_with_forward() {
    std::string nom = "Renault";

    const Voiture v1(nom);   //lvalue -> copie
    const Voiture v4(&nom);   // lvalue reference -> pas de copie
    const Voiture v2("Peugeot");   // prvalue -> move
    const Voiture v3(std::move(nom));   //xvalue  -> move


    v1.afficher();
    v2.afficher();
    v3.afficher();
}



#endif //TUTO_CPP_VALUE_CATEGORY_H
