//
// Created by thierry on 07/05/2026.
//

#ifndef TUTO_CPP_UTILITAIRES_STL_H
#define TUTO_CPP_UTILITAIRES_STL_H

#include <iostream>
#include <vector>
#include  <string>
#include <optional>


// Optional
inline std::optional<int> parserEntier(const std::string& s) {
    try {
        return std::stoi(s);
    }
    catch (...) {
        return std::nullopt;
    }
}


inline std::optional<int> chercher(const std::vector<int> &v, const int val) {
    for (const int x : v) {
        if (x == val) {
            return x;
        }
    }
    return std::nullopt;
}

inline void try_optional() {

    const std::vector v = {1,2,3,4,5};

    const auto result = chercher(v,3);

    // Cas correct, valeur présente
    if (result.has_value()) {
        std::cout << "Trouvé : " << result.value() << std::endl;
    }

    if (result) {
        std::cout << "Trouvé : " << *result << std::endl;
    }

    // Valeur non présente
    const auto result2 = chercher(v, 99);
    if (!result2) {
        std::cout << "Pas trouvé !" << std::endl;
    }

    std::cout << result2.value_or(-1) << std::endl;  // retourne par défaut -1 si nullopt
}



// Variant (union typée et sûre)




#endif //TUTO_CPP_UTILITAIRES_STL_H
