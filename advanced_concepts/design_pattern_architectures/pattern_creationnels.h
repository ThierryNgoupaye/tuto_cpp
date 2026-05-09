//
// Created by thierry on 09/05/2026.
//

#ifndef TUTO_CPP_PATTERN_CREATIONNELS_H
#define TUTO_CPP_PATTERN_CREATIONNELS_H

#include <iostream>
#include <print>
#include <fstream>
#include <mutex>
#include <filesystem>


using std::cout;
using string = std::string;
using mutex = std::mutex;

namespace fs = std::filesystem;

/**
 * Comment créer des objets de facon propre et flexible ?
 */



// Pattern Singleton
class SingletonPattern {

    public:
        // Point d'accès unique
        static SingletonPattern& get_instance() {
            static SingletonPattern instance;
            return instance;
        }

        void direBonjour() const {
            std::println(cout, "Bonjour depuis le Singleton");
        }

        // Interdire les constructeurs de copie
        SingletonPattern(const SingletonPattern&) = delete;
        SingletonPattern& operator=(const SingletonPattern&) = delete;

        // On peut interdire les constructeurs de move…
        SingletonPattern(SingletonPattern&&) = delete;
        SingletonPattern& operator=(SingletonPattern&&) = delete;


    private:
        SingletonPattern() {
            std::println(cout, "Singleton créé !");
        }
};


inline void simple_singleton_pattern_use() {
    SingletonPattern::get_instance().direBonjour();
    SingletonPattern::get_instance().direBonjour();   // Meme instance chaque fois
}


// Exemple d'un gestionnaire de logs
class Logger {

    std::ofstream m_fichier;
    mutex m_mutex;

    Logger() : m_fichier(calculer_chemin_log())  {
        if (!m_fichier.is_open()) {
            std::cerr << "Erreur critique : Impossible d'initialiser le Logger." << std::endl;
        }
    }

    static fs::path calculer_chemin_log() {
        const fs::path fichier_source = __FILE__;
        const fs::path parent = fichier_source.parent_path().parent_path().parent_path();
        return parent / "app.log";
    }

    public:
        static Logger& get_instance() {
            static Logger logger;
            return logger;
        }

        void log(const string& message) {
            std::lock_guard lock(m_mutex);
            m_fichier << std::chrono::system_clock::now() <<" [INFO] " << message << std::endl;
            std::println("[INFO] {}", message);
        }

        void erreur(const string& message) {
            std::lock_guard lock(m_mutex);
            m_fichier << std::chrono::system_clock::now() << " [ERR] " << message << std::endl;
            std::println("[ERR] {}", message);
        }

        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;
};


#define LOG(msg)     Logger::get_instance().log(msg)
#define ERREUR(msg)  Logger::get_instance().erreur(msg)



inline void logger_singleton_pattern_use() {

    LOG("Application démarrée");
    LOG("Chargement de la caméra...");
    ERREUR("Caméra introuvable !");
    LOG("Application fermée");
}



// Pattern Factory
class Animal_ {
public:
    virtual void parler() const = 0;
    virtual ~Animal_() = default;
};

class Chien_ : public Animal_{
public:
    void parler() const override {
        std::println("Woof !");
    }
};

class Chat_: public Animal_ {
public:
    void parler() const override {
        std::println("Miaou !");
    }
};


class AnimalFactory {
public:
    static std::unique_ptr<Animal_> creer(const string& type) {
        if (type == "chien") return std::make_unique<Chien_>();
        if (type == "chat") return std::make_unique<Chat_>();
        return nullptr;
    }
};


inline void factory_pattern_use() {
    string type;
    std::cin >> type;

    if (const auto animal = AnimalFactory::creer(type)) animal->parler();
}



#include <memory>
#include <string>
#include <unordered_map>
#include <functional>



// Interface
class Filtre {
public:
    virtual void appliquer() const = 0;
    virtual std::string nom() const = 0;
    virtual ~Filtre() = default;
};


// Filtres concrets
class FiltreFlou : public Filtre {
public:
    explicit FiltreFlou(const int intensite) : m_intensite(intensite) {}
    void appliquer() const override {
        std::cout << "Filtre flou x" << m_intensite << std::endl;
    }
    std::string nom() const override { return "Flou"; }
private:
    int m_intensite;
};


class FiltreContour : public Filtre {
public:
    void appliquer() const override {
        std::cout << "Filtre contour" << std::endl;
    }
    std::string nom() const override { return "Contour"; }
};


class FiltreNB : public Filtre {
public:
    void appliquer() const override{
        std::cout << "Filtre noir et blanc" << std::endl;
    }
    std::string nom() const override { return "NB"; }
};


// Factory avec registre dynamique
class FiltreFactory {
public:
    using Createur = std::function<std::unique_ptr<Filtre>()>;

    // Enregistrer un filtre
    static void enregistrer(const std::string& nom, Createur createur) {
        registre()[nom] = std::move(createur);
    }

    // Créer un filtre par nom
    static std::unique_ptr<Filtre> creer(const std::string& nom) {
        if (const auto it = registre().find(nom); it != registre().end()) {
            return it->second();
        }
        return nullptr;
    }

    // Lister les filtres disponibles
    static void listerFiltres() {
        std::cout << "Filtres disponibles :" << std::endl;
        for (const auto &nom: registre() | std::views::keys) {
            std::cout << "  → " << nom << std::endl;
        }
    }

private:
    static std::unordered_map<std::string, Createur>& registre() {
        static std::unordered_map<std::string, Createur> r;
        return r;
    }
};

inline void factory_pattern_filter_use() {
    // Enregistrer les filtres disponibles
    FiltreFactory::enregistrer("flou", [] { return std::make_unique<FiltreFlou>(5); });
    FiltreFactory::enregistrer("contour",[] { return std::make_unique<FiltreContour>(); });
    FiltreFactory::enregistrer("nb",[] { return std::make_unique<FiltreNB>(); });

    FiltreFactory::listerFiltres();

    // Créer depuis un choix utilisateur
    const std::string choix = "flou";
    if (const auto filtre = FiltreFactory::creer(choix)) filtre->appliquer();
}



// Pattern Builder

class ConfigFenetre {

    std::string m_titre = "Mon App";
    int m_largeur = 800;
    int m_hauteur = 600;
    bool m_resizable = true;
    bool m_fullscreen = false;
    std::string m_theme = "dark";

    // Constructeur privé : on FORCE l'utilisation du Builder
    ConfigFenetre() = default;

    public:

        [[nodiscard]] const std::string& getTitre() const { return m_titre; }
        [[nodiscard]] int getLargeur() const { return m_largeur; }
        [[nodiscard]] int getHauteur() const { return m_hauteur; }
        [[nodiscard]] const std::string& getTheme() const { return m_theme; }


        class Builder;

        static Builder builder();
};



class ConfigFenetre::Builder {

    ConfigFenetre m_config;

    public:
        Builder& titre(std::string t) {
            m_config.m_titre = std::move(t);
            return *this;
        }

        Builder& taille(const int w, const int h) {
            m_config.m_largeur = w;
            m_config.m_hauteur = h;
            return *this;
        }

        Builder& resizable(const bool r) {
            m_config.m_resizable = r;
            return *this;
        }

        Builder& fullscreen() {
            m_config.m_fullscreen = true;
            return *this;
        }

        Builder& theme(std::string t) {
            m_config.m_theme = std::move(t);
            return *this;
        }

        [[nodiscard]] ConfigFenetre build() {
            if (m_config.m_largeur <= 0 || m_config.m_hauteur <= 0) {
                throw std::invalid_argument("Taille invalide !");
            }
            return std::move(m_config);
        }
};


inline ConfigFenetre::Builder ConfigFenetre::builder() {
    return Builder{};
}



inline void builder_factory_use() {
    const ConfigFenetre config = ConfigFenetre::builder()
        .titre("Mon App de Traitement d'Image")
        .taille(1920, 1080)
        .resizable(false)
        .theme("dark")
        .build();

    std::cout << config.getTitre() << std::endl;
    std::cout << config.getLargeur() << "x" << config.getHauteur() << std::endl;
    std::cout << config.getTheme() << std::endl;
}



// Alternative c++ avec les Design Initializers

// 1. On déclare juste une structure avec des valeurs par défaut
struct ConfigFenetre_ {
    std::string titre = "Mon App";
    int largeur = 800;
    int hauteur = 600;
    bool resizable = true;
    bool fullscreen = false;
    std::string theme = "dark";
};

// 2. C'est tout. Dans le main, on utilise les Designated Initializers !
inline void designed_initialized_use() {
    const ConfigFenetre_ config = {
        .titre = "Mon App de Traitement d'Image",
        .largeur = 1920,
        .hauteur = 1080,
        .resizable = false,
        .fullscreen = true,
        .theme = "dark",
    };

    std::cout << config.titre << std::endl;
}




#endif //TUTO_CPP_PATTERN_CREATIONNELS_H
