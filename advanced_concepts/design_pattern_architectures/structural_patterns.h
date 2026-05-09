//
// Created by thierry on 09/05/2026.
//

#ifndef TUTO_CPP_STRUCTURAL_PATTERNS_H
#define TUTO_CPP_STRUCTURAL_PATTERNS_H
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <chrono>




/**
 * Les patterns structurels répondent à la question de savoir comment assembler
 * les objets pour former des structures plus grandes et flexibles.
 */


// Pattern Adapter = vouloir rendre compatible une ancienne version de code avec une nouvelle sans pour autant modifier l'ancienne version
class AfficheurImage {
public:
    virtual void afficher(const std::string& titre) = 0;
    virtual ~AfficheurImage() = default;
};



class OpenCVImage {
    public :
        explicit OpenCVImage(const std::string& fichier) : m_fichier(fichier) {}

        void imshow(const std::string& fenetre) {
            std::println(std::cout,"OpenCV affiche {}, dans {}", m_fichier, fenetre);
        }

    private:
        std::string m_fichier;
};


class OpenCVAdapter : public AfficheurImage {

    OpenCVImage m_image;

    public:
        explicit OpenCVAdapter(std::string fichier) : m_image(std::move(fichier)){}

        void afficher(const std::string& titre) override {
            // code avant pour affichage
            m_image.imshow(titre);
            // code apres pour traduction
        }
};


inline void adapter_pattern_use() {

    const std::unique_ptr<AfficheurImage> afficheur = std::make_unique<OpenCVAdapter>("photo.jpg");

    afficheur->afficher("Résultat du traitement");
}



// Pattern Decorator = Ajouter des fonctionnalités à un objet sans modifier sa classe et pouvoir combiner
// des fonctionnalités librement
// L'héritage bien pensée resous ce probleme.

// interface commune
class RegleValidation {
    public:
        virtual ~RegleValidation() = default;
        virtual bool evaluer() const = 0;
};

//objet de base
class RegleEtancheite : public RegleValidation {
    public:
        bool evaluer() const override {
            std::cout << "  [Test] Vérification de la résistance à 5 ATM..." << std::endl;
            return true; // Test réussi
        }
};

// décorateur de base
class DecorateurRegle : public RegleValidation {
    protected:
        // Il possède un pointeur vers la règle qu'il enveloppe
        std::unique_ptr<RegleValidation> m_regle_interne;

    public:
        explicit DecorateurRegle(std::unique_ptr<RegleValidation> regle) : m_regle_interne(std::move(regle)) {}

        bool evaluer() const override {
            return m_regle_interne->evaluer();
        }
};


// --- Décorateur A : Le Chronomètre ---
class DecorateurChrono : public DecorateurRegle {
    public:
        explicit DecorateurChrono(std::unique_ptr<RegleValidation> regle) : DecorateurRegle(std::move(regle)) {}

        bool evaluer() const override {
            const auto debut = std::chrono::high_resolution_clock::now();

            const bool resultat = DecorateurRegle::evaluer(); // Appelle la règle interne

            const auto fin = std::chrono::high_resolution_clock::now();
            std::cout << "  [Chrono] Le test a pris "
                      << std::chrono::duration_cast<std::chrono::microseconds>(fin - debut).count()
                      << " microsecondes.\n";
            return resultat;
        }
};

// --- Décorateur B : Le Logger ---
class DecorateurLog : public DecorateurRegle {
public:
    explicit DecorateurLog(std::unique_ptr<RegleValidation> regle)
        : DecorateurRegle(std::move(regle)) {}

    bool evaluer() const override {
        std::cout << "  [Log] Démarrage de l'évaluation de la règle...\n";
        const bool resultat = DecorateurRegle::evaluer();
        std::cout << "  [Log] Résultat de l'évaluation : " << (resultat ? "SUCCÈS" : "ÉCHEC") << "\n";
        return resultat;
    }
};


// l'usine (factory)  qui assemble tout
class FabriqueRegles {
public:
    // La Factory prend le type de base, et des drapeaux (flags) pour les décorateurs
    static std::unique_ptr<RegleValidation> creerRegleHorlogere(const std::string& type, const bool avecLog, const bool avecChrono) {

        std::unique_ptr<RegleValidation> regle_finale;

        // 1. Création de la règle de base
        if (type == "Etancheite") {
            regle_finale = std::make_unique<RegleEtancheite>();
        } else {
            return nullptr;
        }

        // 2. Emballage dynamique selon les options (L'ordre d'emballage est important !)
        if (avecLog) {
            regle_finale = std::make_unique<DecorateurLog>(std::move(regle_finale));
        }

        if (avecChrono) {
            regle_finale = std::make_unique<DecorateurChrono>(std::move(regle_finale));
        }

        return regle_finale;
    }
};

// =========================================================
// 5. UTILISATION DANS LE MAIN
// =========================================================
inline void decorator_pattern_use() {
    std::cout << "Création d'une règle avec Log ET Chrono depuis la Factory :\n";

    if (const auto regle_complexe = FabriqueRegles::creerRegleHorlogere("Etancheite", true, true)) {
        const std::string v = regle_complexe->evaluer() ? "Okay" : "Recallé";
        std::println(std::cout, "{}", v);
    }

}




#endif //TUTO_CPP_STRUCTURAL_PATTERNS_H
