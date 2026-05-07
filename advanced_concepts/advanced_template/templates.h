//
// Created by thierry on 06/05/2026.
//

#ifndef TUTO_CPP_TEMPLATES_H
#define TUTO_CPP_TEMPLATES_H

#include <deque>
#include <iostream>
#include <type_traits>


/**
 * Non-Type Template Parameters.
 * Il s'agit ici de pouvoir passer une valeur comme paramètre template
 * Les types autorisés sont les types numériques et les pointeurs.
 */

template<typename T, int N>
class Tableau {

    T m_data[N]{};

    public:
        T& operator[](int i) { return m_data[i]; }
        [[nodiscard]] int size() const { return N; }

};

inline void process_non_type_template_parameters() {
    Tableau<int, 5> t1;
    Tableau<double, 3> t2;
    Tableau<char, 10> t3;


    t1[0] = 42;
    std::cout << t1.size() << std::endl;
    std::cout << t1[0] << std::endl;
}



/**
 * Template Template Parameters
 * On peut passer un template comme paramètre d'un autre template
 * Dans ce cas le deuxieme paramètre template est un template (ex std::vector est une classe template, peut etre notre deuxieme paramètre template ici.)
 */

template<class T, template<class...> class Container>
class Pile {

    Container<T> m_container;
    public:
        void empiler(const T& val) {
            m_container.emplace_back(val);
        }
        void depiler() {
            m_container.pop_back();
        }

        [[nodiscard]] T sommet() const {
            return m_container.back();
        }

        [[nodiscard]] bool vide() const {
            return m_container.empty();
        }
};

inline void process_template_template_parameters() {
    Pile<int, std::vector> pile_vect;
    pile_vect.empiler(1);
    pile_vect.empiler(2);
    pile_vect.empiler(3);
    std::cout << pile_vect.sommet() << std::endl;


    Pile<int, std::deque> pile_dec;
    pile_dec.empiler(10);
    pile_dec.empiler(20);
    std::cout << pile_dec.sommet() << std::endl;
}


/**
 * Variadic Templates Avancés
 *
 */

template <typename... Args>
void print_number_of_template_parameters() {
    std::cout << "Nombre de types : " << sizeof...(Args) << std::endl;
}


// Acceder aux arguments individuellement
// Ceci retourne le type du N-ième argument
template<int N, typename ...Args>
using NthType = std::tuple_element_t<N, std::tuple<Args...>>;


// Fold expression
template<class... Args>
auto somme(Args ... args) {
    return (args + ...);
}

template<class... Args>
auto produit(Args... args) {
    return (args * ...);
}

template<class... Args>
void afficher_avec_separateur(Args... args) {
    bool premier = true;
    ((std::cout << (premier ? "" : ", ") << args, premier = false), ...);
    std::cout << std::endl;
}

#endif //TUTO_CPP_TEMPLATES_H
