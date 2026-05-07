//
// Created by thierry on 07/05/2026.
//

#ifndef TUTO_CPP_STL_ALGORITHMS_H
#define TUTO_CPP_STL_ALGORITHMS_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <numeric>



// Algorithmes non modifiants (juste lecture)
inline void readonly_algo() {

    std::vector vect = {3,1,6,9,10,65,0,2,4,5,100};

    // compter le nombre d'occurrences
    std::cout << std::count(vect.begin(), vect.end(), 5) << std::endl;

    // compter le nombre d'occurrences à une condition
    std::cout << std::count_if(vect.begin(), vect.end(), [](const int a){ return a >5; });  // compte le nombre d'éléments supérieurs à 5.

    // trouver un élement
    if (const auto it = std::find(vect.begin(), vect.end(), 100); it != vect.end()) {
        std::cout << "L'élement " << *it  << "a été trouvé à l'index " << std::distance(vect.begin(), it) << std::endl;
    }

    // trouver un élément selon une condition
    auto it = std::find_if(vect.begin(), vect.end(), [](const int a){ return a % 2 == 0;});


    // tous satisfont la condition  (vérifie que le tableau est trié dans l'ordre croissant.)
    const bool estTrie = std::all_of(vect.begin(), vect.end(), [](const int a){ return a < a+1;});
    std::cout << "Tableau trié dans l'ordre croissant : " << (estTrie ? "Oui" : "Non")  << std::endl;

    // au moins un satisfait la condition
    const bool unNegatif = std::any_of(vect.begin(), vect.end(), [](const int a){ return a < 0; });

    // aucun ne satisfait la condition
    const bool aucunNegatif = std::none_of(vect.begin(), vect.end(), [](const int a){ return a < 0;});

    // appliquer une fonction à chaque élement
    std::for_each(vect.begin(), vect.end(),
        [](const int a) {
            std::cout << a * a << " ";
        }
    );
    std::cout << std::endl;


    const auto minIt = std::min_element(vect.begin(), vect.end());
    const auto maxIt = std::max_element(vect.begin(), vect.end());
    std::cout << "Min : " << *minIt << std::endl;  // 0
    std::cout << "Max : " << *maxIt << std::endl;  //  100
}



// algorithmes modifiants
inline void read_write_algo() {

    std::vector v = {1,2,3,4};
    std::vector<int> doubles(v.size());

    // remplit le tableau doubles avec les élements de v multipliés par 2.
    std::transform(v.begin(), v.end(), doubles.begin(),
        [](const int x) {
            return x * 2 ;
        }
    );


    // transform avec deux sources
    std::vector<int> sommes(v.size());
    std::transform(v.begin(), v.end(), doubles.begin(), sommes.begin(),
        [](const int a, const int b) {
            return a + b;
        }
    );   // sommes = {3,6,8,12,15}


    // remplir avec une valeur
    std::vector<int> zeros(5);
    std::fill(zeros.begin(), zeros.end(), 0);


    // remplir N élements
    std::fill(zeros.begin(), zeros.begin() +2 , 42);

    // remplir avec une fonction
    std::vector<int> suite(5);
    int compteur = 0;
    std::generate(suite.begin(), suite.end(),
        [&compteur]() {
            return compteur++;
        }
    );


    // std::replace → remplacer une valeur
    std::vector r = {1, 2, 3, 2, 1};
    std::replace(r.begin(), r.end(), 2, 99);


    // std::replace_if → remplacer selon condition
    std::replace_if(r.begin(), r.end(),
        [](const int x) { return x > 50; }, 0);


    // std::remove → "supprimer" (ne réduit pas la taille !)
    std::vector v2 = {1, 2, 3, 2, 1, 2};
    const auto newEnd = std::remove(v2.begin(), v2.end(), 2);
    // v2 = {1, 3, 1, ?, ?, ?} → éléments après newEnd indéfinis !
    v2.erase(newEnd, v2.end());  // ← erase-remove idiom !
    // v2 = {1, 3, 1}

    // std::unique → supprimer les doublons CONSÉCUTIFS
    std::vector  v3 = {1, 1, 2, 3, 3, 3, 4};
    const auto newEnd2 = std::unique(v3.begin(), v3.end());
    v3.erase(newEnd2, v3.end());
    // v3 = {1, 2, 3, 4} ✅

    // std::reverse → inverser
    std::vector v4 = {1, 2, 3, 4, 5};
    std::reverse(v4.begin(), v4.end());
    // v4 = {5, 4, 3, 2, 1}

    // Affichage
    for (const auto x : doubles) std::cout << x << " ";
    std::cout << std::endl;

    for (const auto x : v2) std::cout << x << " ";
    std::cout << std::endl;

    for (const auto x : v3) std::cout << x << " ";
    std::cout << std::endl;
}


// Algorithmes de copie
inline void copy_algo_use() {
    std::vector source = {1, 2, 3, 4, 5, 6, 7, 8};

    // std::copy → copie tous les éléments
    std::vector<int> dest1(source.size());
    std::copy(source.begin(), source.end(), dest1.begin());

    // std::copy_if → copie selon condition
    std::vector<int> pairs;
    std::copy_if(source.begin(), source.end(),
                 std::back_inserter(pairs),
                 [](const int x) { return x % 2 == 0; }
    );
    // pairs = {2, 4, 6, 8}

    // std::copy_n → copie N éléments
    std::vector<int> premiers3(3);
    std::copy_n(source.begin(), 3, premiers3.begin());
    // premiers3 = {1, 2, 3}

    // std::move (algorithme, pas std::move !) → move les éléments
    std::vector<std::string> src = {"Alice", "Bob", "Charlie"};
    std::vector<std::string> dst(3);
    std::move(src.begin(), src.end(), dst.begin());
    // dst = {"Alice", "Bob", "Charlie"}
    // src = {"", "", ""} → pillé !

    for (const auto x : pairs) std::cout << x << " ";
    std::cout << std::endl;  // 2 4 6 8

    for (const auto& s : dst) std::cout << s << " ";
    std::cout << std::endl;  // Alice Bob Charlie
}


// Algorithmes numériques
inline void numerical_algo_use() {
    std::vector v = {1, 2, 3, 4, 5};

    // std::accumulate → somme (ou opération custom)
    const int somme = std::accumulate(v.begin(), v.end(), 0);
    std::cout << somme << std::endl;  // 15

    // Avec opération custom → produit
    const int produit = std::accumulate(v.begin(), v.end(), 1,
        [](const int acc, const int x) { return acc * x; }
    );
    std::cout << produit << std::endl;  // 120


    // std::reduce → comme accumulate mais parallélisable
    const int somme2 = std::reduce(v.begin(), v.end(), 0);
    std::cout << somme2 << std::endl;  // 15

    // std::inner_product (produit scalaire)
    std::vector v2 = {1, 2, 3, 4, 5};
    const int dot = std::inner_product(
        v.begin(), v.end(),
        v2.begin(), 0);
    std::cout << dot << std::endl;  // 55 (1+4+9+16+25)


    // std::partial_sum → sommes partielles
    std::vector<int> sommesPartielles(v.size());
    std::partial_sum(v.begin(), v.end(),
                     sommesPartielles.begin());
    // {1, 3, 6, 10, 15}

    for (const auto x : sommesPartielles)
        std::cout << x << " ";
    std::cout << std::endl;


    // std::iota (remplir avec valeurs croissantes)
    std::vector<int> suite(10);
    std::iota(suite.begin(), suite.end(), 0);
    // {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}

    for (const auto x : suite) std::cout << x << " ";
    std::cout << std::endl;
}



// Algorithmes de partition
inline void partition_algo_use() {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8};

    // std::partition (sépare en deux groupes)
    const auto pivot = std::partition(v.begin(), v.end(),
        [](const int x) { return x % 2 == 0; });  // pairs d'abord
    // v = {8, 2, 6, 4, 5, 3, 7, 1} (ordre non garanti)


    std::cout << "Pairs : ";
    for (auto it = v.begin(); it != pivot; ++it)
        std::cout << *it << " ";
    std::cout << std::endl;

    std::cout << "Impairs : ";
    for (auto it = pivot; it != v.end(); ++it)
        std::cout << *it << " ";
    std::cout << std::endl;



    // std::stable_partition (préserve l'ordre)
    std::vector v2 = {1, 2, 3, 4, 5, 6, 7, 8};
    std::stable_partition(v2.begin(), v2.end(),
        [](const int x) { return x % 2 == 0; });
    // v2 = {2, 4, 6, 8, 1, 3, 5, 7} ← ordre préservé !

    for (const auto x : v2) std::cout << x << " ";
    std::cout << std::endl;

    // std::is_partitioned → vérifie si partitionné
    std::cout << std::is_partitioned(v2.begin(), v2.end(),
        [](const int x) { return x % 2 == 0; })
              << std::endl;  // 1

}




// Cas pratique

struct Produit {
    std::string nom;
    double prix;
    int quantite;
};

inline void algorithm_practical_use() {
    std::vector<Produit> inventaire = {
        {"Pomme",   0.5,  100},
        {"Banane",  0.3,  50},
        {"Orange",  0.8,  75},
        {"Mangue",  1.5,  30},
        {"Raisin",  2.0,  20},
    };

    // 1. Trier par prix croissant
    std::sort(inventaire.begin(), inventaire.end(),
        [](const Produit& a, const Produit& b) {
            return a.prix < b.prix;
        });

    // 2. Garder seulement les produits > 0.5€
    std::vector<Produit> chersInventaire;
    std::copy_if(inventaire.begin(), inventaire.end(),
                 std::back_inserter(chersInventaire),
                 [](const Produit& p) { return p.prix > 0.5; });


    // 3. Calculer la valeur totale de l'inventaire
    const double valeurTotale = std::accumulate(
        inventaire.begin(), inventaire.end(), 0.0,
        [](const double acc, const Produit& p) {
            return acc + p.prix * p.quantite;
        }
    );


    // 4. Trouver le produit le plus cher
    const auto maxIt = std::max_element(inventaire.begin(), inventaire.end(),
        [](const Produit& a, const Produit& b) {
            return a.prix < b.prix;
        }
    );

    // Affichage
    std::cout << "=== Inventaire trié ===" << std::endl;
    std::for_each(inventaire.begin(), inventaire.end(),
        [](const Produit& p) {
            std::cout << p.nom << " : " << p.prix
                      << "€ × " << p.quantite << std::endl;
        }
    );

    std::cout << "\nValeur totale : " << valeurTotale << "€" << std::endl;
    std::cout << "Produit le + cher : " << maxIt->nom << std::endl;

    std::cout << "\nProduits > 0.5€ :" << std::endl;
    std::for_each(chersInventaire.begin(), chersInventaire.end(),
        [](const Produit& p) {
            std::cout << "  " << p.nom << std::endl;
        }
    );

}


#endif //TUTO_CPP_STL_ALGORITHMS_H
