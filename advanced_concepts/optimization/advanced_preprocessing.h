//
// Created by thierry on 09/05/2026.
//

#ifndef TUTO_CPP_ADVANCED_PREPROCESSING_H
#define TUTO_CPP_ADVANCED_PREPROCESSING_H

#include <iostream>
#include <print>

// Macro Avancés.
// Stringification = transformer un nom de macro en chaine de caractères littérale
// L'opérateur # devant un paramètre = mets le paramètre entre guillemets
#define VERS_STRING(X) #X

#define DEBUG_VAR(X) \
    std::cout<< #X << " = " << (X) << std::endl


// la concaténation
// ## colle deux tokens ensemble = crée un nouvel identifiant

#define CREER_VARIABLE(type, nom) type variable_##nom

inline void print_macro_func() {

    std::cout << VERS_STRING(bonjour)  << std::endl; // "bonjour"
    std::cout << VERS_STRING(42)       << std::endl; // "42"
    std::cout << VERS_STRING(int)      << std::endl; // "int"
    std::cout << VERS_STRING(x + y)    << std::endl; // "x + y"


    constexpr int age = 25;
    constexpr double prix = 3.14;
    constexpr bool actif = true;

    DEBUG_VAR(age);   // age = 25
    DEBUG_VAR(prix);
    DEBUG_VAR(actif);


    CREER_VARIABLE(int, age);  // retourne int variable_age
    CREER_VARIABLE(double, prix);  // retourne double variable_prix

    variable_age = 25;
    variable_prix = prix;

    std::cout << variable_age << std::endl;
    std::cout << variable_prix << std::endl;
}


// Création des getters/setters automatiquement

#define PROPRIETE(type, nom)                        \
    private:                                        \
        type m_##nom{};                               \
    public:                                         \
        type get_##nom() const { return this->m_##nom; }   \
        void set_##nom(type val) { this->m_##nom = val; }


class PersonneMacro {
    PROPRIETE(std::string, nom)
    PROPRIETE(int, age)
    PROPRIETE(double, salaire)
};

inline void getters_setters_automic() {
    PersonneMacro p;
    p.set_nom("Alice");
    p.set_age(24);
    p.set_salaire(45000);


    std::println(cout, "{}", p.get_nom());
}





// __VA__RGS - Macro variadiques
// permet à une macro d'accepter un nombre variable d'arguments comme des variadic templates mais au niveau préprocesseur


#define LOGGER(fmt, ...) \
    std::cout << std::format(fmt, ##__VA_ARGS__) << std::endl    // ## permet de supprimer la virgule quand aucun des ... n'est renseigné


// __VA_OPT__(,) → ajoute une virgule SEULEMENT si args présents
#define OPT_LOGGER(fmt, ...)                                     \
    std::cout << std::format(fmt __VA_OPT__(,) __VA_ARGS__)      \
    << std::endl


inline void va_args_use() {
    LOGGER("Bonjour !");
    LOGGER("Age : {}", 25);
    LOGGER("{} + {} = {}", 3, 5, 3+5);     // 3 + 5 = 8
    OPT_LOGGER("Valeur : {}", 42);
}



// X-Macro
#define LISTE_COULEURS          \
    X(Rouge, 255,0,0)           \
    X(Vert,   0,   255, 0  )    \
    X(Bleu,   0,   0,   255)    \
    X(Jaune,  255, 255, 0  )



enum class Couleur {
    #define X(nom, r, g, b) nom,
    LISTE_COULEURS
    #undef X
};


inline const char* nomsCouleurs[] = {
#define X(nom, r, g, b) #nom,
    LISTE_COULEURS
    #undef X
};

// Générer les valeurs RGB automatiquement
struct RGB { int r, g, b; };
inline RGB valeursRGB[] = {
    #define X(nom, r, g, b) {r, g, b},
        LISTE_COULEURS
    #undef X
};




#define STATE_LIST \
X(IDLE, En attente) \
X(RUNNING, EN cours) \
X(STOPPED, Arreté) \
X(ERROR, Erreur fatale)


enum class Etat {
    #define X(id, label) id,
    STATE_LIST
    #undef X
};

inline const char* get_label(const Etat e) {
    switch (e) {
        #define X(id, label) case Etat::id : return #label;
            STATE_LIST
        #undef X
        default:
            return "Inconnu";
    }
}



// Exemple avancé

// Macro de débogage complète
#ifdef DEBUG
    #define DBG(msg) \
        std::clog << "[DBG] " << __FILE__ << ":" << __LINE__ \
            << " (" << __func__ << ") " << (msg) << std::endl

    #define ASSERT(condition, msg) \
        if (!(condition)) { std::cerr << "[ASSERT FAIL] " << #condition << " → " << (msg) \
            << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
            std::terminate(); \
        }
#else
    #define DBG(msg)
    #define ASSERT(condition, msg)
#endif


inline int diviser(const int a, const int b) {
    ASSERT(b != 0, "Division par zéro !");
    DBG("diviser(" + std::to_string(a) + ", " + std::to_string(b) + ")");
    return a / b;
}

inline void x_macro_use() {
    DBG("Démarrage");
    std::cout << diviser(10, 2) << std::endl;  // 5
    // diviser(10, 0);  // ← ASSERT échoue en debug !
}



#endif //TUTO_CPP_ADVANCED_PREPROCESSING_H
