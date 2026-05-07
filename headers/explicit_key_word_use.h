//
// Created by thierry on 02/05/2026.
//

#ifndef TUTO_CPP_EXPLICIT_KEY_WORD_USE_H
#define TUTO_CPP_EXPLICIT_KEY_WORD_USE_H
#include <string>

using string = std::string;

/*
 * On utilise le mot clé explicit pour evitier les conversations explicites par le compilateur
 * durant l'instanciation d'un objet ne prenant qu'un seul parametre
 * par son constructeur.
 */

class Player {
public:
    explicit Player(const int& age) : m_age(age) {
        m_name = "Anonyme";
    }
    explicit Player(string m_name): m_name(std::move(m_name)) {
        m_age = 0;
    }
    [[nodiscard]] int getAge() const {
        return m_age;
    }

private:
    int m_age;
    string m_name;
};


#endif //TUTO_CPP_EXPLICIT_KEY_WORD_USE_H
