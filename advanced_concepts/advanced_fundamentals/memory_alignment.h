//
// Created by thierry on 05/05/2026.
//

#ifndef TUTO_CPP_MEMORY_ALIGNMENT_H
#define TUTO_CPP_MEMORY_ALIGNMENT_H

#include <iostream>
#include <new>

#include "../../headers/_inline_use.h"


/**
 * alingof
 */

// Alingof retourne l'alignement d'une variable.
// impose que l'adresse de la variable soit un multiple de cet alignement
inline void print_basic_type_alignment() {
    std::cout << "alignement d'un char = " << alignof(char) << std::endl;
    std::cout << "alignement d'un type \"short\" = " << alignof(short) << std::endl;
    std::cout << "alignement d'un int = " << alignof(int) << std::endl;
    std::cout << "alignement d'un entier long = " << alignof(long) << std::endl;
    std::cout << "alignement d'un réel float = " << alignof(float) << std::endl;
    std::cout << "alignement d'un double = " << alignof(double) << std::endl;
    std::cout << "alignement d'un pointeur  = " << alignof(int*) << std::endl;
}


struct MalOrdonnee {
    char a;   // 1 octet
    int b;    // 4 octets
    char c;   // 1 octet
    double d;  // 8 octets
};

struct BienOrdonne {
    double d;  // 8 octets
    int b;    // 4 octets
    char a;    // 1 octet
    char c;    // 1 octet

};


inline void print_structs_alignment() {

    // La différence d'espace occupé en mémoire vient du fait des alignements.
    // le compilateur rajoute des espaces pour respecter les alignements
    // Pour la structure MalOrdonnee, sa taille à priori est 14 octets, mais comme
    // le compilateur range les adresses des variable en respectant la regle qu'ils soient multiples
    // de leur alignement, ce dernier rajoute des paddings ou des vides pour respecter cette
    // règle, ce qui peut augmenter considérablement la taille d'un objet en mémoire, quand ses attributs sont mal rangés
    std::cout << "Mal ordonnée: " << sizeof(MalOrdonnee) << std::endl;
    std::cout << "Bien ordonnée: " << sizeof(BienOrdonne) << std::endl;

}



/**
 * Alignas
 */

// Forcer le struct à s'aligner sur 64 octets.
struct alignas(16) VecteurSIMD {
    float x,y,z,w;     // 4*4 = 16 octets occupés en mémoire
                       // l'alignement de la structure sera de 16 au lieu de 4 car on l'a imposé
};


// force cette variable entiere à s'aligner sur 64 octets
// son adresse en mémoire sera un multiple de 64 et la prochaine variable
// la suivant sera à l'adresse addr_cache_line_var + 64
alignas(64) inline int cache_line_var;



inline void display_structs_alignment() {
    constexpr VecteurSIMD v{};
    std::cout << "alignof(VecteurSIMD) = " << alignof(VecteurSIMD) << std::endl;
    std::cout << "sizeof(VecteurSIMD) = " << sizeof(VecteurSIMD) << std::endl;

    std::cout << "adresse : " << &v << std::endl;
}





/**
 * Placement new
 * Construire à une adresse précise.
 */

class Moteur {
    int m_puissance;
    public:
        explicit Moteur(const int puissance) : m_puissance(puissance) {
            std::cout << "Moteur construit ! puissance = " << puissance << std::endl;
        };

        ~Moteur() {
            std::cout << "Moteur détruit !" << std::endl;
        };

        [[nodiscard]] int getPuissance() const {
            return m_puissance;
        }
};



/*
 * En utilisant le placement new() à la place du new classique,
 * on outrepasse les appels systèmes qui sont très couteux et on gagne en performance !
 */
inline void placement_new() {
    alignas(Moteur) char buffer[sizeof(Moteur)];  // garantit un emplacement mémoire de la taille d'un Moteur et dont l'adresse memoire est un multiple de l'alignement d'un Moteur

    auto* moteur = new(buffer) Moteur(300);

    std::cout << "Puissance : " << moteur->getPuissance() << std::endl;

    std::cout << "Adresse mémoire moteur = " << &moteur << std::endl;

    moteur->~Moteur();
}






/**
 * Memory Pool
 * Il s'agit de pré-allouer un gros bloc et y construire des objets sans appeler malloc
 * donc zero allocations heap  → performances améliorées
 **/

template <typename T, size_t N>
class MemoryPool {
    public:
        MemoryPool() : m_index(0) {
            std::cout << "Pool crée pour " << N << " objets" << std::endl;
        }

        /**
         * Cette fonction alloue de l'espace pour un objet T bien aligné
         * @return T*
         */
        T* allocate() {
            if (m_index >= N) {
                throw std::bad_alloc();
            }
            T* ptr = new(&m_buffer[m_index]) T();
            m_index++;
            return ptr;
        }

        /**
         * Cette fonction libère l'objet situé à l'adresse contenue dans ptr via son destructeur
         * et décrémente d'un le nombre
         * @param ptr
         */
        void free(T* ptr) {
            ptr->~T();
            m_index--;
        }

        [[nodiscard]] size_t available() const { return N - m_index; }


    private:
        alignas(T) char m_buffer[sizeof(T) * N]{};
        size_t m_index;

};


class Balle {
    public:
        Balle() : m_x(0), m_y(0){}
        void setPos(const float x, const float y) {
            m_x = x;
            m_y = y;
        }
        void afficher() const {
            std::cout << "Balle(" << m_x << ", " << m_y << ")" << std::endl;
        }

    private:
        float m_x, m_y;
};


inline void allocate_via_pool_memory() {
    MemoryPool<Balle, 10> pool;
    std::cout << "Disponible: " << pool.available() << std::endl;

    Balle* b1 = pool.allocate();
    Balle* b2 = pool.allocate();
    Balle* b3 = pool.allocate();

    b1->setPos(1.0f, 2.0f);
    b2->setPos(4.0f, 10.0f);
    b3->setPos(53.0f, 14.0f);

    std::cout << "Disponible : " << pool.available() << std::endl;

    pool.free(b1);
    pool.free(b2);
    pool.free(b3);

    std::cout << "Disponible : " << pool.available() << std::endl;
}



#endif //TUTO_CPP_MEMORY_ALIGNMENT_H
