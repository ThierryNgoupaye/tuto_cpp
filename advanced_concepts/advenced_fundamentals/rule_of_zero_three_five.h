//
// Created by thierry on 05/05/2026.
//

#ifndef TUTO_CPP_RULE_OF_ZERO_THREE_FIVE_H
#define TUTO_CPP_RULE_OF_ZERO_THREE_FIVE_H
#include <algorithm>
#include <iostream>
#include <ostream>
#include <string>
#include <utility>



class MaClass {

    // Une classe peut avoir 5 fonctions que le compilateur peut générer automatiquement

    public:
        MaClass() = default;   // constructeur par défaut

        MaClass(const MaClass&) = default;  // constructeur de copie
        MaClass& operator=(const MaClass&) = default;  // operateur de copie
        MaClass(MaClass&&) = default;  // constructeur move
        MaClass& operator=(MaClass&&) = default;  // operateur move
        ~MaClass() = default;
};



/**
 * Regle de zero : le compilateur génère tout correctement.
 */
class Personne {

    std::string m_nom;
    int m_age;

    public:
        explicit Personne(std::string nom, const int age) : m_nom(std::move(nom)), m_age(age) {}

    // pas de destructeur
    // pas de constructeur de copie
    // pas de constructeur de move
    // pas d'opérateur de copie
    // pas d'opérateur de move
};



inline void zero_rule() {
    Personne p1("Alcie", 25);
    Personne p2 = p1;    // operateur de copie généré automatiquement
    Personne p3 = std::move(p1);    // opérateur move généré automatiquement.

}



/**
 * Règle des trois
 * Si on définit un des 3, on les définit tous (destructeur, constructeur de copie, opérateur de copie)
 * Si on le fait, le compilateur en appelant le constructeur de copie ou l'opérateur d'assignation, va juste faire
 * pointer la nouvelle variable vers l'ancienne
 * Ça peut etre un danger, car si on détruit un (via le destructeur), on détruit les deux
 */
class Buffer {
    int* m_data;
    int m_taille;

public:
    explicit Buffer(const int taille): m_taille(taille) {
        m_data = new int[m_taille];
        std::cout << "Constructeur appelé" << std::endl;
    }

    // Destructeur
    ~Buffer() {
        delete[] m_data;
        std::cout << "Destructeur appelé " << std::endl;
    }

    Buffer(const Buffer& other) : m_taille(other.m_taille) {
        this->m_data = new int[m_taille];
        std::copy_n(other.m_data, this->m_taille, this->m_data);
        std::cout << "Constructeur de copie" << std::endl;
    }


    Buffer& operator=(const Buffer& other) {
        if (this == &other) {
            return *this;
        }
        delete[] m_data;
        this->m_taille = other.m_taille;
        this->m_data = new int[m_taille];
        std::copy_n(other.m_data, m_taille, m_data);
        std::cout << "Opérateur d'assignation " << std::endl;
        return *this;
    }


    [[nodiscard]] int size() const { return this->m_taille; }
};



/**
 * Règle des 5. Si on définit un de ces 3, on les définit tous
 */
class Structure {
public:
    // Constructeur
    explicit Structure(const int taille) : m_taille(taille) {
        m_data = new int[taille]{};
        std::cout << "Constructeur(" << taille << ")" << std::endl;
    }

    // 1. Destructeur
    ~Structure() {
        delete[] m_data;
        std::cout << "Destructeur" << std::endl;
    }

    // 2. Constructeur copie
    Structure(const Structure& other) : m_taille(other.m_taille) {
        m_data = new int[m_taille];
        std::copy_n(other.m_data, m_taille, m_data);
        std::cout << "Copie constructeur" << std::endl;
    }

    // 3. Opérateur copie
    Structure& operator=(const Structure& other) {
        if (this == &other) return *this;
        delete[] m_data;
        m_taille = other.m_taille;
        m_data = new int[m_taille];
        std::copy_n(other.m_data, m_taille, m_data);
        std::cout << "Copie assignation" << std::endl;
        return *this;
    }

    // 4. Constructeur move
    // Structure(Structure&& other) noexcept : m_data(other.m_data), m_taille(other.m_taille) {
    //     other.m_data  = nullptr;
    //     other.m_taille = 0;
    //     std::cout << "Move constructeur" << std::endl;
    // }


    // avec la fonction std::exchange(), on peut plus facilement écrire les constructeur et opérateur de move
    Structure(Structure&& other) noexcept : m_data(std::exchange(other.m_data, nullptr)), m_taille(std::exchange(other.m_taille, 0)){}

    // 5. Opérateur move
    Structure& operator=(Structure&& other) noexcept {
        if (this == &other) return *this;
        delete[] m_data;
        this->m_data = std::exchange(other.m_data, nullptr);
        this->m_taille = std::exchange(other.m_taille, 0);
        std::cout << "Move assignation" << std::endl;
        return *this;
    }

    [[nodiscard]] int size() const { return m_taille; }

private:
    int* m_data;
    int  m_taille;
};



/**
 * On peut interdire certains constructeurs
 */
class Connexion {
    std::string m_url;
public:
    explicit  Connexion(std::string connexion) : m_url(std::move(connexion)) {};

    Connexion(const Connexion& other) = delete;
    Connexion& operator=(const Connexion& other) = delete;

    Connexion(Connexion&& other) = default;
    Connexion& operator=(Connexion&& other) = default;

    ~Connexion() = default;
};


inline void unauthorize_constructor() {
    Connexion c1("htpps://www.google.com");

    //Connexion c2 = c1; // erreur de compilation

    Connexion c2 = std::move(c1);

}


#endif //TUTO_CPP_RULE_OF_ZERO_THREE_FIVE_H
