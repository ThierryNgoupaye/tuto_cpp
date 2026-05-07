//
// Created by thierry on 02/05/2026.
//

#ifndef TUTO_CPP_CHRONO_USE_H
#define TUTO_CPP_CHRONO_USE_H
#include <functional>
#include <chrono>
#include <iostream>

using namespace std::chrono;


template <class R, class ...Params>
using Function = std::function<R(Params...)>;

// autre facon de definir un type pour referencer une fonction;
template <class R, class ...Params>
using Func = R(*)(Params...);


inline void print_today_date() {

    // floor prend un time_point et l'arrondit au jour près
    const auto today = year_month_day{floor<days>(system_clock::now())};
    const auto _day = today.day();
    const auto _month = today.month();
    const auto _year = today.year();

    // temps qu'il s'est écoulé depuis le 1er jan 1970
    const auto time_since = system_clock::now().time_since_epoch();
    const auto in_days = duration_cast<days>(time_since);
    const auto in_years = duration_cast<years>(time_since);

    std::cout << "Jour écoulé depuis le 1er janvier 1979: " << in_days << std::endl;
    std::cout << "Années écoulées depuis le 1er janvier 1979: " << in_years << std::endl;


   std::cout << "Today's date: " << _year << "-" << _month << "-" << _day  << std::endl;

}


template <class R, class ...Params>
unsigned long int print_func_duration(Function<R, Params...> func, Params... params) {

    // on peut avoir system_clock ou aussi
    const auto start = steady_clock::now();
    func(params...);
    const auto end = steady_clock::now();
    const milliseconds duration = duration_cast<milliseconds>(end-start);

    std::cout << "Durée d'exécution de la fonction " << duration.count() << " millisecondes" << std::endl ;
    return duration.count();
}


template <class R>
unsigned long int print_func_duration(Function<R> func) {

    const auto start = steady_clock::now();
    func();
    const auto end = steady_clock::now();
    const milliseconds duration = duration_cast<milliseconds>(end-start);

    std::cout << "Durée d'exécution de la fonction " << duration.count() << " millisecondes" << std::endl ;
    return duration.count();
}





// Suppléments

// Exemple de passage d'une fonction par réference à une autre fonction
template <typename T>
void ecrire(const std::function<void(const T&)>& func, const T& t) {
    func(t);
}

// dangereux, mais possible (on peut passer une fonction ici par auto)
void perform(auto& operation, const int a) {
    return operation(a);
}





#endif //TUTO_CPP_CHRONO_USE_H
