//
// Created by thierry on 02/05/2026.
//

#ifndef TUTO_CPP_STATIC_KEY_WORD_USE_H
#define TUTO_CPP_STATIC_KEY_WORD_USE_H

#include <iostream>


// Une variable statique peut continue de vivre en dehors de la
// portée de la fonction qui l'a créée
inline void incrementStatic() {
    static int a = 0;
    a++;
    std::cout << a << std::endl;
}



// un membre statique d'une classe peut etre accédé hors de sa classe
// grâce à l'opérateur de portée NomClasse::nom_membre
class Stat {
public:
    explicit Stat(const int a):  m_a(a) {}

    void displayAttributes() const {
        std::cout <<m_a << std::endl;
        std::cout << m_b << std::endl;
    }

private:
    int m_a;
    static int m_b;
};

int Stat::m_b = 10;



#endif //TUTO_CPP_STATIC_KEY_WORD_USE_H
