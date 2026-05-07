//
// Created by thierry on 06/05/2026.
//

#ifndef TUTO_CPP_SFINAE_H
#define TUTO_CPP_SFINAE_H

#include <iostream>
#include <type_traits>




// Vérification de catégorie
// std::is_integral_v<T>        // int, char, bool...
// std::is_floating_point_v<T>  // float, double...
// std::is_arithmetic_v<T>      // intégral + flottant
// std::is_pointer_v<T>         // T*
// std::is_reference_v<T>       // T& ou T&&
// std::is_array_v<T>           // T[]
// std::is_void_v<T>            // void
// std::is_class_v<T>           // struct/class
// std::is_enum_v<T>            // enum
//
// // Relations entre types
// std::is_same_v<T, U>         // T == U ?
// std::is_base_of_v<Base, T>   // T hérite de Base ?
// std::is_convertible_v<T, U>  // T convertible en U ?
//
// // Propriétés
// std::is_const_v<T>           // T est const ?
// std::is_trivial_v<T>         // type trivial ?
// std::is_default_constructible_v<T>  // constructeur défaut ?
// std::is_copy_constructible_v<T>     // copiable ?
// std::is_move_constructible_v<T>     // movable ?
//
// // Transformations de types
// std::remove_const_t<T>       // enlève const
// std::remove_reference_t<T>   // enlève &/&&
// std::remove_pointer_t<T>     // enlève *
// std::add_const_t<T>          // ajoute const
// std::decay_t<T>              // enlève const, ref, array→pointer

// SFINAE = Substitution Failure Is Not An Error
// Un echec de substitution n'est pas une erreur.




template<typename T>
std::enable_if_t<std::is_integral_v<T>> afficher_type(T val) {
    std::cout << "Entier : " << val << std::endl;
}

template<typename T>
std::enable_if_t<std::is_floating_point_v<T>> afficher_type(T val) {
    std::cout << "Flottant : " << val << std::endl;
}



// version moderne
void traiter(std::integral auto val) {
    std::cout << "Integral : " << val << std::endl;
}




template<typename T, typename = void>
struct is_container : std::false_type {};

template<typename T>
struct is_container<T,std::void_t<decltype(std::declval<T>().begin()),decltype(std::declval<T>().end()),typename T::value_type>>
    : std::true_type {};


template<typename T>
inline constexpr bool is_container_v = is_container<T>::value;


template<typename T>
void print_(const T& val) {
    if constexpr (is_container_v<T>) {
        std::cout << "[";
        bool first = true;
        for (const auto& elem : val) {
            if (!first) std::cout << ", ";
            std::cout << elem;
            first = false;
        }
        std::cout << "]" << std::endl;
    }
    else if constexpr (std::is_same_v<T, bool>) {
        std::cout << (val ? "true" : "false") << std::endl;
    }
    else {
        std::cout << val << std::endl;
    }
}











#endif //TUTO_CPP_SFINAE_H
