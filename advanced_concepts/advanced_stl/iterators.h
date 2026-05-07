//
// Created by thierry on 06/05/2026.
//

#ifndef TUTO_CPP_ITERATORS_H
#define TUTO_CPP_ITERATORS_H

#include <algorithm>
#include <forward_list>
#include <iostream>
#include <iterator>
#include <list>
#include <set>
#include <vector>
#include <sstream>

// Un itérateur est un objet qui pointe vers élément d'un conteneur et qui sait comment passer à l'élément suivant.


template <class Iterator>
void trier(Iterator d, Iterator f) {
    std::sort(d, f);
}


// Différents types d'itérateurs


//1. Input iterator (lecture une fois)
inline void input_iterator_use() {
    std::istringstream iss("1 2 3 4 5");
    std::istream_iterator<int> debut(iss);
    const std::istream_iterator<int> fin;

    while (debut != fin) {
        std::cout << *debut << " ";
        ++debut;
    }
    std::cout << std::endl;
}


//2. forward iterator (écriture et lecture, plusieurs fois)
inline void forward_iterator_use() {
    std::forward_list f1 = {1,2,3,4,5,6};  // liste simplement chainée
    auto it = f1.begin();

    std::cout << *it << std::endl;
    std::cout << *it << std::endl;

    *it = 10;
    std::cout << *it << std::endl;

    ++it;
    std::cout << *it << std::endl;


    // ne peut pas reculer
    // --it;   Erreur
}



//3. bidirectional iterator (avance et recule)
inline void bidirectional_iterator_use() {
    std::list  my_list = {1,2,3,4,5};

    auto it = my_list.begin();
    ++it;
    ++it;  // pointe vers 3

    std::cout << *it << std::endl;

    --it;   // recul possible, pointe vers 2
    std::cout << *it << std::endl;


    std::set s = {1,2,3,4,5};
    auto s_it = s.end();
    --s_it;  // pointe sur le dernier élement
    std::cout << *s_it << std::endl;

    // Impossible de sauter
    // it + 3;
}



//4. Random Access Iterator
inline void random_access_iterator_use() {
    std::vector v = {1,2,3,4,5};

    auto it = v.begin();

    std::cout << *it << std::endl;
    std::cout << it[2] << std::endl;
    std::cout << *(it+3) << std::endl;


    it += 2;
    std::cout << *it << std::endl;

    it -= 1;
    std::cout << *it << std::endl;


    // comparaison
    const auto it2 = v.begin() + 3;
    std::cout << (it < it2) << std::endl;
    std::cout << (it2-it) << std::endl ;

    //tableau C sont aussi random access !
    constexpr int arr[] = {56,32,12,69};
    const int* ptr = arr;
    std::cout << ptr[3] << std::endl;
    std::cout << *(ptr+3) << std::endl;
}



// Iterator Traits
template<class Iterator>
void afficher_category() {
    using Category = std::iterator_traits<Iterator>::iterator_category;

    if constexpr (std::is_same_v<Category, std::random_access_iterator_tag>) {
        std::cout << "Random Access" << std::endl;
    }
    else if constexpr (std::is_same_v<Category, std::bidirectional_iterator_tag>) {
        std::cout << "Bidirectionnel" << std::endl;
    }
    else if constexpr (std::is_same_v<Category, std::forward_iterator_tag>) {
        std::cout << "Forward" << std::endl;
    }
    else {
        std::cout << "Input/Output" << std::endl;
    }
}


inline void afficher_categorie_iterator() {

    afficher_category<std::vector<int>::iterator>();

    afficher_category<std::list<int>::const_iterator>();

    afficher_category<std::forward_list<int>::const_iterator>();
}




// Fonctions utilitaires
inline void utils_function_use() {

    std::list l = {1,2,3,4,5};
    auto it = l.begin();

    std::advance(it, 3);   // avance de 3
    std::cout << *it << std::endl;   // affiche 4

    std::advance(it, -1); // recule de 1 (si bidirectionnel)
    std::cout << *it << std::endl;


    //distance entre deux itérateurs
    const auto debut = l.begin();
    const auto fin = l.end();
    std::cout << std::distance(debut, fin) << std::endl;


    const auto it2 = l.begin();
    const auto it3 = std::next(it2, 2);  // it2 non modifié, et it3 pointe sur 3
    std::cout << *it2 << std::endl;
    std::cout << *it3 << std::endl;

    const auto it4 = l.end();   // retourne un itérateur reculé de N
    const auto it5 = std::prev(it4, 2);
    std::cout << *it5 << std::endl;
}


// Insert Iterators
inline void insert_iterator_use() {
    std::vector source = {1,2,3,4,5};

    std::vector<int> dest1;
    std::copy(source.begin(), source.end(), std::back_inserter(dest1));
    // dest = {1,2,3,4,5}

    std::list<int> dest2;
    std::copy(source.begin(), source.end(), std::front_inserter(dest2));
    // dest 2 = {5,4,3,2,1}

    // insérer à une position précise
    std::vector dest3 = {10,20,30};
    std::copy(source.begin(), source.end(), std::inserter(dest3, dest3.begin() + 1));
    // dest3 = {10,1,2,3,4,5,20,30}

    for (const auto x : dest1) std::cout << x << " ";
    std::cout << std::endl;

    for (const auto x : dest2) std::cout << x << " ";
    std::cout << std::endl;

    for (const auto x : dest3) std::cout << x << " ";
    std::cout << std::endl;
}




// stream iterator (lire et écrire dans des flux)
inline void stream_iterator_use() {
    std::vector source = {1,2,3,4,5};

    std::copy(source.begin(), source.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;


    // istream_iterator (lire depuis un flux)
    std::istringstream iss("10 20 30 40 50");
    // result = {10,20,30,40,50}

    for (const std::vector<int> result{std::istream_iterator<int>(iss), std::istream_iterator<int>()}; const auto x : result) std::cout << x << " ";
    std::cout << std::endl;
}





// Reverse iterator
inline void reverse_iterator_use() {

    std::vector v = {1, 2, 3, 4, 5};

    // rbegin/rend → itérateurs inverses
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        std::cout << *it << " ";  // 5 4 3 2 1
    }
    std::cout << std::endl;

    // Ou avec range-based for
    // (pas directement, mais avec std::reverse_iterator)

    // Trouver le dernier élément pair
    const auto it = std::find_if(v.rbegin(), v.rend(),
        [](const int x) { return x % 2 == 0; });

    if (it != v.rend()) {
        std::cout << "Dernier pair : " << *it << std::endl; // 4
    }

    // .base() → convertir en itérateur normal
    const auto normal_it = it.base();
    // pointe vers l'élément APRÈS !
    std::cout << *std::prev(normal_it) << std::endl;  // 4
}






// Itérateur custom
template<typename T, std::size_t N>
class TableauStatique {
public:
    // Définir l'itérateur comme classe interne
    class Iterator {
    public:
        // Tags obligatoires pour les iterator_traits !
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;

        // Constructeur
        explicit Iterator(T* ptr) : m_ptr(ptr) {}

        // Déréférencement
        T& operator*()  { return *m_ptr; }
        T* operator->() { return m_ptr;  }

        // Avance
        Iterator& operator++() { ++m_ptr; return *this; }
        Iterator  operator++(int) {
            Iterator tmp = *this;
            ++m_ptr;
            return tmp;
        }

        // Recul
        Iterator& operator--() { --m_ptr; return *this; }
        Iterator  operator--(int) {
            Iterator tmp = *this;
            --m_ptr;
            return tmp;
        }

        Iterator operator+=(difference_type n) { m_ptr += n; return *this; }
        Iterator operator-=(difference_type n) { m_ptr -= n; return *this; }

        // Arithmétique
        Iterator operator+(difference_type n) const {
            return Iterator(m_ptr + n);
        }
        Iterator operator-(difference_type n) const {
            return Iterator(m_ptr - n);
        }
        difference_type operator-(const Iterator& other) const {
            return m_ptr - other.m_ptr;
        }

        // Accès indexé
        T& operator[](difference_type n) { return m_ptr[n]; }

        // Comparaison
        bool operator==(const Iterator& other) const {
            return m_ptr == other.m_ptr;
        }
        bool operator!=(const Iterator& other) const {
            return m_ptr != other.m_ptr;
        }
        bool operator<(const Iterator& other) const {
            return m_ptr < other.m_ptr;
        }

    private:
        T* m_ptr;
    };


    TableauStatique() : m_data{} {}

    T& operator[](std::size_t i) {
        if (i >= N) throw std::out_of_range("Index hors limites");
        return m_data[i];
    }

    // Méthodes begin/end → retournent nos itérateurs
    Iterator begin() { return Iterator(m_data);     }
    Iterator end()   { return Iterator(m_data + N); }

    [[nodiscard]] std::size_t size() const { return N; }

private:
    T m_data[N];
};


inline void custom_iterator_use() {
    TableauStatique<int, 5> t;
    t[0] = 5;
    t[1] = 3;
    t[2] = 1;
    t[3] = 4;
    t[4] = 2;

    // Range-based for → fonctionne grâce à begin/end
    for (const auto& x : t) std::cout << x << " ";
    std::cout << std::endl;

    // Algorithmes STL → fonctionnent grâce aux iterator tags
    std::sort(t.begin(), t.end());

    for (const auto& x : t) std::cout << x << " ";
    std::cout << std::endl;  // 1 2 3 4 5

    // std::find
    auto it = std::find(t.begin(), t.end(), 3);
    if (it != t.end()) {
        std::cout << "Trouvé : " << *it << std::endl;  // 3
    }

    // std::advance ✅
    auto it2 = t.begin();
    std::advance(it2, 2);
    std::cout << *it << std::endl;  // 5
}






#endif //TUTO_CPP_ITERATORS_H
