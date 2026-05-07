//
// Created by thierry on 02/05/2026.
//

#ifndef TUTO_CPP_MUTABLE_KEY_WORD_USE_H
#define TUTO_CPP_MUTABLE_KEY_WORD_USE_H


struct entity {

    mutable int a;
    int b;
    entity(const int& a, const int& b) : a(a), b(b) {}

    void increment() const {
        a++;
    }
};


#endif //TUTO_CPP_MUTABLE_KEY_WORD_USE_H
