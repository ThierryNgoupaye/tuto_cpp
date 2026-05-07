//
// Created by thierry on 07/05/2026.
//

#ifndef TUTO_CPP_RANGES_H
#define TUTO_CPP_RANGES_H

#include <iostream>
#include <vector>
#include <ranges>
#include <algorithm>
#include <pstl/glue_execution_defs.h>

// Versions améliorés des algorithmes de la STL avec ranges

inline void involved_ranges_algo_use() {
    std::vector v = {5, 3, 1, 4, 2};

    // Plus besoin de begin()/end() !
    std::ranges::sort(v);
    // v = {1, 2, 3, 4, 5}

    // Avec comparateur
    std::ranges::sort(v, std::greater<int>{});
    // v = {5, 4, 3, 2, 1}

    // find
    if (const auto it = std::ranges::find(v, 3); it != v.end()) {
        std::cout << "Trouvé : " << *it << std::endl;
    }

    // count_if
    const auto nb = std::ranges::count_if(v,
        [](const int x) { return x > 2; });
    std::cout << "> 2 : " << nb << std::endl;  // 3


    // all_of, any_of, none_of
    std::cout << std::ranges::all_of(v,
        [](const int x) { return x > 0; }) << std::endl;  // 1


    // Avantage supplémentaire → projections !
    struct Personne {
        std::string nom;
        int age;
    };

    std::vector<Personne> personnes = {
        {"Alice", 30},
        {"Bob", 25},
        {"Charlie", 28}
    };

    // Trier par age sans lambda complexe !
    std::ranges::sort(personnes, {}, &Personne::age); // trie selon person.age directement

    for (const auto&[nom, age] : personnes) {
        std::cout << nom << " : " << age << std::endl;
    }
    // Bob : 25
    // Charlie : 28
    // Alice : 30
}



// Les projections

struct Produit {
    std::string nom;
    double prix;
    int stock;
};

inline void projection_ranges_algo_use() {

    std::vector<Produit> produits = {
        {"Pomme",  0.5, 100},
        {"Banane", 0.3, 50},
        {"Orange", 0.8, 75},
    };


    // Classique → lambda verbeuse
    std::sort(produits.begin(), produits.end(),
        [](const Produit& a, const Produit& b) {
            return a.prix < b.prix;
        });
    // Ranges + projection → élégant !
    std::ranges::sort(produits, {}, &Produit::prix);


    // find_if avec projection
    const auto it = std::ranges::find_if(produits,[](const double prix) { return prix > 0.5; }, &Produit::prix);  // compare les prix directement !

    if (it != produits.end()) {
        std::cout << "Premier > 0.5€ : " << it->nom << std::endl;
    }


    // min/max avec projection
    const auto maxPrix = std::ranges::max_element(produits,{}, &Produit::prix);
    std::cout << "Plus cher : " << maxPrix->nom << std::endl;
}



// Les Views
inline void view_range_use() {
    std::vector v = {5, 3, 1, 4, 2};

    auto pairs = v | std::views::filter([](const int x){ return x % 2 == 0;});

    std::cout << "Pairs : ";
    for (const auto x : pairs) std::cout << x << " ";
    std::cout << std::endl;
    // v non modifié


    // transformation
    auto doubles = v | std::views::transform([](const int x){return 2 * x;});
    for (const int x : doubles) std::cout << x << " ";
    std::cout << std::endl;

    // récupère les N premiers éléments
    auto premiers5 = v | std::views::take(5);
    for (const int x : premiers5) std::cout << x << " ";
    std::cout << std::endl;


    //ignore les N premiers élemnts
    auto sans3 = v | std::views::drop(3);
    for (const int x : sans3) std::cout << x << " ";
    std::cout << std::endl;


    // inverse l'ordre
    auto inverse = v | std::views::reverse;
    for (const int x : inverse) std::cout << x << " ";
    std::cout << std::endl;
}



// Pipelines avec |
inline void pipeline_ranges_use() {
    std::vector v = {5, 3, 1, 4, 2};

    std::ranges::sort(v);

    auto resultat = v | std::views::filter([](const int x) { return x % 2 == 0;})
                                        | std::views::take(3)
                                        | std::views::transform([](const int x) { return x * x;});

    std::cout << "Carrés des 3 premiers pairs : ";
    for (const int x : resultat) std::cout << x << " ";
    std::cout << std::endl;


    std::vector<std::string> mots = {"bonjour", "monde", "c++", "ranges", "super", "cool"};

     auto longs = mots
        | std::views::filter([](const std::string& s) { return s.size() > 4;})
        | std::views::transform([](const std::string& s) {
                std::string upper  = s;
                std::ranges::transform(upper, upper.begin(), ::toupper);
            return upper;
    });

    for (const auto& s : longs) std::cout << s << " ";
    std::cout << std::endl;



    // Matérialiser une vue manuelle, en utilisant les itérateurs de la vue
    std::vector material_view(longs.begin(), longs.end());

    std::vector<std::string> material_view_b;
    std::ranges::copy(longs, std::back_inserter(material_view_b));  // en copiant
}




#endif //TUTO_CPP_RANGES_H
