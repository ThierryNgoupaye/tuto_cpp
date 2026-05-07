//
// Created by thierry on 02/05/2026.
//

#ifndef TUTO_CPP_NAMESPACE_USE_H
#define TUTO_CPP_NAMESPACE_USE_H
#include <iostream>

namespace Orange {
    inline void  displayFruit() {
        std::cout << "Je suis une orange" << std::endl;
    }
}

namespace Pomme {
    inline void  displayFruit() {
        std::cout << "Je suis une pomme" << std::endl;
    }
}


using namespace Orange;
using namespace Pomme;

#endif //TUTO_CPP_NAMESPACE_USE_H
