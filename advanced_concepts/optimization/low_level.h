//
// Created by thierry on 10/05/2026.
//

#ifndef TUTO_CPP_LOW_LEVEL_H
#define TUTO_CPP_LOW_LEVEL_H

#include <iostream>
#include <bitset>
#include <cstdint>
#include <string>
#include <vector>

// Bit manipulation
inline void basic_bit_use() {
    constexpr int a = 0b00101010;
    constexpr int b = 0b00001111;

    std::cout << std::bitset<8>(a & b) << std::endl;   // applique le  et logique bit à bit

    std::cout << std::bitset<8>(a | b) << std::endl;     // applique le ou logique bit à bit

    std::cout << std::bitset<8>(a ^ b) << std::endl;     // applique a xor b bit à bit

    std::cout << std::bitset<8>(~a) << std::endl;       //  retourne !a bit Àà bit

    std::cout << std::bitset<8>(a << 1) << std::endl;     // décallage de 1 bit à gauche

    std::cout << std::bitset<8>(a >> 1) << std::endl;         // décallage de de 1 bit à droite
}


inline void bit_operation() {

    uint8_t flag = 0b0000000;

    // activer un bit (on utilise | avec un masque)
    flag |= 1 << 3;   // flag = flag | 0b00000001 << 3

    std::cout << std::bitset<8>(flag) << std::endl;  // 00001000


    // desactiver un bi (on utilise & avec le complément du masque)
    flag &= ~(1 << 3);
    std::cout << std::bitset<8>(flag) << std::endl;


    // inverser un bit
    flag ^= 1 << 5;  // inverse le bit 5
    std::cout << std::bitset<8>(flag) << std::endl;

    flag ^= 1 << 5;  // inverse encore
    std::cout << std::bitset<8>(flag) << std::endl;


    // Lire un bit (on utilise & avec un masque 0 ou non zero)
    flag = 0b10110101;
    const bool bit3 = (flag >> 3) & 1;    // lit le bit 3
    std::cout <<"Bit 3 : " << bit3 << std::endl;

    const bool bit2 = (flag >> 2) & 1;    // lit le bit 2
    std::cout <<"Bit 2 : " << bit2 << std::endl;
}




// cas pratique (système de flags avec bits)
enum class Permission : uint8_t {
    AUCUNE    = 0b00000000,
    LECTURE   = 0b00000001,   // bit 0
    ECRITURE  = 0b00000010,   // bit 1
    EXECUTION = 0b00000100,   // bit 2
    ADMIN     = 0b00001000,   // bit 3
};


Permission operator | (Permission a, Permission b) {
    return static_cast<Permission>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

bool operator &(Permission a, Permission b) {
    return static_cast<uint8_t>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
}

class Utilisateur {
    std::string m_nom;
    Permission m_permissions;

    public:
        explicit Utilisateur(std::string nom, const Permission& permission) : m_nom(std::move(nom)), m_permissions(permission) {}

        [[nodiscard]] bool peutLire() const  {
            return m_permissions & Permission::LECTURE;
        }

        [[nodiscard]] bool peutEcrire()  const {
                return m_permissions & Permission::ECRITURE;
        }
        [[nodiscard]] bool peutExecuter()const {
                return m_permissions & Permission::EXECUTION;
        }
        [[nodiscard]] bool estAdmin()    const {
                return m_permissions & Permission::ADMIN;
        }

        void afficherPermissions() const {
            std::cout << m_nom << " → ";
            if (peutLire())     std::cout << "R";
            if (peutEcrire())   std::cout << "W";
            if (peutExecuter()) std::cout << "X";
            if (estAdmin())     std::cout << "A";
            std::cout << std::endl;
        }

};


inline void bit_manip_use() {
    const Utilisateur alice("Alice",
        Permission::LECTURE |
        Permission::ECRITURE |
        Permission::EXECUTION);

    const Utilisateur bob("Bob",
        Permission::LECTURE);


    const Utilisateur root("Root",
        Permission::LECTURE   |
        Permission::ECRITURE  |
        Permission::EXECUTION |
        Permission::ADMIN);

    alice.afficherPermissions();  // Alice → RWX
    bob.afficherPermissions();    // Bob   → R
    root.afficherPermissions();   // Root  → RWXA
}


constexpr bool estPair(const int x) {
    return !( x & 1);
}

constexpr int multiply_power_2(const int x, const int puis) {
    return x << puis;
}

constexpr int div_power_2(const int x, const int puis) {
    return x >> puis;
}

constexpr void bit_tips() {

    std::cout << estPair(42) << std::endl;    // 1
    std::cout << estPair(39) << std::endl;    // 0
    std::cout << multiply_power_2(38, 2) << std::endl;
    std::cout << multiply_power_2(28, 4) << std::endl;
    std::cout << div_power_2(38, 1) << std::endl;
    std::cout << div_power_2(43, 2) << std::endl;


    constexpr auto estPuissance2 = [](const int n) {
        return n > 0 && !(n & (n-1));
    };

    std::cout << estPuissance2(8)  << std::endl;  // 1
    std::cout << estPuissance2(7)  << std::endl;  // 0
    std::cout << estPuissance2(16) << std::endl;  // 1


    // compter les bits
    #include <bit>
    std::cout << std::popcount(42u) << std::endl;

    // Swap sans variable temporaire
    int a = 6, b= 2;
    a ^=b;
    b ^=a;
    a ^=b;
    std::cout << a << " " << b << std::endl;  // 3 5
}



// Padding avancé
struct DateNaive {
    int jour;  // 4 octets pour stocker de  1-31, 5 bits suffisent
    int mois;  // 4 octets pour stocker de 1 -12, 4 bits suffisent
    int annee;   // 4 octets pour stocker de 0 - 9999 (14 bits suffisent !)
};
// Total : 12 octets de gaspillage


struct DateCompacte {
    uint32_t jour : 5;   // 5bits
    uint32_t mois : 4;   // 4 bits
    uint32_t annee : 14;  // 14 bits
    uint32_t       :9;   // 9 bits pour aligner la structure sur 32 bits
};

inline void padding_optimization_use() {
    std::cout << sizeof(DateNaive) << std::endl;
    std::cout << sizeof(DateCompacte) << std::endl;

    DateCompacte d{};
    d.jour = 11;
    d.mois = 5;
    d.annee = 2026;

    std::cout << d.jour << " " << d.mois <<  " " << d.annee <<  std::endl;
}


struct NormalStruct {
    uint8_t a;   // 1 octet
    uint32_t b;   // 4 octets  (+ 3 octet de padding après a)
    uint8_t c;    // 1 octet   (+ 3 octets de padding après c!)
};
// total : 12 octets


struct __attribute__((packed)) PackedStruct {
    uint8_t  a;  // 1 octet
    uint32_t b;  // 4 octets (pas de padding !)
    uint8_t  c;  // 1 octet (pas de padding !)
};
// Total : 6 octets

// packed s'utilise quand
// protocole réseau avec format exact de données
// fichiers binaires, lecture écriture directe
// systèmes embarquée avec mémoire très limitée
inline void suppress_padding_use() {
    std::cout << sizeof(NormalStruct) << std::endl;
    std::cout << sizeof(PackedStruct) << std::endl;
}



// bit_cast,  reinterpret_cast sans undefined behavior
inline void bit_cast_without_ub_use() {

    float f = 3.14f;
    auto* ptr = reinterpret_cast<uint32_t*>(&f);  // mauvais, ub!


    constexpr float valeur = 3.14f;
    const auto bits = std::bit_cast<uint32_t>(valeur);
    std::cout << std::hex << bits << std::endl;

    // Voir la représentation d'un float
    constexpr float pi = 3.14159f;
    auto piBits = std::bit_cast<uint32_t>(pi);

    std::cout << "Signe    : " << ((piBits >> 31) & 1) << std::endl;
    std::cout << "Exposant : " << ((piBits >> 23) & 0xFF) << std::endl;
    std::cout << "Mantisse : " << (piBits & 0x7FFFFF) << std::endl;

    // Reconvertir → valeur identique !
    const auto retour = std::bit_cast<float>(piBits);
    std::cout << retour << std::endl;  // 3.14159
}


struct __attribute__((packed)) EnteteImage {
    uint16_t magic;
    uint32_t taille;
    uint16_t reserve;
    uint32_t offset;
};

inline void bit_parse_use() {

    const std::vector<uint8_t> donnees = {
        0x42, 0x4D,        // magic = "BM"
        0x36, 0x00, 0x00, 0x00,  // taille = 54
        0x00, 0x00,        // réservé
        0x36, 0x00, 0x00, 0x00   // offset = 54
    };

    // Parser directement depuis les octets bruts !
    const auto* entete = reinterpret_cast<const EnteteImage*>(donnees.data());

    std::cout << std::hex;
    std::cout << "Magic  : " << entete->magic   << std::endl;
    std::cout << std::dec;
    std::cout << "Taille : " << entete->taille  << std::endl;
    std::cout << "Offset : " << entete->offset  << std::endl;

}













#endif //TUTO_CPP_LOW_LEVEL_H
