//
// Created by thierry on 06/05/2026.
//

#ifndef TUTO_CPP_TEMPLATE_SPECIALISATION_H
#define TUTO_CPP_TEMPLATE_SPECIALISATION_H
#include <iostream>
#include <ostream>

/**
 * Spécialisation complète
 */

// Spécialisation générique
template <typename T>
void afficher_(T val) {
    std::cout << "Valeur : " << val << std::endl;
}

// Spécialisation complète pour bool
template <>
inline void afficher_<bool>(const bool val) {
    std::cout << "Bool : " << (val ? "true" : "false") << std::endl;
}

// Spécialisation complète pour const char*
template <>
inline void afficher_<const char*>(const char* val) {
    std::cout << "String : \"" << val << "\"" << std::endl;
}



// Pour classe
template <class T>
class Stockage {
    T m_val;
public:
    explicit Stockage(T val) : m_val(val) {}
    void afficher() const {
        std::cout << "Générique : " << m_val << std::endl;
    }
};


// Spécialisation complète pour un bool
template <>
class Stockage<bool> {
    unsigned char m_val : 1;
    public:
    explicit Stockage(const bool val) : m_val(val ? 1 : 0) {}
    void afficher() const {
        std::cout << "Bool optimisé : " << (m_val ? "true" : "false") << std::endl;
    }
};



/**
 * Spécialisation partielle
 * on a un modele general, et puis des modeles spécialisés pour des familles de type
 */
template <class T>
class Info {
    static void afficher() {
        std::cout << "Type quelconque " << std::endl;
    }
};

// Modèle pour les pointeurs
template <typename T>
class Info<T*> {
public:
    static void afficher() {
        std::cout << "Ceci est un pointeur " << std::endl;
    }
};

#endif //TUTO_CPP_TEMPLATE_SPECIALISATION_H
