//
// Created by thierry on 06/05/2026.
//

#ifndef TUTO_CPP_METAPROGRAMMING_H
#define TUTO_CPP_METAPROGRAMMING_H


#include <iostream>
#include <type_traits>
#include <vector>

// La métaprogrammation est le fait de manipuler des types à la compilation
// les type_traits sont des structs templates qui donnent des informations sur les types à la compilation.


inline void display_types_trait() {
    // Catégories
    std::cout << std::is_integral_v<int>       << std::endl; // 1
    std::cout << std::is_integral_v<double>    << std::endl; // 0
    std::cout << std::is_pointer_v<int*>       << std::endl; // 1
    std::cout << std::is_reference_v<int&>     << std::endl; // 1
    std::cout << std::is_class_v<std::string>  << std::endl; // 1

    // Propriétés
    std::cout << std::is_const_v<const int>    << std::endl; // 1
    std::cout << std::is_abstract_v<std::string> << std::endl; // 0

    // Construction/destruction
    std::cout << std::is_default_constructible_v<int>         << std::endl; // 1
    std::cout << std::is_copy_constructible_v<std::string>    << std::endl; // 1
    std::cout << std::is_move_constructible_v<std::string>    << std::endl; // 1
    std::cout << std::is_trivially_destructible_v<int>        << std::endl; // 1
}


inline void display_types_transformations() {

    // Enlever/ajouter const
    using T1 = std::remove_const_t<const int>;    // → int
    using T2 = std::add_const_t<int>;             // → const int

    // Enlever références
    using T3 = std::remove_reference_t<int&>;     // → int
    using T4 = std::remove_reference_t<int&&>;    // → int

    // Enlever pointeur
    using T5 = std::remove_pointer_t<int*>;       // → int

    // decay → enlève const, ref, array→pointer
    using T6 = std::decay_t<const int&>;          // → int
    using T7 = std::decay_t<int[5]>;              // → int*
    using T8 = std::decay_t<void(int)>;           // → void(*)(int)

    // conditional → choisir un type selon une condition
    using T9  = std::conditional_t<true,  int, double>; // → int
    using T10 = std::conditional_t<false, int, double>; // → double

    // Vérifications
    std::cout << std::is_same_v<T1, int>    << std::endl; // 1
    std::cout << std::is_same_v<T3, int>    << std::endl; // 1
    std::cout << std::is_same_v<T9, int>    << std::endl; // 1
    std::cout << std::is_same_v<T10, double><< std::endl; // 1
}



template<class T>
struct remove_all_pointers {
    using type = T;
};

template<class T>
struct remove_all_pointers<T*> {
    using type = remove_all_pointers<T>::type;
};


template<class T>
using remove_all_pointers_t =  remove_all_pointers<T>::type;


// Type_list — liste de types à la compilation
template<class... Types>
struct TypeList {
    static constexpr  std::size_t size = sizeof...(Types);
};


// Récupérer le premier type d'une TypeList
template<class List>
struct Front;

template <typename First, typename... Rest>
struct Front<TypeList<First, Rest...>> {
    using type = First;
};


template <typename List>
using Front_t = Front<List>::type;


inline void process() {
    // remove_all_pointers
    using T1 = remove_all_pointers_t<int***>;   // → int
    using T2 = remove_all_pointers_t<double**>; // → double
    using T3 = remove_all_pointers_t<char>;     // → char

    std::cout << std::is_same_v<T1, int>    << std::endl; // 1
    std::cout << std::is_same_v<T2, double> << std::endl; // 1
    std::cout << std::is_same_v<T3, char>   << std::endl; // 1

    // TypeList
    using MaListe = TypeList<int, double, std::string>;
    std::cout << MaListe::size << std::endl;  // 3

    using Premier = Front_t<MaListe>;  // → int
    std::cout << std::is_same_v<Premier, int> << std::endl; // 1
}




// PGCD compile-time (Plus Grand Commun Diviseur)
template<int A, int B>
struct PGCD {
    static constexpr int value = PGCD<B, A % B>::value;
};

template<int A>
struct PGCD<A, 0> {
    static constexpr int value = A;
};




// Choisir le type de stockage selon la taille
template<std::size_t N>
using StorageType = std::conditional_t<N <= 8, unsigned char, std::conditional_t<N <= 16,short unsigned int,  unsigned int >>;


// Utilisation pratique — choisir int ou double
template<typename T>
using PromotedType = std::conditional_t<std::is_integral_v<T>, double , T>;  // entier → promouvoir en double // déjà flottant → garder tel quel




template<typename T>
std::string serialiser(const T& val) {

    if constexpr (std::is_same_v<T, std::string>) {
        return "\"" + val + "\"";
    }
    else if constexpr (std::is_same_v<T, bool>) {
        return val ? "true" : "false";
    }
    else if constexpr (std::is_arithmetic_v<T>) {
        return std::to_string(val);
    }
    else if constexpr (std::is_pointer_v<T>) {
        if (val == nullptr) return "null";
        return "ptr(" + serialiser(*val) + ")";
    }
    else {
        return "inconnu";
    }
}

// Sérialiser un vector
template<typename T>
std::string serialiser(const std::vector<T>& vec) {
    std::string result = "[";
    for (std::size_t i = 0; i < vec.size(); i++) {
        result += serialiser(vec[i]);
        if (i < vec.size() - 1) result += ", ";
    }
    return result + "]";
}


#endif //TUTO_CPP_METAPROGRAMMING_H
