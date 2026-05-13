//
// Created by thierry on 14/05/2026.
//

#ifndef TUTO_CPP_CPP_23_H
#define TUTO_CPP_CPP_23_H


#include <iostream>
#include <string>
#include <print>
#include <cmath>
#include <list>
#include <vector>
#include <ranges>
//#include <mdspan>
//#include <flat_map>
//#include <flat_set>



class Base23 {
    protected:
        std::string m_nom;

    public:
        auto& setNom(this auto& self, std::string nom) {
            self.m_nom = std::move(nom);
            return self;
        }
};


class Derivee23 : public Base23 {

    int m_age{0};
    public:
        auto& setAge(this auto& self, const int age) {
            self.m_age = age;
            return self;
        }

        void afficher() const {
            std::println("{} ({})", m_nom, m_age);
        }
};


inline void deducting_this_use() {
    Derivee23 d;
    d.setNom("Alice").setAge(30).afficher();

}



// inline void flat_map_flat_set_use() {
//     // ─────────────────────────────────────
//     // flat_map → interface identique à map !
//     // ─────────────────────────────────────
//     std::flat_map<std::string, int> scores;
//
//     scores["Alice"]   = 100;
//     scores["Bob"]     = 85;
//     scores["Charlie"] = 92;
//
//     // Même API que std::map
//     for (auto& [nom, score] : scores) {
//         std::println("{} : {}", nom, score);
//     }
//     // Alice : 100
//     // Bob : 85
//     // Charlie : 92
//
//     // find, contains, insert... → identiques !
//     if (auto it = scores.find("Bob"); it != scores.end()) {
//         std::println("Bob trouve : {}", it->second);
//     }
//
//     std::println("Alice existe : {}", scores.contains("Alice"));
//
//     // ─────────────────────────────────────
//     // flat_set → interface identique à set !
//     // ─────────────────────────────────────
//     std::flat_set<int> nombres = {5, 3, 1, 4, 2, 3, 1};
//     // doublons supprimés, trié automatiquement
//
//     for (int n : nombres) std::print("{} ", n);
//     std::println("");
//     // 1 2 3 4 5
//
//     // ─────────────────────────────────────
//     // Initialisation depuis un vecteur trié
//     // ─────────────────────────────────────
//     std::vector<std::pair<int, std::string>> donnees = {
//         {1, "un"}, {2, "deux"}, {3, "trois"}
//     };
//
//     std::flat_map<int, std::string> fm(
//         std::sorted_unique,  // ← indique que les données sont déjà triées
//         donnees.begin(), donnees.end()
//     );  // → construction O(n) au lieu de O(n log n) !
//
//     for (auto& [k, v] : fm) {
//         std::println("{} → {}", k, v);
//     }
// }


// std::flat_map  :
// → beaucoup de lectures, peu d'insertions
// → map construite une fois puis consultée
// → petite à moyenne taille (< 1000 éléments)
// → performances de cache critiques
//
// std::map  :
// → insertions/suppressions fréquentes
// → très grande taille
// → itérateurs qui ne doivent pas être invalidés
//    (flat_map invalide TOUS les itérateurs à chaque insertion !)



constexpr double racineCarree(const double x) {
    if consteval {
        // Contexte compile-time → algorithme exact
        // (pas accès à std::sqrt à la compilation)
        double r = x;
        for (int i = 0; i < 100; i++) {
            r = (r + x/r) / 2.0;  // méthode de Newton
        }
        return r;
    }
    // Contexte runtime → std::sqrt optimisé !
    return std::sqrt(x);
}

inline void if_consteval_use() {
    // Compile-time → branche if consteval
    constexpr double r1 = racineCarree(2.0);
    std::println("{}", r1);  // 1.41421...

    // Runtime → branche else (std::sqrt)
    double x = 2.0;
    double r2 = racineCarree(x);
    std::println("{}", r2);  // 1.41421...
}


// inline void mdspan_use() {
//     std::vector data = {
//         1, 2, 3,
//         4, 5, 6,
//         7, 8, 9
//     };
//
//     // Vue 2D (3×3) sur ce tableau → zéro copie !
//     std::mdspan<int, std::extents<size_t, 3, 3>> mat(data.data());
//     //                              ↑ dimensions fixes compile-time
//
//     // Accès par [ligne][colonne]
//     std::println("mat[0][0] = {}", mat[0, 0]);  // 1
//     std::println("mat[1][2] = {}", mat[1, 2]);  // 6
//     std::println("mat[2][1] = {}", mat[2, 1]);  // 8
//
//     // Modifier via mdspan → modifie le vecteur original !
//     mat[1, 1] = 99;
//     std::println("data[4] = {}", data[4]);  // 99 ✅
//
//     // Afficher la matrice
//     for (size_t i = 0; i < 3; i++) {
//         for (size_t j = 0; j < 3; j++) {
//             std::print("{:3} ", mat[i, j]);
//         }
//         std::println("");
//     }
//
//     // ─────────────────────────────────────
//     // Dimensions dynamiques (runtime)
//     // ─────────────────────────────────────
//     int largeur = 1920, hauteur = 1080;
//     std::vector<uint8_t> imageData(largeur * hauteur * 3);
//
//     // Vue 3D sur l'image (hauteur × largeur × canaux)
//     std::mdspan<uint8_t,
//         std::dextents<size_t, 3>> image(
//             imageData.data(),
//             hauteur, largeur, 3);
//
//     // Accéder au pixel (y=100, x=200, canal R)
//     image[100, 200, 0] = 255;  // Rouge = 255 ✅
//
//     std::println("Image {}x{} créée", largeur, hauteur);
// }



inline void ranges_to_use() {
    std::vector<int> v = {1, 2, 3, 4, 5, 6};

    // Avant C++23 → verbeux
    auto pairs = v | std::views::filter([](const int x) { return x%2==0; });
    std::vector resultat(pairs.begin(), pairs.end());

    // C++23 → élégant !
    auto resultat2 = v
        | std::views::filter([](const int x) { return x % 2 == 0; })
        | std::ranges::to<std::vector>();  // 

    // Vers n'importe quel conteneur !
    auto liste = v
        | std::views::take(3)
        | std::ranges::to<std::list>();
}



#endif //TUTO_CPP_CPP_23_H
