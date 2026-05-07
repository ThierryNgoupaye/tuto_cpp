//
// Created by thierry on 01/05/2026.
//

#ifndef TUTO_CPP_U_CONTAINER_H
#define TUTO_CPP_U_CONTAINER_H

#include <ostream>
#include <string>

using string = std::string;

struct st {
    st(const int& A, const int& B): a(A), b(B) {};

    int a, b;
};

union union_use {
    explicit union_use(const st& S) : my_struct(S) {}
    int a;
    float b;
    double c;
    char d;
    bool e;
    st my_struct;
};


// Liaisons sécurisées

struct StructuredBinding  {
    int a;
    char c;
    string mot;

};



inline void print_structured_binding() {
    StructuredBinding st = {12, 'a', "exemple"};

    auto [i, j, l] = st;
    std::cout << j << ' ' << i << ' ' << l << std::endl;


    // Marche meme avec des arrays
    array tab = {1,3,6,7,8};
    auto [a, b, c,d,e] = tab;


    // On peut utiliser les structured binding sur des classes aussi mais attention aux objets dynamiques et complexes

}







#endif //TUTO_CPP_U_CONTAINER_H
