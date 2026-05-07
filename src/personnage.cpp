//
// Created by thierry on 29/04/2026.
//

#include <iostream>
#include "../headers/personnage.h"

// Classe Personnage
Personnage::Personnage(const unsigned int &pdv, const unsigned int &vitesse, const unsigned int &level)
    : m_pdv(pdv), m_vitesse(vitesse), m_level(level){
    std::cout << "Constructeur de Personnage" <<std::endl;
}

void Personnage::attaquer(Personnage& other) {
    other.m_level -= 10;
}

void Personnage::displayAttributes() {
    std::cout << this->m_pdv << " / " << this->m_vitesse << " / " << this->m_level << std::endl;
}

void Personnage::setPdv(const unsigned int& pdv) {
    this->m_pdv = pdv;
}

unsigned int Personnage::getPdv() const {
    return this->m_pdv;
}


// Classe Guerrier
Guerrier::Guerrier(const unsigned int& pdv, const unsigned int&  vitesse, const unsigned int& level, const unsigned int& force)
    : Personnage(pdv, vitesse, level), m_force(force){
    std::cout << "Constructeur de Guerrier";
}

void Guerrier::attaquer(Personnage& other) {
    other.setPdv(other.getPdv() - 10);
}

void Guerrier::displayAttributes() {
    std::cout << this->m_pdv << " / " << this->m_vitesse << " / " << this->m_level << " / " << this->m_force << std::endl;
}



//  Classe GuerrierDef
GuerrierDef::GuerrierDef(const unsigned int& pdv, const unsigned int&  vitesse, const unsigned int& level, const unsigned int& force, const unsigned int& armor )
    : Guerrier(pdv, vitesse, level, force), m_armor(armor){
    std::cout << "Constructeur de Guerrier Def" << std::endl;
}

void GuerrierDef::attaquer(Personnage& other) {
    other.setPdv(other.getPdv() - 10 * this->m_force);
}

void GuerrierDef::displayAttributes() {
    std::cout << this->m_pdv << " / " << this->m_vitesse << " / " << this->m_level << " / " << this->m_force << std::endl;
}



