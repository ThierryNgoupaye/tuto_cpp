//
// Created by thierry on 15/05/2026.
//

#include "../headers/Camera.h"

#include "Camera.h"
#include <stdexcept>

T_Camera::T_Camera() = default;

void T_Camera::initialiser() {
    // Simulation d'une ouverture de caméra
    m_ouverte = true;
}

void T_Camera::fermer() {
    m_ouverte = false;
}

void T_Camera::setResolution(const int largeur, const int hauteur) {
    if (largeur <= 0 || hauteur <= 0) {
        throw std::invalid_argument("Résolution invalide !");
    }
    m_largeur = largeur;
    m_hauteur = hauteur;
}

T_Frame T_Camera::capturerFrame() const {
    if (!m_ouverte) {
        throw std::runtime_error("Caméra non initialisée !");
    }

    // Simulation d'une frame remplie de pixels noirs
    T_Frame frame;
    frame.largeur = m_largeur;
    frame.hauteur = m_hauteur;
    frame.pixels.resize(m_largeur * m_hauteur * 3, 0);

    return frame;
}