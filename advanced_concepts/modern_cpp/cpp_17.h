//
// Created by thierry on 11/05/2026.
//

#ifndef TUTO_CPP_STRUCTURED_BINDINGS_H
#define TUTO_CPP_STRUCTURED_BINDINGS_H

#include <iostream>
#include <tuple>
#include <map>
#include <string>
#include <filesystem>
#include <fstream>
#include <vector>
#include <numeric>
#include <execution>
#include <algorithm>
#include <cmath>
#include  <cstdint>


namespace fs = std::filesystem;



// Structured Bindings
struct S_Pixel {
    int x, y;
    float valeur;
};

inline S_Pixel trouverMaximum() {
    return {320, 240, 0.95f};
}



inline void simple_structured_bindings() {
    std::pair<int, std::string> p = {42, "Alice"};


    // avant
    int age1 = p.first;
    std::string nom1 = p.second;

    // c++ moderne
    auto [age, nom] = p;
    std::cout << age << " " << nom << std::endl;


    // avec std::tuple
    auto t = std::make_tuple(1, 3.14f, "bonjour");
    auto [entier, flottant, texte] = t;
    std::cout << entier << " " << flottant << " " << texte  << std::endl;


    // avec std::map
    std::map<std::string, int> ages {
        {"Alice", 30}, {"Bob", 25}
    };

    for (const auto& [nom_, age_]: ages) {
        std::cout << nom_ << " : "  << age_ << std::endl;
    }


    auto [x, y,valeur] = trouverMaximum();
    std::cout << "Max à (" << x << "," << y << ")" << " valeur=" << valeur << std::endl;


    S_Pixel pi = {10,20,0.5f};
    auto& [px, py, pz] = pi;
    px = 100;
    std::cout << pi.x << std::endl;
}

inline std::optional<int> chargerImage(const std::string& fichier) {
    if (fichier == "photo.jpg") return 1020 * 1000 ;
    return std::nullopt;
}

inline void if_else_initializer() {
    std::map<std::string, int> ages = {
        {"Alice", 30},
        {"Bob", 25}
    };

    // ancienne facon
    auto it = ages.find("Alice");
    if (it != ages.end()) {
        std::cout << it->first << " " << it->second << std::endl;
    }


    // moderne
    if (const auto ite = ages.find("Alice"); ite != ages.end()) {
        std::cout << ite->first << " " << ite->second << std::endl;
    }


    // avec switch
    constexpr int code = 42;
    switch (constexpr auto result = code *2; result) {
        case 84:
            std::cout << "84" << std::endl;
            break;
        default:
            std::cout << "Autre " << std::endl;
    }



    if (const auto taille = chargerImage("photo.jpg"); taille) {
        std::cout << "Image chargée " << *taille << " pixels"  << std::endl;
    }
    else {
        std::cout << "Erreur chargement !" << std::endl;
    }
}





// File system
inline void basic_filesystem_use() {

    fs::path p1("/home/thierry/projets/polytech-dijon/tuto_cpp/photo.jpg");

    const fs::path p2 = "/home/thierry";

    fs::path p3 = p2 / "images" / "photo.jpg";


    // décomposer un chemin
    const fs::path chemin("/home/thierry/images/photo.jpg");

    std::cout << "Chemin complet : " << chemin << std::endl;
    std::cout << "Nom du fichier : " << chemin.filename() << std::endl;
    std::cout << "Extension : " << chemin.extension().string() << std::endl;
    std::cout << "Stem (sans extension)" << chemin.stem().string() << std::endl;
    std::cout << "Dossier parent : " << chemin.parent_path() << std::endl;
    std::cout << "Racine : " << chemin.root_path() << std::endl;



    // Modifier un chemin
    fs::path p = "/home/thierry/photo/projets/polytech-dijon/tuto_cpp/photo.jgp";

    // changer l'extension
    p.replace_extension(".png");
    std::cout << p << std::endl;

    // changer le nom du fichier
    p.replace_filename("autre.jpg");
    std::cout << p << std::endl;

    //iterer sur les composants du chemin
    for (const auto& composant : p) std::cout << composant;



    // Vérifications sur le chemin
    std::cout << fs::exists(p) << std::endl;
    std::cout << fs::is_directory(p) << std::endl;
    std::cout << fs::is_regular_file(p) << std::endl;
    std::cout << fs::is_symlink(p) << std::endl;
    std::cout<<fs::is_empty(p) << std::endl;


    // information sur un fichier
    if (const fs::path fichier = "homme/thierry/projets/polytech-dijon/tuto_cpp/photo.jpg"; fs::exists(fichier)) {
        const auto taille = fs::file_size(fichier);
        std::cout << "Taille : " << taille  << " octets" << std::endl;

        const auto modif = fs::last_write_time(fichier);
        std::cout << modif << std::endl;

        // permission
        const auto permission = fs::status(fichier).permissions();
        std::cout << "Lisible : " << ((permission & fs::perms::owner_read) != fs::perms::none) << std::endl;
    }



    fs::path relatif = "toto_cpp/photo.jpg";
    fs::path absolu = fs::absolute(relatif);

    fs::path canonique = fs::canonical("/home/thierry/../thierry/photo.jpg");



    // operations sur les fichiers et dossier
    fs::path base = fs::temp_directory_path() / "test_fs";


    // créer des dossiers
    fs::create_directory(base);  // crée un seul dossier
    fs::create_directories(base / "sous" / "dossiers");  // crée aussi tous les dossiers intermédiaires


    // créer des fichiers
    std::ofstream(base / "fichier.txt") << "contenu";
    std::ofstream(base / "autre.jpg").close();



    //copier
    fs::copy_file(base/"fichier.txt", base / "copie2.txt");

    // copie avec options
    fs::copy_file(base / "fichier.txt", base / "copie2.txt", fs::copy_options::overwrite_existing);    //écrase si existe déjà

    //copier un dossier entier
    fs::copy(base, fs::temp_directory_path() / "backup", fs::copy_options::recursive);


    // déplacer / renommer
    fs::rename(base / "fichier.txt", base / "renomme.txt");


    // supprimer
    fs::remove(base / "autre.txt");

    //supprimer le dossier et son contenu
    fs::remove_all(base);


    // Parcourir les dossiers
    fs::path dossier = "home/thierry/projets/tuto_cpp";


    // parcours simple, non récursif
    for (auto& entree : fs::directory_iterator(dossier)) {
        std::cout << entree.path().filename() << std::endl;
        if (entree.is_directory()) std::cout << "/";
        std::cout << std::endl;
    }



    // parcours récursif avec gestion erreurs
    constexpr auto options = fs::directory_options::skip_permission_denied;
    for ( auto& entree: fs::recursive_directory_iterator(dossier, options)) {
        std::cout << entree.path().filename() << std::endl;
    }


    // Parcours avec filtre et tri
    std::vector<fs::path> fichiersCpp;

    for (auto& entree : fs::recursive_directory_iterator(dossier, options)) {
        if (entree.is_regular_file() & entree.path().extension() == ".cpp") {
            fichiersCpp.emplace_back(entree.path());
        }
    }

    std::ranges::sort(fichiersCpp);

    std::cout << "\nFichiers .cpp trouvés :" << std::endl;
    for (auto& ch : fichiersCpp) {
        std::cout << "  " << ch.filename() << " (" << fs::file_size(ch) << " o)" << std::endl;
    }
}


// cas pratique - Gestionnaire de fichiers images
struct InfoImage {
    fs::path chemin;
    uintmax_t taille;
    std::string extension;
};

class GestionnaireImages {
    fs::path m_dossier;
    std::vector<InfoImage> m_images;

public:
    explicit GestionnaireImages(fs::path dossier) : m_dossier(std::move(dossier)) {}

    void scanner() {
        m_images.clear();


        if (!fs::exists(this->m_dossier)) {
            std::println("Dossier introuvable : {}", this->m_dossier.string());
        }


        for (auto& entree: fs::recursive_directory_iterator(m_dossier, fs::directory_options::skip_permission_denied)) {
            if (!entree.is_regular_file()) continue;

            auto ext = entree.path().extension().string();

            std::ranges::transform(ext, ext.begin(),  tolower);

            if (ext == ".jpg" || ext == ".jpeg" || ext == ".png" || ext == ".bmp"  ||ext == ".tiff"|| ext == ".webp") {
                m_images.push_back({
                    entree.path(),
                    fs::file_size(entree.path()),
                    ext
                });
            }
        }

        std::ranges::sort(m_images, [](const InfoImage& a, const InfoImage& b) {
            return a.taille > b.taille;
        });
    }


    void afficherStats() const {
        std::println("=== Statistiques ===");
        std::println("Total images : {}", m_images.size());

        // Taille totale
        uintmax_t total = 0;
        for (auto& img : m_images) total += img.taille;
        std::println("Taille totale : {} Mo", total / (1024 * 1024));

        // Par extension
        std::map<std::string, int> parExt;
        for (auto& img : m_images) parExt[img.extension]++;

        std::println("\nPar format :");
        for (auto& [ext, count] : parExt) {
            std::println("  {} : {} fichiers", ext, count);
        }

        // Top 5 plus grandes
        std::println("\nTop 5 plus grandes :");
        const int n = std::min(5, static_cast<int>(m_images.size()));
        for (int i = 0; i < n; i++) {
            std::println("  {} ({} Ko)",m_images[i].chemin.filename().string(), m_images[i].taille / 1024);
        }
    }


    void organiserParExtension(const fs::path& destination) const {
        fs::create_directories(destination);

        for (auto& img : m_images) {
            // Créer sous-dossier par extension
            auto sousDir = destination / img.extension.substr(1);
            // substr(1) → enlève le point → "jpg" au lieu de ".jpg"
            fs::create_directories(sousDir);

            // Copier le fichier
            auto dest = sousDir / img.chemin.filename();

            // Éviter les collisions de noms
            if (fs::exists(dest)) {
                auto stem = img.chemin.stem().string();
                auto ext  = img.chemin.extension().string();
                int  n    = 1;
                do {
                    dest = sousDir /
                           (stem + "_" + std::to_string(n++) + ext);
                } while (fs::exists(dest));
            }

            fs::copy_file(img.chemin, dest,
                fs::copy_options::skip_existing);

            std::println("Copié : {} → {}",
                img.chemin.filename().string(),
                sousDir.filename().string());
        }
    }


    void organiserParExtension(const fs::path& destination) {
        fs::create_directories(destination);

        for (auto& img : m_images) {
            // Créer sous-dossier par extension
            auto sousDir = destination / img.extension.substr(1);
            // substr(1) → enlève le point → "jpg" au lieu de ".jpg"
            fs::create_directories(sousDir);

            // Copier le fichier
            auto dest = sousDir / img.chemin.filename();

            // Éviter les collisions de noms
            if (fs::exists(dest)) {
                auto stem = img.chemin.stem().string();
                auto ext  = img.chemin.extension().string();
                int  n    = 1;
                do {
                    dest = sousDir /
                           (stem + "_" + std::to_string(n++) + ext);
                } while (fs::exists(dest));
            }

            fs::copy_file(img.chemin, dest,
                fs::copy_options::skip_existing);

            std::println("Copié : {} → {}",
                img.chemin.filename().string(),
                sousDir.filename().string());
        }
    }
};

inline void image_handle_use() {
    GestionnaireImages gestionnaire("/home/thierry/projets/tuto_cpp");
    gestionnaire.scanner();
    gestionnaire.afficherStats();

    // Organiser dans un dossier temporaire
    gestionnaire.organiserParExtension(fs::temp_directory_path() / "images_organisées");
}




// Parallel STL
// il existe trois facon d'exécuter les algos de la SLT, séquentiellement, parallèlement, ou de facon vectorisé

double mesurer_temps_execution(auto&& function) {
    const auto debut = std::chrono::high_resolution_clock::now();
    function();
    const auto fin  = std::chrono::high_resolution_clock::now();
    return static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(fin-debut).count());
}

inline void parallel_stl_algo() {
    constexpr size_t N = 100'000'000;
    std::vector v(N, 1.0f);


    // éxecution séquentielle
    const double sequential_t1 = mesurer_temps_execution([&] {
       std::for_each(std::execution::seq, v.begin(), v.end(), [](float& x) {
           x = x * 2.0f + 1.0f;
       }) ;
    });


    // execution parallele (sur tous les coeurs)
    const double parallel_t2 = mesurer_temps_execution([&] {
       std::for_each(std::execution::par, v.begin(), v.end(), [](float& x) {
           x = x * 2.0f + 1.0f;
       });
    });


    // par_unseq = parallel + SIMD
    const double parallel_simd_t3 = mesurer_temps_execution([&] {
        std::for_each(std::execution::par_unseq, v.begin(), v.end(), [](float& x) {
            x = x * 2.0f + 1.0f;
        });
    });

    // unseq = SIMD (vectorisé)
    const double simd_t3 = mesurer_temps_execution([&] {
       std::for_each(std::execution::unseq, v.begin(), v.end(), [](float& x) {
           x = x * 2.0f + 1.0f;
       });
   });


    std::cout << "seq        : " << sequential_t1 << " ms" << std::endl;
    std::cout << "par        : " << parallel_t2 << " ms" << std::endl;
    std::cout << "unseq  : " << simd_t3 << " ms" << std::endl;
    std::cout << "par_unseq  : " << parallel_simd_t3 << " ms" << std::endl;
    std::cout << "Gain par   : " << sequential_t1/parallel_t2 << "x" << std::endl;
    std::cout << "Gain par_unseq : " << sequential_t1/parallel_simd_t3 << "x" << std::endl;
    std::cout << "Gain unseq : " << sequential_t1/simd_t3 << "x" << std::endl;

}





// cas pratique traitement d'image parallèle

struct P_Pixel {
    u_int8_t r, g, b;
};

// convertir en niveaux de gris
inline uint8_t versGris(const P_Pixel& p) {
    return p.r * 0.299f + p.g* 0.587f + p.b * 0.114f;
}


// appliquer un gamma
inline uint8_t appliquerGamma(const uint8_t val, const float gama) {
    return std::pow(val / 255.0f, gama) * 255.0f;
}


inline void traiter_image_parallel() {
    constexpr  int W = 1920, H = 1080;
    std::vector<P_Pixel> image(W*H);

    // remplir avec des pixels aléatoires
    std::generate(std::execution::par,
        image.begin(),
        image.end(),
        [] ->  P_Pixel{
            return {
                static_cast<uint8_t>(rand() % 256),
                static_cast<uint8_t>(rand() % 256),
                static_cast<uint8_t>(rand() % 256),
            };
        }
    );


    std::vector<uint8_t> gris(W*H);

    const auto t1 = mesurer_temps_execution([&] {
       std::transform(std::execution::par,
           image.begin(),
           image.end(),
           gris.begin(),
           versGris
        );
    });


    const auto t2 = mesurer_temps_execution([&] {
        std::transform(std::execution::par_unseq,
            image.begin(),
            image.end(),
            gris.begin(),
            versGris
        );
    });


    std::cout << "Conversion gris seq       : " << t1 << " ms" << std::endl;
    std::cout << "Conversion gris par_unseq : " << t2 << " ms" << std::endl;



    // Correction gama parallèle
    float gamma = 2.2f;
    const auto t3 = mesurer_temps_execution([&] {
        std::transform(std::execution::par_unseq,
            gris.begin(),
            gris.end(),
            gris.begin(),
            [gamma](const uint8_t v) {
                return appliquerGamma(v, gamma);
            }
        );
    });


    std::cout << "Correction gamma par_unseq: " << t3 << " ms" << std::endl;


    // statistiques
    auto [minIt, maxIt] = std::minmax_element(
       std::execution::par,
       gris.begin(),
       gris.end()
    );

    const float moyenne = std::reduce(
        std::execution::par,
        gris.begin(),
        gris.end(),
        0.0f
    ) / gris.size();

    std::cout << "Min     : " << static_cast<int>(*minIt) << std::endl;
    std::cout << "Max     : " << static_cast<int>(*maxIt) << std::endl;
    std::cout << "Moyenne : " << moyenne     << std::endl;

}



// seq :
// → débogage → comportement déterministe
// → petites collections → overhead threads > gain
// → lambda avec état partagé complexe
// → opérations non parallélisables
//
// par :
// → collections > 10 000 éléments
// → lambda thread-safe (atomic ou sans état partagé)
// → opérations indépendantes entre éléments
// → traitement d'images, calcul scientifique
//
// par_unseq :
// → comme par MAIS lambda encore plus stricte
// → opérations pures uniquement
// → pas de malloc, mutex, I/O
// → quand tu veux le maximum de performance
// → traitement pixel par pixel ← ton cas !
//
// Règle pratique :
// → commence par seq → fonctionne correctement ?
// → passe à par → même résultat ?
// → passe à par_unseq → lambda pure ?
// → mesure → garde la politique optimale









#endif //TUTO_CPP_STRUCTURED_BINDINGS_H
