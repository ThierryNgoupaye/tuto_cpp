//
// Created by thierry on 06/05/2026.
//

#ifndef TUTO_CPP_CONTAINERS_H
#define TUTO_CPP_CONTAINERS_H

#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <map>
#include <deque>
#include <unordered_map>
#include <set>
#include <unordered_set>

// STL = Standard Template Library


struct Personne {
    std::string nom;
    int age;

    Personne(std::string n, const int a): nom(std::move(n)), age(a) {
        std::cout << "Construit : " << nom << std::endl;
    }
};

// vector
inline void vector_use() {

    // Création
    std::vector<int> v1;                    // vide
    std::vector<int> v2(5);                 // 5 zéros
    std::vector<int> v3(5, 42);             // 5 fois 42
    std::vector<int> v4 = {1, 2, 3, 4, 5}; // liste init

    std::vector<int> v;
    std::cout << "size     : " << v.size()     << std::endl; // 0
    std::cout << "capacity : " << v.capacity() << std::endl; // 0

    v.push_back(1);
    std::cout << "size     : " << v.size()     << std::endl; // 1
    std::cout << "capacity : " << v.capacity() << std::endl; // 1


    //  Sans reserve → réallocations multiples !
    std::vector<int> _v1;
    for (int i = 0; i < 1000000; i++) {
        _v1.push_back(i);  // ~20 réallocations !
    }

    // ✅ Avec reserve → une seule allocation !
    std::vector<int> _v2;
    v2.reserve(1000000);  // alloue d'un coup
    for (int i = 0; i < 1000000; i++) {
        _v2.push_back(i);  // jamais de réallocation
    }

    // push_back vs emplace_back
    std::vector<Personne> personnes;
    personnes.reserve(3);

    // push_back → construit PUIS copie/move dans le vector
    Personne p("Alice", 25);
    personnes.push_back(p);              // copie
    personnes.push_back(std::move(p));   // move

    // emplace_back → construit DIRECTEMENT dans le vector !
    personnes.emplace_back("Bob", 30);   // ← construit sur place
    // pas de copie, pas de move → optimal !



    std::vector<int> _v = {1, 2, 3, 4, 5};

    auto it = v.begin();  // itérateur vers 1

    // DANGER — push_back peut invalider les itérateurs !
    v.push_back(6);  // Si réallocation → it devient invalide !
    // *it → undefined behavior !

    // Solution — ne pas garder d'itérateur si on modifie
    // OU utiliser l'index
    constexpr std::size_t index = 0;
    v.push_back(7);
    std::cout << v[index] << std::endl;  //toujours valide

    // Règles d'invalidation :
    // push_back → invalide TOUT si réallocation
    // insert au milieu → invalide les itérateurs après le point d'insertion
    // erase → invalide les itérateurs après le point de suppression
}



// array
inline void array_use() {
    // Taille fixe, connue à la compilation
    std::array<int, 5> arr = {3, 1, 4, 1, 5};

    // Mêmes méthodes que vector !
    std::cout << arr.size()  << std::endl;  // 5
    std::cout << arr.front() << std::endl;  // 3
    std::cout << arr.back()  << std::endl;  // 5
    std::cout << arr[2]      << std::endl;  // 4

    // Avantages vs tableau C brut
    // int tab[5] = {3,1,4,1,5};
    // → pas de size(), pas d'itérateurs, pas de bounds checking

    // Trié avec algorithme STL !
    std::sort(arr.begin(), arr.end());

    for (const auto x : arr) std::cout << x << " ";
    std::cout << std::endl;  // 1 1 3 4 5

    // Sur la stack → zéro allocation heap
    std::cout << sizeof(arr) << std::endl;  // 20 (5 × 4 octets)
}


// std::map et unordered_map
inline void map_and_unordered_map_use() {

    // std::map → trié par clé, O(log n)
    std::map<std::string, int> ages;
    ages["Alice"] = 25;
    ages["Bob"]   = 30;
    ages["Charlie"] = 28;


    // Toujours trié alphabétiquement !
    for (const auto& [nom, age] : ages) {
        std::cout << nom << " : " << age << std::endl;
    }

    // Chercher → O(log n)
    if (const auto it = ages.find("Bob"); it != ages.end()) {
        std::cout << "Trouvé : " << it->second << std::endl;
    }

    // std::unordered_map → non trié, O(1) moyen
    std::unordered_map<std::string, int> scores;
    scores["Alice"] = 100;
    scores["Bob"]   = 85;
    scores["Charlie"] = 92;

    // Chercher → O(1) moyen !
    std::cout << scores["Alice"] << std::endl;

    // Vérifier existence sans créer
    if (scores.count("Bob") > 0) {
        std::cout << "Bob existe !" << std::endl;
    }

    // C++20 → contains() plus lisible
    if (scores.contains("Charlie")) {
        std::cout << "Charlie existe !" << std::endl;
    }
}



// set
inline void set_and_unordered_set_use() {

    // set → trié, sans doublons, O(log n)
    std::set<int> s = {5, 3, 1, 4, 1, 5, 9, 2, 6};
    // → doublons supprimés, trié automatiquement !

    for (const int x : s) std::cout << x << " ";
    std::cout << std::endl;

    s.insert(7);
    s.erase(3);

    std::cout << s.count(5) << std::endl;  // 1 (existe)
    std::cout << s.count(3) << std::endl;  // 0 (supprimé)

    // unordered_set → non trié, O(1) moyen
    std::unordered_set<std::string> noms = {"Alice", "Bob", "Charlie"};
    noms.insert("David");

    if (noms.contains("Alice")) {
        std::cout << "Alice est dans l'ensemble" << std::endl;
    }

}

#endif //TUTO_CPP_CONTAINERS_H
