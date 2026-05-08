//
// Created by thierry on 07/05/2026.
//

#ifndef TUTO_CPP_UTILITAIRES_STL_H
#define TUTO_CPP_UTILITAIRES_STL_H

#include <iostream>
#include <vector>
#include  <string>
#include <optional>
#include <variant>
#include <any>
#include <span>
#include <expected>



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




template<typename ... Ts>
struct overloaded : Ts ... {using Ts::operator()...; };


// Variant (union typée et sûre)
inline void variant_use() {
    std::variant<int, double, std::string> v = 42;
    std::cout << std::get<int>(v) << std::endl;

    v = 3.14;

    std::cout << std::get<double>(v) << std::endl;

    v = std::string("bonjour");
    std::cout << std::get<std::string>(v) << std::endl;

    std::cout<< v.index() << std::endl;  // vérifier le type actif

    std::cout<< std::holds_alternative<std::string>(v) << std::endl;
    std::cout<< std::holds_alternative<int>(v) << std::endl;


    if (const auto ptr = std::get_if<std::string>(&v)) {
        std::cout << "String : " << *ptr << std::endl;
    }

    using Valeur = std::variant<int, double, std::string, bool>;

    const std::vector<Valeur> valeurs = {42,3.14,std::string("bonjour"), true};

    for (const auto& val : valeurs) {
        std::visit([]<typename K>(const K& visitor) {

            using T = std::decay_t<K>;

            if constexpr (std::is_same_v<T, int>) {
               std::cout << "int : " << visitor << std::endl;
            }
            else if constexpr (std::is_same_v<T, double>) {
               std::cout << "double : " << visitor  << std::endl;
            }
            else if constexpr (std::is_same_v<T, std::string>) {
               std::cout << "String : " << visitor << std::endl;
            }
            else if constexpr (std::is_same_v<T, bool>) {
               std::cout << "bool : " << (visitor ? "true" : "false") << std::endl;
            }
        }, val);
    }


    // alternative plus simple
    auto visiteur = overloaded{
        [](const int vi) { std::cout << "int :" << vi << std::endl; },
        [](const double vi){ std::cout << "double : " << vi << std::endl; },
        [](const string& vi){ std::cout << "string : " << vi << std::endl; },
        [](const bool vi){ std::cout << "bool : " << (vi ? "true" : "false") << std::endl ; },
    };


    for (const auto& val : valeurs) {
        std::visit(visiteur, val);
    }
}



// std::any
inline void any_use() {
    std::any a = 45;

    std::cout << std::any_cast<int>(a) << std::endl;   // retourne 45;

    a = std::string("bonjour");
    std::cout << std::any_cast<std::string>(a) << std::endl;

    a = 4.15f;
    std::cout << std::any_cast<float>(a) << std::endl;

    // verification du type
    std::cout << a.type().name() << std::endl;  // d (double)
    std::cout << a.has_value() << std::endl;   // 1


    // mauvais cast
    try {
        std::any_cast<int>(a);   // a contient un float
    }
    catch (const std::bad_any_cast& e) {
        std::cout << "Mauvais cast. " << e.what() << std::endl;
    }

    //reinitialiser
    a.reset();
    std::cout << a.has_value() << std::endl;   // 0, car rien dans a.
}



// string view (vue sur une string)
inline void afficher_stv(std::string_view sv) {
    std::cout << sv << std::endl;
    std::cout << sv.size() << std::endl;
    std::cout << sv.substr(0,3) << std::endl;
}


inline void string_view_use() {
    const std::string s = "string";
    const char* c = "hello world";

    afficher_stv(s);   // string -> string_view, zéro copie
    afficher_stv(c);     // const char* → string_view, zero copie
    afficher_stv("!!");    // littéral → string_view, zéro copie

    //string_view -> sous-chaine sans copie !
    const std::string_view sv = s;
    const std::string_view sous = sv.substr(0, 7);

    std::cout << sous << std::endl;

    std::string_view dangereux;
    {
        const std::string temp = "temporaire";
        dangereux = temp;  // dangereux
    }
}



//vue sur un tableau
inline void afficher_vue_tableau(std::span<int> s) {
    for (const int x : s) std::cout << x << std::endl;
    std::cout << std::endl;
    std::cout << "Taille : " << s.size() << std::endl;
}


inline void doubler_(std::span<int> s) {
    for (int& x : s) x *=2;
}


inline void span_use() {
    std::vector v = {1,2,3,4,5};
    afficher_vue_tableau(v);


    std::array a = {10,11,12,13,14,15};
    afficher_vue_tableau(a);

    int tab[] = {1,2,3,4,5};
    afficher_vue_tableau(tab);

    // sous-span => vue sur une partie
    const std::span sous_span(v);
    const auto premiers3 = sous_span.first(3);
    const auto derniers2 = sous_span.last(2);
    auto milieu = sous_span.subspan(1,3);


    afficher_vue_tableau(premiers3);
    afficher_vue_tableau(derniers2);

    // span de taille fixe (compile-time)
    std::span<int, 5> fixe(v);  // taille conne à la compilation


    // Modifier via span
    doubler_(v);
    afficher_vue_tableau(v);
}



// std::expected
enum class ErreurDivision {
    DivisionParZero,
    DepassementCapacite
};


// expected retourne un résultat ou une erreur
std::expected<double, ErreurDivision> inline diviser_(const double a, const double b) {
    if (b == 0) {
        return std::unexpected(ErreurDivision::DivisionParZero);
    }
    return a / b;  // succès
}


inline std::expected<int, std::string> parse_entier_(const std::string& s) {
    try {
        return std::stoi(s);
    } catch (...) {
        return std::unexpected("Impossible de parser : " + s);
    }
}


inline void expected_key_word_use() {

    const auto result = diviser_(10.0, 2.0);
    if (result) {
        std::cout << "Résultat : " << *result << std::endl;
    }

    // cas d'erreur
    auto result2 = diviser_(10.0, 0.0);
    if (!result) {
        switch (result2.error()) {
            case ErreurDivision::DivisionParZero:
                std::cout << "Division par zéro !" << std::endl;
                break;
            default:
                std::cout << "Erreur inconnue" << std::endl;
        }
    }


    // value_or (valeur par defaut en cas d'erreur)
    std::cout << result2.value_or(-1) << std::endl;
}




// tuple avancés
inline void advanced_tuple_use() {
    const auto t = std::make_tuple(42,3.14,std::string("bonjour"));

    // Accès par index
    std::cout << std::get<0>(t) << std::endl;  // 42
    std::cout << std::get<1>(t) << std::endl;  // 3.14
    std::cout << std::get<2>(t) << std::endl;  // bonjour

    // Accès par type (si unique !)
    std::cout << std::get<int>(t)    << std::endl;  // 42
    std::cout << std::get<double>(t) << std::endl;  // 3.14

    // Structured bindings C++17 → destructuration !
    auto [entier, flottant, texte] = t;
    std::cout << entier   << std::endl;  // 42
    std::cout << flottant << std::endl;  // 3.14
    std::cout << texte    << std::endl;  // bonjour


    // std::tie → lier à des variables existantes
    int    i;
    double d;
    std::string s;
    std::tie(i, d, s) = t;
    std::cout << i << " " << d << " " << s << std::endl;


    // std::ignore → ignorer certains éléments
    std::tie(i, std::ignore, s) = t;  // ignore le double


    // tuple_size → nombre d'éléments
    std::cout << std::tuple_size_v<decltype(t)> << std::endl;  // 3


    // tuple_cat → concaténer des tuples
    auto t1 = std::make_tuple(1, 2);
    auto t2 = std::make_tuple("hello", 3.14);
    const auto t3 = std::tuple_cat(t1, t2);
    // t3 = (1, 2, "hello", 3.14)
    std::cout << std::get<2>(t3) << std::endl;  // hello


    // Fonction retournant plusieurs valeurs !
    auto div_mod = [](const int a, const int b) {
        return std::make_tuple(a/b, a%b);
    };

    auto [quotient, reste] = div_mod(17, 5);
    std::cout << "17/5 = " << quotient<< " reste " << reste << std::endl;  // 3 reste 2



   

}






#endif //TUTO_CPP_UTILITAIRES_STL_H
