//
// Created by thierry on 02/05/2026.
//

#ifndef TUTO_CPP_LINKED_LIST_H
#define TUTO_CPP_LINKED_LIST_H

#include  <iostream>
#include <list>
#include <algorithm>
#include <vector>
#include <string>
#include  <array>
#include <tuple>



using std::list;
using std::vector;
using std::array;
using std::ranges::sort;
using std::tuple;

using string = std::string;


inline void sort_container() {

    vector vec = {8,5,4,7,2};

    vector v_string = {"orange", "pomme", "ananas", "melon", "pasteque", "pamplemousse"};

    // fonction de tri (possibilité une fonction décrivant comment se passera le tri)
    sort(vec.begin(), vec.end(), [](const int a, const int b) { return a > b; });

    sort(v_string.begin(), v_string.end(), [](const string& m, const string& n){ return m.length() < n.length();});

    for (const string& i : v_string) {
        std::cout << i << std::endl;
    }
}


inline void list_use() {
    std::list<int> liste;
    std::list liste2 = {12,43,4,98};

    // insere un élement à la fin de la liste sans faire de copie
    liste.emplace_back(60);

    // insere un element a la position désirée
    liste.insert(liste.begin(), 89);

    // retourne le dernier élement de la liste
    std::cout <<liste.back() << std::endl;

    // insere un élement au debut de la liste
    liste.emplace_front(100);

    // logique ...
    std::cout << liste.empty() << std::endl;

    // affiche le premier élement de la liste
    std::cout << liste.front();

    // retire l'élement qui se trouve à la fin de la liste
    liste.pop_back();

    // retire le premier élement de la liste
    liste.pop_front();

    //retire l'élément de la liste à la position souhaitée
    const auto it = liste.begin();
    liste.erase(it);

    // range dans l'ordre croissant
    liste.sort();

    // move semantique (liste pointe vers le contenu de liste 2 et liste 2 devient zombie)
    liste.swap(liste2);

    for (const auto& l : liste) {
        std::cout<< l << std::endl ;
    }
}



inline void tuple_use() {

    auto my_tuple = std::make_tuple(12, 'd', string("mon tuple"));

    std::get<2>(my_tuple) ="Thierry Ngoupaye";

    std::cout << std::get<2>(my_tuple) << std::endl;


  


}






#endif //TUTO_CPP_LINKED_LIST_H
