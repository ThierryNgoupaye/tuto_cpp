//
// Created by thierry on 15/05/2026.
//

#ifndef TUTO_CPP_PROFILING_H
#define TUTO_CPP_PROFILING_H

// main.cpp — exemple avec goulot évident
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cmath>
#include <cstdint>


// Fonction LENTE — O(n²) intentionnel
inline void trierLentement(std::vector<int>& v) {
    for (size_t i = 0; i < v.size(); i++) {
        for (size_t j = 0; j < v.size() - 1; j++) {
            if (v[j] > v[j+1]) std::swap(v[j], v[j+1]);
        }
    }
}


// Fonction RAPIDE — O(n log n)
inline void trierRapidement(std::vector<int>& v) {
    std::ranges::sort(v);
}

// Calcul intensif
inline  double calculerSomme(const std::vector<double>& v) {
    double somme = 0;
    for (const auto x : v) {
        somme += std::sqrt(x) * std::sin(x);  // calcul lourd
    }
    return somme;
}



// Traitement lent — à détecter avec Perf
inline void traiterFrameLent(std::vector<uint8_t>& pixels, const int largeur, const int hauteur) {
    // Parcours colonne par colonne → cache unfriendly !
    for (int x = 0; x < largeur; x++) {
        for (int y = 0; y < hauteur; y++) {
            const int idx = (y * largeur + x) * 3;
            const uint8_t r = pixels[idx];
            const uint8_t g = pixels[idx + 1];
            const uint8_t b = pixels[idx + 2];
            pixels[idx] = pixels[idx+1] = pixels[idx+2] =
                static_cast<uint8_t>(r * 0.299 +
                                     g * 0.587 +
                                     b * 0.114);
        }
    }
}

// Traitement rapide — cache friendly
inline void traiterFrameRapide(std::vector<uint8_t>& pixels) {
    // Parcours linéaire → cache friendly
    for (size_t i = 0; i < pixels.size(); i += 3) {
        const auto gris = static_cast<uint8_t>(
            pixels[i]   * 0.299f +
            pixels[i+1] * 0.587f +
            pixels[i+2] * 0.114f);
        pixels[i] = pixels[i+1] = pixels[i+2] = gris;
    }
}


inline void profiling_program() {
    // Préparer les données
    std::vector<int>    v1(10000);
    std::vector<double> v2(1000000);

    for (int i = 0; i < 10000; i++)   v1[i] = 10000 - i;
    for (int i = 0; i < 1000000; i++) v2[i] = i * 0.001;

    // Opérations
    trierLentement(v1);     // ← goulot 1 !
    const double s = calculerSomme(v2);  // ← goulot 2 ?

    std::cout << "Somme : " << s << std::endl;
}


inline void profiling_with_cache_miss() {
    constexpr int W = 1920, H = 1080;
    std::vector<uint8_t> frame(W * H * 3, 128);

    // Répéter pour que Perf ait assez de données
    for (int i = 0; i < 100; i++) {
        traiterFrameLent(frame, W, H);
    }

    for (int i = 0; i < 100; i++) {
        traiterFrameRapide(frame);
    }

}



#endif //TUTO_CPP_PROFILING_H
