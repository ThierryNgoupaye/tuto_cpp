//
// Created by thierry on 06/05/2026.
//

#ifndef TUTO_CPP_CPP_20_CONCEPTS_H
#define TUTO_CPP_CPP_20_CONCEPTS_H

#include <concepts>
#include <iostream>


//Un concept est une contrainte claire et nommée sur un type.

template <typename T>
concept Entier = std::is_integral_v<T>;


template <typename T>
concept Floattant = std::is_floating_point_v<T>;

template <typename T>
concept Nombre = Entier<T> || Floattant<T>;

// utilisation
template<Entier T>
void afficherEntier(const T& val) {
    std::cout << "Entier : " << val << std::endl;
}


template <Floattant T>
void afficherFlottant(const T& val) {
    std::cout << "Flottant : " << val << std::endl;
}

template <Nombre T>
void afficherNombre(const T& val) {
    std::cout << "Nombre : " << val << std::endl;
}



// requires permet de définir des contraintes complexes
template<class T>
concept Conteneur = requires(T c)
{
    // doit contenir ces membres/méthodes valides
    c.begin();
    c.end();
    c.size();
    typename T::value_type;
};


template <class T>
concept Comparable = requires(T a, T b)
{
    {a == b} -> std::convertible_to<bool>;
    {a != b} -> std::convertible_to<bool>;
    {a < b } -> std::convertible_to<bool>;

};


template <class T>
concept Affichable = requires(T val)
{
    {std::cout << val} -> std::same_as<std::ostream&>;
};


//Utilisation
template<Conteneur C>
void afficherConteneur(const C& c) {
    for (const auto& ele : c) {
        std::cout << ele << " ";
    }
    std::cout << std::endl;
}




// Types de contraintes dans requires
template <class T>
concept MonConcept = requires(T val, T other)
{
    // Expression simple devant etre valide
    val.size();    // doit avoir size()
    val++;          // doit supporter ++


    // Expression typée → verifie aussi le type de retour
    {val.size()}  -> std::convertible_to<std::size_t>;
    {val + other}  -> std::same_as<T>;


    // attribut doit exister
    typename T::value_type;
    typename T::iterator;


    // contraintes imbriquées : condition sur le type
    requires std::is_copy_constructible_v<T>;
    requires sizeof(T) > 4;
};


//Utilisation
template<MonConcept C>
void afficherConteneurConcept(const C& c) {
    for (const auto& ele : c) {
        std::cout << ele << " ";
    }
    std::cout << std::endl;
}




// Facon d'utiliser un concept
template <Entier T>   //(dans la liste template, afin d'imposer des conditions sur le paramètre template)
void f1(T val){}

template <class T>          // Avec requires après les paramètres (a la meme action que précédemment).
void f2(T val) requires Entier<T> {}

template<typename T> requires Entier<T>  // Pareil
void f3() {}

void f4(Entier auto val){}  // la plus courte




// Concepts les plus utilisés

//  Types
// std::integral<T>           // entiers
// std::signed_integral<T>    // entiers signés
// std::unsigned_integral<T>  // entiers non signés
// std::floating_point<T>     // flottants
// std::arithmetic<T>         // intégral + flottant
//
// // Relations
// std::same_as<T, U>         // T == U exactement
// std::derived_from<T, Base> // T hérite de Base
// std::convertible_to<T, U>  // T convertible en U
// std::common_with<T, U>     // T et U ont un type commun
//
// // Opérations
// std::equality_comparable<T>    // supporte ==
// std::totally_ordered<T>        // supporte < > <= >=
// std::copyable<T>               // copiable
// std::movable<T>                // movable
// std::default_initializable<T>  // constructeur défaut
//
// // Callable
// std::invocable<F, Args...>     // F appelable avec Args
// std::predicate<F, Args...>     // F retourne bool
//




#endif //TUTO_CPP_CPP_20_CONCEPTS_H
