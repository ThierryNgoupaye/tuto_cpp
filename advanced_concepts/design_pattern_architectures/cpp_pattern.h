//
// Created by thierry on 09/05/2026.
//

#ifndef TUTO_CPP_CPP_PATTERN_H
#define TUTO_CPP_CPP_PATTERN_H

#include <iostream>
#include <memory>


// 1. CRTP (Curiously Recurring Template Pattern)
// Le polymorphisme à la vitesse de la lumière.
// La classe fille hérite de la classe mère en se passant elle-meme comme paramètre de template. Le compilateur connait à l'avance le type exact, et supprime la vtable, puis injecte le code directement.
template <typename T>
class RegleBase {
    public:
        void executer() {
            std::println(std::cout, "[Moteur] Préparation de la règle...");
            static_cast<T*>(this)->evaluerImpl();
            std::println(std::cout, "[Moteur] Nettoyage terminé.");
        }
};

class RegleFrottement : public RegleBase<RegleFrottement> {
    public:
        void evaluerImpl() const {
            std::println(std::cout, "(Frottement) Calcul des tolérances physiques.");
        }
};

class ReglePression : public RegleBase<ReglePression> {
    public:
        void evaluerImpl() const {
            std::println(std::cout, "(Pression) Vérification des micro-fissures.");
        }
};

template <typename TypeRegle>
void lanerRegle(RegleBase<TypeRegle> regle) {
    regle.executer();
}

inline void test_crtp_pattern() {
    constexpr  RegleFrottement regle1;
    constexpr ReglePression regle2;
    lanerRegle(regle1);
    lanerRegle(regle2);
}





// Autre exemple
template<typename Derived>
class Animal_CRTP {
    public:
        void parler() {
            // Appel statique vers la classe dérivée
            static_cast<Derived*>(this)->parlerImpl();
        }
};

class Chien_CRTP : public Animal_CRTP<Chien_CRTP> {
    public:
        void parlerImpl() {
            std::cout << "Woof!" << std::endl;
        }
};

class Chat_CRTP : public Animal_CRTP<Chat_CRTP> {
    public:
        void parlerImpl() {
            std::cout << "Miaou!" << std::endl;
        }
};

inline void crtp_simple_use() {
    Chien_CRTP chien;
    Chat_CRTP  chat;

    chien.parler();  // → "Woof!"  résolu à la compilation !
    chat.parler();   // → "Miaou!" résolu à la compilation !

    // Zéro vtable, zéro overhead !
}



// PIMPL (Pointer to Implementation)
// Le pare-feu de compilation
// Le fichier .h doit etre vide de tout détail technique.



//exemple d'implémentation pattern PIMPL + Builder
// dans le fichier header
class PIMPL_MoteurRegles {
    struct Impl;
    std::unique_ptr<Impl> m_impl;
    PIMPL_MoteurRegles();

    public:
        PIMPL_MoteurRegles(const PIMPL_MoteurRegles&) = delete;
        PIMPL_MoteurRegles& operator=(const PIMPL_MoteurRegles&) = delete;

        PIMPL_MoteurRegles(PIMPL_MoteurRegles&&) noexcept;
        PIMPL_MoteurRegles& operator=(PIMPL_MoteurRegles&&) noexcept;

        ~PIMPL_MoteurRegles();

        class PIMPL_MoteurBuilder;
        static PIMPL_MoteurBuilder builder();
        void executerAnalyse(string rapport) const;
};


class PIMPL_MoteurRegles::PIMPL_MoteurBuilder {
    PIMPL_MoteurRegles m_moteur_en_cours;
    public:
        PIMPL_MoteurBuilder& resolution(int res);
        PIMPL_MoteurBuilder& modeStrict(bool md);
        PIMPL_MoteurRegles build();
};



// Fichiers sources
struct PIMPL_MoteurRegles::Impl {
    int resolution = 1000;
    bool mode_strict = false;

    void initialisationLourde() {
        std::println(std::cout," Init lourde avec résolution : {}", resolution);
    }
};


inline PIMPL_MoteurRegles::PIMPL_MoteurRegles() : m_impl(std::make_unique<Impl>()) {}
inline PIMPL_MoteurRegles::PIMPL_MoteurRegles(PIMPL_MoteurRegles&&) noexcept = default;
inline PIMPL_MoteurRegles& PIMPL_MoteurRegles::operator=(PIMPL_MoteurRegles&&) noexcept = default;
inline PIMPL_MoteurRegles::~PIMPL_MoteurRegles() = default;
inline PIMPL_MoteurRegles::PIMPL_MoteurBuilder PIMPL_MoteurRegles::builder() {
    return PIMPL_MoteurBuilder{};
}
inline void PIMPL_MoteurRegles::executerAnalyse(string rapport) const {
    std::println(std::cout, "J'ai exécuté l'analyse de {}", rapport);
}



inline PIMPL_MoteurRegles::PIMPL_MoteurBuilder& PIMPL_MoteurRegles::PIMPL_MoteurBuilder::resolution(const int res) {
    m_moteur_en_cours.m_impl->resolution = res;
    return *this;
}
inline PIMPL_MoteurRegles::PIMPL_MoteurBuilder& PIMPL_MoteurRegles::PIMPL_MoteurBuilder::modeStrict(const bool md) {
    m_moteur_en_cours.m_impl->mode_strict = md;
    return *this;
}
inline PIMPL_MoteurRegles PIMPL_MoteurRegles::PIMPL_MoteurBuilder::build() {
    m_moteur_en_cours.m_impl->initialisationLourde();
    return std::move(m_moteur_en_cours);
}



inline void pimpl_pattern_use() {
    const PIMPL_MoteurRegles moteur_regles = PIMPL_MoteurRegles::builder()
                                        .modeStrict(true)
                                        .resolution(100)
                                        .build();
    const string rapport = " Analyse des bulletins de santé du végétal";
    moteur_regles.executerAnalyse(rapport);
}


#endif //TUTO_CPP_CPP_PATTERN_H
