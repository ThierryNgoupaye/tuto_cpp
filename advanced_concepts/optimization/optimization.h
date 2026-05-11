//
// Created by thierry on 09/05/2026.
//

#ifndef TUTO_CPP_OPTIMIZATION_H
#define TUTO_CPP_OPTIMIZATION_H
#include <vector>


using std::vector;

// 1. Cache locality

// Data-Oriented Design - restructureur pour le cache
// Au lieu de créer un vecteur contenant par exemple 1.000.000 de struct, particules, il preferable de
// De créer une structure contenant des tableaux d'attributs, rangés par type afin d'optimiser le cache du processeur



// Mauvais exemple  (Aos = Array of Struct)
struct MParticule {
    float x, y, z;      // position (12 octets)
    float vx, vy, vz;   // vitesse (12 octets)
    float masse;         // masse (4 octets)
    float energie;       // énergie (4 octets)
};

inline void bad_cache_friendly_optimization() {

    // Mettre à jour seulement les positions
    for (std::vector<MParticule> particules(1000000); auto& p : particules) {
        p.x += p.vx;  // ← charge 32 octets par particule
        p.y += p.vy;  // mais n'utilise que x,y,z,vx,vy,vz
        p.z += p.vz;  // masse et énergie chargées inutilement !
    }
}



// Bon exemple dans ce cas (SoA) structure of Array
struct Particules {
    vector<float> x, y, z;
    vector<float> vx, vy, vz;
    vector<float> masses;
    vector<float> energie;

    explicit Particules(const size_t n) : x(n), y(n), z(n), vx(n), vy(n), vz(n), masses(n), energie(n) {}
};


inline void soa_optimization_use() {

    Particules particules(10000000);

    //Mettre à jour les positions
    for (size_t i = 0; i < particules.x.size(); ++i) {
        particules.x[i] += particules.vx[i];    // chargement en une fois de 16 floats de x ainsi de suite (ligne de cache = 64 octets et 1 float = 4 octets)
        particules.y[i] += particules.vy[i];
        particules.z[i] += particules.vz[i];
    }
}



// Branch Prediction

//eviter des if else, pour afin d'éviter au processeur de faire des devinettes
inline int valeurAbsolueBranchless(const int x) {
    const int masque  = x >> 31;
    return (x + masque) ^ masque;
}

inline bool estNegatif(const int x) {
    return (x >> 31) & 1 ;
}


// Equation importante
// Resultat = A + (masque & (B-A))    resume un if/else


// [[likely]] et [[unlikely]]
inline void traiter(const int valeur) {

    if (valeur > 0) [[likely]] {
        std::println(cout, "Positif : {}", valeur);
    } else [[unlikely]] {
        std::println(cout, "Negatif ou zéro : {}", valeur);
    }
}




// # Niveaux d'optimisation
// g++ -O0   # aucune optimisation → debug facile
// g++ -O1   # optimisations de base
// g++ -O2   # optimisations standard → recommandé production
// g++ -O3   # optimisations agressives → parfois instable
// g++ -Os   # optimise pour la taille (embarqué)
// g++ -Oz   # encore plus petit (Clang)
//
// # Optimiser pour TON CPU spécifiquement
// g++ -march=native
// # → utilise TOUTES les instructions de ton CPU
// # → binaire non portable mais ultra optimisé !
//
// # Combinaison recommandée pour la production
// g++ -O2 -march=native -std=c++23
//
// # Link-time optimization → optimise entre fichiers !
// g++ -O2 -flto
// # → le linker peut inliner entre .cpp différents !
//



// # Debug → pas d'optimisation, symboles de debug
// set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
//
// # Release → optimisation maximale
// set(CMAKE_CXX_FLAGS_RELEASE "-O2 -march=native -DNDEBUG")
//
// # RelWithDebInfo → optimisé mais debuggable
// set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG")







#endif //TUTO_CPP_OPTIMIZATION_H
