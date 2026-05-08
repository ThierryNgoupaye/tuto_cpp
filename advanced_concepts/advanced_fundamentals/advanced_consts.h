//
// Created by thierry on 06/05/2026.
//

#ifndef TUTO_CPP_ADVANCED_CONSTS_H
#define TUTO_CPP_ADVANCED_CONSTS_H
#include <iostream>
#include <ostream>


// const = non-modifiable (en lecture seulement)
// constexpr = évalué à la compilation
// consteval = obligatoirement évaluée à la compilation --- ne s'applique qu'au fonctions
// constinit = initialisé à la compilation mais modifiable


/**
 * constexpr
 * évalué à la compilation
 */

constexpr  int MAX = 10;     // valeur connue à la compilation et non modifiable
inline int tableau[MAX];   // taille du tableau connu à la compilation#
constexpr int add(const int a, const int b) { return a + b; }  // la fonction peut etre calculée à la compilation si on lui donne des parametres dont les valeurs sont connues à la compilation sinon elle se comporte comme une fonction normale
constexpr int factorielle(const int n) {
    if (n<=1) return 1;
    return n * factorielle(n-1);
}   // Cette fonction, à son appel est évaluée à la compilation si possible, sinon se comporte comme une fonction normale et sera évaluée au runtime.

template<int N>
class Vec{};
inline Vec<MAX> v;    // template parameter, connu à la compilation, et disponible dans la stack du programme et non dans le heap


// classes constexpr
class Point {
    double m_x, m_y;
    public:
    constexpr Point(const double x, const double y) : m_x(x), m_y(y) {}
    [[nodiscard]] constexpr  double x() const { return m_x; }
    [[nodiscard]] constexpr double y() const { return m_y; }

    [[nodiscard]] constexpr double distanceOrange() const {
        return m_x*m_x + m_y*m_y;
    }

    [[nodiscard]] constexpr Point operator+(const Point& other) const {
        const Point p(m_x + other.m_x, m_y + other.m_y);
        return p;
    }
};

inline void process_classes_constexpr() {
    constexpr Point p1(1.0,4.8);
    constexpr Point p2(2.0,4.8);
    constexpr Point p3 = p1 + p2;
    constexpr  double dist = p1.distanceOrange();

    std::cout << "p3 = (" << p3.x() << ", " << p3.y() << ")" << std::endl;
    std::cout << "dist = " << dist << std::endl;
}


// branchement if constexpr
template <class T>
void afficher_valeur(const T& val) {
    if constexpr (std::is_integral_v<T>) {
        std::cout << "Entier : " << val << std::endl;   // les conditions sont évaluées pendant la compilation.
    }
    else if constexpr (std::is_floating_point_v<T>) {
        std::cout << "Floattant : " << val << std::endl;
    }
    else {
        std::cout << "Autre : " << val << std::endl;
    }
}



template<class T>
std::string toString(const T& val) {
    if constexpr (std::is_same_v<T, std::string>) {
        return val;
    }
    else if constexpr (std::is_arithmetic_v<T>) {
        return std::to_string(val);
    }
    else {
        return "inconnu";
    }
}


/***
 * consteval
 * force l'évaluation d'une fonction à la compilation
 */

consteval int carre_eval(const int x) {
    return x*x;
}  // cette fonction etre obligatoirement compile time sinon une erreur est levé, et donc il faut s'assurer que les paramètres qu'on lui injecte soient des constexpr, ou des const


/**
 * Constinit
 * la variable doit etre évaluée à la compilation, mais peut changer durant l'exécution du programme entier
 */
inline constinit int compteur = 0;  // initialisé à la compilation et non au runtime
inline void modify_constinit_var() {
    compteur = 5;
    std::cout << compteur << std::endl;
}








#endif //TUTO_CPP_ADVANCED_CONSTS_H
