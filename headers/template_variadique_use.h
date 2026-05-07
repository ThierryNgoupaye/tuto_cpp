//
// Created by thierry on 05/05/2026.
//

#ifndef TUTO_CPP_TEMPLATE_VARIADIQUE_USE_H
#define TUTO_CPP_TEMPLATE_VARIADIQUE_USE_H

#include <complex>
#include <cstdarg>
#include <iostream>
#include <ostream>
#include <string>
#include <utility>



// Fonctions variadiques.
// Il s'agit d'une fonction qui accepte un nombre variable d'arguments


inline int addition(const int count, ...){
    va_list args; // liste permettant de parcourir les variables passées apres le parameter count
    va_start(args, count); // on se base sur la list args, et on compte ses élements apres count
    int sum = 0;
    for (int i =0; i < count; i++) {
        sum += va_arg(args, int);
    }
    va_end(args);
    return sum;
}


inline void apply_variadic_function(){

    const int result = addition(5, 1,2,3,4,5);

    std::cout << result << std::endl;
}


// Template variadique

template<class... Args>  // les ... sont des parameters pack
void display_template(const Args&... args) {
    (std::cout << ... << args) << std::endl;

    // ce qu'il y a dans les parentheses, c'est une opération récursive

}


template<class ... Tail>
struct tuple{};

template<>
struct tuple<>{};

template<class Head, class... Tail>
struct tuple<Head, Tail ...> {
    Head m_head;
    tuple<Tail ...> m_tail;

    explicit tuple(Head head, Tail... tail) : m_head(head), m_tail(tail...) {}
};




inline void apply_Variadic_template() {
    display_template("bonjour", 'a', "papa", 12, 45.f);

    const tuple<int, double, std::string> my_tuple{2, 4.5, "exemple"};

    std::cout << my_tuple.m_head << std::endl;
    std::cout << my_tuple.m_tail.m_head << std::endl;
    std::cout << my_tuple.m_tail.m_tail.m_head << std::endl;
}









#endif //TUTO_CPP_TEMPLATE_VARIADIQUE_USE_H
