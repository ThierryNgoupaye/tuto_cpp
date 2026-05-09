//
// Created by thierry on 09/05/2026.
//

#ifndef TUTO_CPP_BEHAVIOR_PATTERN_H
#define TUTO_CPP_BEHAVIOR_PATTERN_H

#include <iostream>
#include <vector>
#include  <functional>
#include <string>

using string = std::string;
using std::vector;




/**
 * Pattern comportementaux.
 * Comment les objets communiquent-ils et se repartissent-ils les responsabilités ?
 */



// Pattern Observer.
// Plusieurs objets doivent réagir selon le comportement d'un autre

struct Frame {
    int numero;
    string data;
};



class Camera {
    public:
        using FrameCallback = std::function<void(const Frame&)>;

        void abonner(FrameCallback cb) {
            m_callbacks.push_back(std::move(cb));
        }

        void capturer() const {
            static int numero = 0;
            Frame frame{numero++ , "donnees_image_" + std::to_string(numero)};

            std::println(std::cout, "[Camera] Frame {} capturée ", frame.numero);

            for (auto& cb : m_callbacks) {
                cb(frame);
            }
        }

    private:
        vector<FrameCallback> m_callbacks;
};


inline void observer_pattern_callback() {
    Camera camera;

    // Thread traitement → s'abonne
    camera.abonner([](const Frame& f) {std::cout << "  [Traitement] Filtre sur frame "<< f.numero << std::endl;});

    // Thread affichage → s'abonne
    camera.abonner([](const Frame& f) {std::cout << "  [Affichage] Affiche frame "<< f.numero << std::endl;});

    // Thread enregistrement → s'abonne
    camera.abonner([](const Frame& f) {std::cout << "  [Enregistrement] Sauve frame "<< f.numero << std::endl;});

    camera.capturer();
    camera.capturer();
    camera.capturer();
}



// Moderne avec les weak_ptr

// Contrat de l'observateur
class IObservateur {
public:
    virtual ~IObservateur() = default;
    virtual void onTemperatureChangee(float nouvelle_temp) = 0;

};

// Sujet (source de vérité)
class StationMeteo {
    float m_temperature = 0.0f;
    vector<std::weak_ptr<IObservateur>> m_abonnes;

    void notifierAbonnes() {
        for (auto it = m_abonnes.begin(); it != m_abonnes.end(); ++it) {
            if (const auto abonne_vivant = it->lock()) {
                abonne_vivant->onTemperatureChangee(m_temperature);
                ++it;
            } else {
                std::println(cout, "[Sujet] Un abonné a disparu, nettoyage de la liste...");
                it = m_abonnes.erase(it);
            }
        }
    }

    public:
        void ajouterAbonne(std::shared_ptr<IObservateur> abonne) {
            m_abonnes.emplace_back(abonne);
        }

        void setTemperature(const float temperature) {
            m_temperature = temperature;
            notifierAbonnes();
        }
};


class WidgetAffichage : public IObservateur {
    string m_nom;
    public:
        explicit WidgetAffichage(string nom) : m_nom(std::move(nom)) {}

        void onTemperatureChangee(const float temp) override {
            std::println(cout, "[Widget {}] Mise à jour de l'affichage : {}", m_nom, temp);
        }
};


inline void custom_observer_pattern_use() {
    StationMeteo station;
    const auto widget_mobile = std::make_shared<WidgetAffichage>("Mobile");
    station.ajouterAbonne(widget_mobile);
    station.setTemperature(25.5f);

    {
        auto widget_web = std::make_shared<WidgetAffichage>("Web");
        station.ajouterAbonne(widget_mobile);

        station.setTemperature(26.0f);
    }

    std::println(cout, "--- Après la fermeture du widget Web ---");

    station.setTemperature(22.0f);
}











// Pattern Strategy
// Un algorithme peut varier selon le contexte, on peut donc avoir plusieurs facons de faire
// la meme chose. Il faudrait qu'au runtime l'algorithme soit choisie, sans modifier le code qui l'utilise


// Version Orientée Objet
class StrategyNLP {
public:
    virtual ~StrategyNLP() = default;
    virtual void extraireInfos(const string& texte) const = 0;
};


class ExtractionMotsCles : public StrategyNLP {
public:
    void extraireInfos(const string& texte) const override {
        std::println(cout, "[Stratégie A] comptage des mots fréquents dans : {}", texte);
    }
};


class ExtractionEntitesNommees : public StrategyNLP {
public:
    void extraireInfos(const std::string& texte) const override {
        std::println( "[Stratégie B] Recherche de noms de maladies/plantes dans :  {}",texte);
    }
};


class PipelineAnalyse {

    std::unique_ptr<StrategyNLP> m_strategie;
    public:
        explicit PipelineAnalyse(std::unique_ptr<StrategyNLP> strategie_initiale) : m_strategie(std::move(strategie_initiale)) {}

        void setStrategy(std::unique_ptr<StrategyNLP> nouvelle_strategie) {
                m_strategie = std::move(nouvelle_strategie);
        }

        void traiterTexte(const std::string& texte) const {
            if (m_strategie) {
                m_strategie->extraireInfos(texte);
            }
            else {
               std::println(std::cerr, "Erreur: Aucune stratégie d'analyse définie !");
            }
        }
};


inline void strategy_pattern_with_poo_use() {

    constexpr std::string_view rapport = "Le blé présente des traces de rouille jaune.";
    const PipelineAnalyse pipeline(std::make_unique<ExtractionMotsCles>());
    pipeline.traiterTexte(rapport.data());

}



// approche fonctionnelle
class PipelineAnalyseModerne {
public:
    using StrategyFunc = std::function<void(const string&)>;

private:
    StrategyFunc m_strategy;
public:
    explicit PipelineAnalyseModerne(StrategyFunc strategie_initiale) : m_strategy(std::move(strategie_initiale)) {}

    void setStrategy(StrategyFunc nouvelle_strategy) {
        m_strategy = std::move(nouvelle_strategy);
    }

    void traiterTexte(const string& texte) const {
        if (m_strategy) {
            m_strategy(texte); // Appel direct
        }
    }
};


inline void strategy_pattern_with_functions() {

    constexpr  std::string_view rapport = "Sécheresse détectée sur la parcelle B.";

    PipelineAnalyseModerne pipeline([](const string& texte) {
       std::println(cout, "[Lambda 1] Traitement rapide : {}", texte);
    });

    pipeline.traiterTexte(rapport.data());

    pipeline.setStrategy([](const std::string& txt) {
        std::println(cout, "[Lambda 2] Deep Learning appliqué sur : {}", txt);
    });

    pipeline.traiterTexte(rapport.data());
}




#endif //TUTO_CPP_BEHAVIOR_PATTERN_H
