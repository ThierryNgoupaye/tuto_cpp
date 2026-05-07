//
// Created by thierry on 05/05/2026.
//

#ifndef TUTO_CPP_SINGLETON_PATTERN_USE_H
#define TUTO_CPP_SINGLETON_PATTERN_USE_H

#include <iostream>


class Singleton {

    Singleton() {
        std::cout << "Instance créee" << std::endl;
    }


public:
    static Singleton& get_instance() {
        static Singleton instance;
        return instance;
    }
    Singleton(const Singleton&) = delete;
    Singleton &operator=(const Singleton &) = delete;

     void display_message() {
        std::cout << "Instance appelée" << std::endl;
    }
};


inline void apply() {
    Singleton& s1 = Singleton::get_instance();

    s1.display_message();

    const Singleton& s2 = Singleton::get_instance();

    std::cout << "s1: " << &s1 <<  ", s2: " << &s2  <<std::endl;
}


#endif //TUTO_CPP_SINGLETON_PATTERN_USE_H
