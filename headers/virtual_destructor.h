//
// Created by thierry on 02/05/2026.
//

#ifndef TUTO_CPP_VIRTUAL_DESTRUCTOR_H
#define TUTO_CPP_VIRTUAL_DESTRUCTOR_H
#include <iostream>
#include <ostream>

using string = std::string;

class Voiture {
public:
    explicit Voiture(string  marque): m_marque(std::move(marque)) {
        std::cout << "constructeur de base appelé" << std::endl;
    }
    virtual ~Voiture() {
        std::cout << "destructeur  Voiture  appelé" << std::endl;
    }
private:
    string m_marque;
};


class Berline: public Voiture {
public:
    explicit Berline(string marque) : Voiture(std::move(marque)) {
        std::cout << "Constructeur de la berline appelé" << std::endl;
    }
    ~Berline() override {
        std::cout << "destructeur de berline appelée" << std::endl;
    }
};

#endif //TUTO_CPP_VIRTUAL_DESTRUCTOR_H
