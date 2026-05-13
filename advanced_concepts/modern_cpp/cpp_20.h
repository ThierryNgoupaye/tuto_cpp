//
// Created by thierry on 13/05/2026.
//

#ifndef TUTO_CPP_CPP_20_H
#define TUTO_CPP_CPP_20_H

#include <iostream>
#include <chrono>
#include <compare>
#include  <string>
#include <vector>
#include <algorithm>
#include <ranges>
#include <filesystem>
#include <format>
#include <fstream>




using namespace std::chrono;
namespace fs = std::filesystem;



// L'opérateur <=>


struct A_Point {
    int x, y;
    auto operator<=>(const A_Point &a) const = default;  // genere automatiquement ==, !=, <, > <=, >=
};

inline void multi_operator_use() {
    constexpr int a = 5, b = 10;

    auto resultat = a <=> b;

    // Tester le résultat
    if (resultat < 0)
        std::cout << "a < b" << std::endl;
    else if (resultat == 0)
        std::cout << "a == b" << std::endl;
    else
        std::cout << "a > b" << std::endl;

    // Avec std::is_lt, std::is_eq, std::is_gt
    std::cout << std::is_lt(a <=> b) << std::endl;  // 1
    std::cout << std::is_eq(a <=> b) << std::endl;  // 0
    std::cout << std::is_gt(a <=> b) << std::endl;  // 0
}





// strong_ordering → égal = identique
struct Entier {
    int valeur;
    std::strong_ordering operator<=>(const Entier& o) const {
        return valeur <=> o.valeur;
    }
    bool operator==(const Entier& o) const = default;
};


// weak_ordering → égal = équivalent (pas identique !)
struct MotInsensible {
    std::string mot;

    std::weak_ordering operator<=>(const MotInsensible& o) const {
        // Compare sans tenir compte de la casse
        std::string a = mot, b = o.mot;
        std::ranges::transform(a, a.begin(), ::tolower);
        std::ranges::transform(b, b.begin(), ::tolower);

        if (a < b) return std::weak_ordering::less;
        if (a > b) return std::weak_ordering::greater;
        return std::weak_ordering::equivalent;
        // → "Alice" et "alice" sont ÉQUIVALENTS,
        //   mais pas identiques !
    }
    bool operator==(const MotInsensible& o) const {
        std::string a = mot, b = o.mot;
        std::ranges::transform(a, a.begin(), ::tolower);
        std::ranges::transform(b, b.begin(), ::tolower);
        return a == b;
    }
};

// partial_ordering → NaN incomparable !
struct Flottant {
    float valeur;
    std::partial_ordering operator<=>(const Flottant& o) const {
        return valeur <=> o.valeur;
        // NaN <=> NaN → unordered !
    }
    bool operator==(const Flottant& o) const = default;
};

inline void ordering_use() {
    // strong_ordering
    constexpr Entier e1{5}, e2{10};
    std::cout << (e1 < e2)  << std::endl;  // 1
    std::cout << (e1 == e2) << std::endl;  // 0

    const MotInsensible m1{"Alice"};
    // weak_ordering
    const MotInsensible m2{"alice"};
    std::cout << (m1 == m2) << std::endl;  // 1 → équivalents
    std::cout << std::is_eq(m1 <=> m2) << std::endl;  // 1

    constexpr Flottant f1{1.0f};
    // partial_ordering
    constexpr Flottant f2{std::numeric_limits<float>::quiet_NaN()};
    const auto res = f1 <=> f2;
    std::cout << std::is_lt(res) << std::endl;  // 0
    std::cout << std::is_gt(res) << std::endl;  // 0
    std::cout << std::is_eq(res) << std::endl;  // 0
    // → NaN est incomparable ! partial_ordering::unordered
}




struct InfoFichierImage {
    fs::path    chemin;
    uintmax_t   taille;
    std::string extension;

    // Comparaison par taille → pour trier facilement
    std::strong_ordering operator<=>(
        const InfoFichierImage& o) const {
        return taille <=> o.taille;
    }

    bool operator==(const InfoFichierImage& o) const {
        return chemin == o.chemin;
    }
};

inline void custom_ordering_use() {
    std::vector<InfoFichierImage> images = {
        {"photo1.jpg", 2048, ".jpg"},
        {"photo2.png", 512,  ".png"},
        {"photo3.bmp", 4096, ".bmp"},
        {"photo4.jpg", 1024, ".jpg"},
    };

    // Trier par taille croissante → fonctionne grâce à <=> !
    std::ranges::sort(images);

    std::cout << "Images triées par taille :" << std::endl;
    for (auto& img : images) {
        std::cout << img.chemin.string()<< " → " << img.taille << " Ko"<< std::endl;
    }
    // photo2.png → 512 Ko
    // photo4.jpg → 1024 Ko
    // photo1.jpg → 2048 Ko
    // photo3.bmp → 4096 Ko

    // Trouver la plus grande
    const auto max = std::ranges::max_element(images);
    std::cout << "Plus grande : " << max->chemin.string() << std::endl;
}




// utilisation de std::format

struct F_Pixel {
    uint8_t r, g, b, a;
};

// Spécialisation de std::formatter<Pixel>
template<>
struct std::formatter<F_Pixel> {
    // Options de format custom
    char mode = 'r';  // 'r' = RGB, 'h' = hex, 'd' = détaillé

    // parse → lit les options entre { et }
    constexpr auto parse(const std::format_parse_context& ctx) {
        auto it = ctx.begin();
        if (it != ctx.end() && *it != '}') {
            mode = *it++;  // lit le mode
        }
        return it;  // retourne la position après les options
    }

    // format → génère la string
    auto format(const F_Pixel& p, std::format_context& ctx) const {
        if (mode == 'h') {
            // Mode hex → #{RRGGBB}
            return std::format_to(ctx.out(),
                "#{:02X}{:02X}{:02X}", p.r, p.g, p.b);
        }
        if (mode == 'd') {
            // Mode détaillé
            return std::format_to(ctx.out(),
                                  "Pixel(r={}, g={}, b={}, a={})",
                                  p.r, p.g, p.b, p.a);
        }
        // Mode RGB par défaut
        return std::format_to(ctx.out(),
                              "rgb({}, {}, {})", p.r, p.g, p.b);
    }
};


inline void format_func_use() {


    // Utilisation simple avec cout:
    // Arguments positionnels
    std::cout << std::format("{} {} {}", 1, 2, 3) << std::endl;
    // → 1 2 3

    // Index explicites
    std::cout << std::format("{2} {0} {1}", "A", "B", "C") << std::endl;
    // → C A B

    // Réutiliser un argument
    std::cout << std::format("{0} {0} {1}", "ping", "pong") << std::endl;
    // → ping ping pong


    // < gauche (défaut pour strings)
    // > droite (défaut pour nombres)
    // ^ centré

    std::cout << std::format("|{:<10}|", "gauche") << std::endl;
    // |gauche    |

    std::cout << std::format("|{:>10}|", "droite") << std::endl;
    // |    droite|

    std::cout << std::format("|{:^10}|", "centre") << std::endl;
    // |  centre  |

    // Avec caractère de remplissage
    std::cout << std::format("|{:*<10}|", "gauche") << std::endl;
    // |gauche****|

    std::cout << std::format("|{:->10}|", "droite") << std::endl;
    // |----droite|

    std::cout << std::format("|{:=^10}|", "centre") << std::endl;
    // |==centre==|




    int n = 255;

    // Bases
    std::cout << std::format("{:d}", n)  << std::endl; // 255 décimal
    std::cout << std::format("{:b}", n)  << std::endl; // 11111111 binaire
    std::cout << std::format("{:o}", n)  << std::endl; // 377 octal
    std::cout << std::format("{:x}", n)  << std::endl; // ff hexadécimal
    std::cout << std::format("{:X}", n)  << std::endl; // FF hexadécimal majuscule

    // Avec préfixe
    std::cout << std::format("{:#b}", n) << std::endl; // 0b11111111
    std::cout << std::format("{:#o}", n) << std::endl; // 0377
    std::cout << std::format("{:#x}", n) << std::endl; // 0xff
    std::cout << std::format("{:#X}", n) << std::endl; // 0XFF

    // Largeur et zéros
    std::cout << std::format("{:8d}",  n) << std::endl; // "     255"
    std::cout << std::format("{:08d}", n) << std::endl; // "00000255"
    std::cout << std::format("{:+d}",  n) << std::endl; // "+255"
    std::cout << std::format("{: d}",  n) << std::endl; // " 255" (espace si positif)

    // Séparateur de milliers (C++23)
    int grand = 1234567;
    std::cout << std::format("{:_>12d}", grand) << std::endl; // ___1234567



    double pi = 3.14159265358979;

    // Précision
    std::cout << std::format("{:.2f}", pi)  << std::endl; // 3.14
    std::cout << std::format("{:.5f}", pi)  << std::endl; // 3.14159
    std::cout << std::format("{:.0f}", pi)  << std::endl; // 3

    // Types de notation
    std::cout << std::format("{:f}", pi)    << std::endl; // 3.141593 (fixe)
    std::cout << std::format("{:e}", pi)    << std::endl; // 3.141593e+00 (expo)
    std::cout << std::format("{:E}", pi)    << std::endl; // 3.141593E+00
    std::cout << std::format("{:g}", pi)    << std::endl; // 3.14159 (général)
    std::cout << std::format("{:g}", 0.000001) << std::endl; // 1e-06

    // Largeur + précision
    std::cout << std::format("{:10.3f}", pi) << std::endl; // "     3.142"
    std::cout << std::format("{:010.3f}",pi) << std::endl; // "000003.142"

    // Afficher un tableau de mesures proprement
    std::vector mesures = {1.5, 23.456, 0.001, 100.0};
    for (size_t i = 0; i < mesures.size(); i++) {
        std::cout << std::format("Mesure {:2d} : {:10.4f}", i+1, mesures[i])<< std::endl;
    }
    // Mesure  1 :     1.5000
    // Mesure  2 :    23.4560
    // Mesure  3 :     0.0010
    // Mesure  4 :   100.0000




    // format_to → écrit directement dans un itérateur
    // → évite de créer une string temporaire !
    std::string resultat;

    // Ajouter à la fin d'une string existante
    std::format_to(std::back_inserter(resultat), "Bonjour {} ! ", "Alice");
    std::format_to(std::back_inserter(resultat), "Tu as {} ans.", 30);

    std::cout << resultat << std::endl;
    // Bonjour Alice ! Tu as 30 ans.

    // format_to_n → limiter le nombre de caractères
    char buffer[20];
    auto [out, size] = std::format_to_n(buffer, 19,"Valeur : {}", 12345678);
    *out = '\0';  // terminer la string C
    std::cout << buffer << std::endl;
    // Valeur : 12345678 (tronqué si trop long)

    // formatted_size → connaître la taille avant d'allouer
    const size_t taille = std::formatted_size("Nom : {}, Age : {}","Alice", 30);
    std::cout << "Taille nécessaire : " << taille << std::endl;
    // → utile pour pré-allouer le buffer exact !




    F_Pixel rouge{255, 0, 0, 255};
    F_Pixel vert{0, 255, 0, 200};

    // Mode par défaut (r)
    std::cout << std::format("{}", rouge)  << std::endl;
    // rgb(255, 0, 0)

    // Mode hex (h)
    std::cout << std::format("{:h}", rouge) << std::endl;
    // #FF0000

    // Mode détaillé (d)
    std::cout << std::format("{:d}", vert)  << std::endl;
    // Pixel(r=0, g=255, b=0, a=200)

    // Dans un vecteur
    std::vector<F_Pixel> palette = {
        {255, 0,   0,   255},
        {0,   255, 0,   255},
        {0,   0,   255, 255}
    };

    for (auto& p : palette) {
        std::cout << std::format("  {:h}", p) << std::endl;
    }
    // #FF0000
    // #00FF00
    // #0000FF



    // std::format → format string connue à la compilation
    std::cout << std::format("Valeur : {}", 42) << std::endl;

    // std::vformat → format string connue au RUNTIME !
    const std::string formatStr = "Valeur : {}";  // runtime !
    int a = 42;
    std::cout << std::vformat(formatStr, std::make_format_args(a)) << std::endl;

    // Utile quand le format vient d'un fichier config
    // ou d'une entrée utilisateur !

}




// cas pratique

enum class NiveauLog { DEBUG, INFO, WARNING, ERROR };

inline std::string niveauVersString(const NiveauLog niveau) {
    switch (niveau) {
        case NiveauLog::DEBUG:   return "DEBUG  ";
        case NiveauLog::INFO:    return "INFO   ";
        case NiveauLog::WARNING: return "WARNING";
        case NiveauLog::ERROR:   return "ERROR  ";
    }
    return "UNKNOWN";
}

class ImageLogger {
public:
    static ImageLogger& getInstance() {
        static ImageLogger instance;
        return instance;
    }

    template<typename... Args>
    void log(const NiveauLog niveau, const std::string_view fmt, Args&&... args) {

        // Formater le message
        auto message = std::vformat(fmt, std::make_format_args(args...));
        // ↑ vformat → version runtime de format
        //   nécessaire avec des args variables !

        // Formater la ligne complète
        auto ligne = std::format("[{}] [{}] {}\n", horodatage(), niveauVersString(niveau),message);

        std::cout << ligne;
        m_fichier << ligne;
    }

    // Macros pratiques
    template<typename... Args>
    void debug(std::string_view fmt, Args&&... args) {
        log(NiveauLog::DEBUG, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void info(std::string_view fmt, Args&&... args) {
        log(NiveauLog::INFO, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void warning(std::string_view fmt, Args&&... args) {
        log(NiveauLog::WARNING, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void error(std::string_view fmt, Args&&... args) {
        log(NiveauLog::ERROR, fmt, std::forward<Args>(args)...);
    }

private:
    ImageLogger() : m_fichier("app.log") {}

    static std::string horodatage() {
        const auto now = system_clock::now();
        const auto t   = system_clock::to_time_t(now);
        char buf[20];
        std::strftime(buf, sizeof(buf), "%H:%M:%S", std::localtime(&t));
        return buf;
    }

    std::ofstream m_fichier;
};

#define LOG_DEBUG(fmt, ...)   ImageLogger::getInstance().debug(fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)    ImageLogger::getInstance().info(fmt, ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...) ImageLogger::getInstance().warning(fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...)   ImageLogger::getInstance().error(fmt, ##__VA_ARGS__)


inline void image_logger() {

    LOG_INFO("Application démarrée");
    LOG_DEBUG("Résolution : {}x{}", 1920, 1080);
    LOG_WARNING("Caméra {} non disponible", 2);
    LOG_ERROR("Impossible d'ouvrir : {}", "video.mp4");
    LOG_INFO("Traitement de {} frames à {} fps", 1000, 30);

    // Output :
    // [14:23:45] [INFO   ] Application démarrée
    // [14:23:45] [DEBUG  ] Résolution : 1920x1080
    // [14:23:45] [WARNING] Caméra 2 non disponible
    // [14:23:45] [ERROR  ] Impossible d'ouvrir : video.mp4
    // [14:23:45] [INFO   ] Traitement de 1000 frames à 30 fps
}





// Calendar et Timezone

inline void calendar_timezone_use() {

    // Unités existantes C++11
    nanoseconds  ns  = 100ns;
    microseconds us  = 100us;
    milliseconds ms  = 100ms;
    seconds      s   = 10s;
    minutes      min = 5min;
    hours        h   = 2h;


    // Nouvelles unités C++20 !
    days    d  = days{7};     // 7 jours
    weeks   w  = weeks{2};    // 2 semaines
    months  mo = months{3};   // 3 mois
    years   y  = years{1};    // 1 an


    // conversions
    std::cout << duration_cast<hours>(days{1}).count()<< "h dans un jour" << std::endl;  // 24h

    std::cout << duration_cast<days>(weeks{1}).count() << " jours dans une semaine" << std::endl;  // 7


    // Créer une date
    constexpr year_month_day date1 = 2024y/January/15;
    //

    year_month_day date2 = 2024y/1/15;  // mois en nombre


    constexpr year_month_day date3{year{2024}, month{1}, day{15}};

    // Accéder aux composants
    std::cout << static_cast<int>(date1.year())  << std::endl;  // 2024
    std::cout << static_cast<unsigned>(date1.month()) << std::endl;  // 1
    std::cout << static_cast<unsigned>(date1.day())   << std::endl;  // 15

    // verifier si la date est valide
    std::cout << date3.ok() << std::endl;

    constexpr  year_month_day invalide = 2026y/February/30;
    std::cout << invalide.ok() << std::endl;  // 0

    const auto today = year_month_day{floor<days>(system_clock::now())};

    std::cout << static_cast<int>(today.year()) << "/" << static_cast<unsigned>(today.month()) << static_cast<unsigned>(today.day());



    // calcul sur les dates
    constexpr year_month_day debut = 2025y/January/1;
    constexpr year_month_day fin =  2025y/December/31;

    // ajouter des mois/années
    constexpr year_month_day dans3mois = debut + months{3};
    std::cout << static_cast<int>(dans3mois.year()) << "/"<< static_cast<unsigned>(dans3mois.month()) << "/"<< static_cast<unsigned>(dans3mois.day())   << std::endl;

    constexpr year_month_day anProchain = debut + years{1};
    std::cout << static_cast<int>(anProchain.year()) << std::endl;



    // Différence en jours
    // convertir en sys_days pour calculer la difference

    constexpr sys_days debutJ = debut;
    constexpr  sys_days finJ = fin;

    constexpr auto diff = finJ - debutJ;
    std::cout << diff.count() << " jours" << std::endl;


    // Dernier jour du mois
    year_month_day_last dernierJanvier = 2025y/January/last;
    year_month_day dernierJour = dernierJanvier;
    std::cout << "Dernier jour janvier : " << static_cast<unsigned>(dernierJour.day())<< std::endl;  // 31

    year_month_day_last dernierFevrier = 2024y/February/last;
    year_month_day  dernierJourFev = dernierFevrier;
    std::cout << "Dernier jour février 2024 : " << static_cast<unsigned>(dernierJourFev.day())<< std::endl;

    // Jour de la semaine
    year_month_day date = 2024y/January/15;
    weekday jourSemaine{sys_days{date}};
    std::cout << "Lundi ?   " << (jourSemaine == Monday) << std::endl;  // 1

    // Prochain lundi
    sys_days prochainLundi = sys_days{date} + (Monday - jourSemaine);
    year_month_day ymdLundi{prochainLundi};

    std::cout << "Prochain lundi : " << static_cast<unsigned>(ymdLundi.day()) << std::endl;


    // Heure et dat3 + heure
    auto now = system_clock::now();
    // convertir en date locale
    auto currentDay = floor<days>(now);
    year_month_day currentDate{currentDay};


    // Extraire l'heure dans la journée
    auto hourDay = now - currentDay;
    auto heures    = floor<hours>(hourDay);
    auto minutes_   = floor<minutes>(hourDay - heures);
    auto secondes  = floor<seconds>(hourDay - heures - minutes_);

    std::cout << std::format("{:04}/{:02}/{:02} {:02}:{:02}:{:02}",
        static_cast<int>(date.year()),
        static_cast<unsigned>(date.month()),
        static_cast<unsigned>(date.day()),
        heures.count(),
        minutes_.count(),
        secondes.count())
              << std::endl;


    // hh_mm_ss → décomposer une durée
    // ─────────────────────────────────────
    seconds duree{3723};  // 1h 2min 3sec
    hh_mm_ss decompose{duree};

    std::cout << decompose.hours().count()   << "h "
              << decompose.minutes().count() << "min "
              << decompose.seconds().count() << "sec"
              << std::endl;
    // 1h 2min 3sec


    // ─────────────────────────────────────
    // Heure locale
    // ─────────────────────────────────────
    auto maintenant = system_clock::now();

    // Convertir en heure locale
    const auto* tz_locale = current_zone();
    auto heureLocale = tz_locale->to_local(maintenant);

    std::cout << "Timezone : " << tz_locale->name() << std::endl;
    // ex: Europe/Paris

    // ─────────────────────────────────────
    // Convertir entre timezones
    // ─────────────────────────────────────
    const auto* tz_paris  = locate_zone("Europe/Paris");
    const auto* tz_tokyo  = locate_zone("Asia/Tokyo");
    const auto* tz_new_york = locate_zone("America/New_York");

    // Heure actuelle à Paris
    zoned_time paris   {tz_paris,   maintenant};
    zoned_time tokyo   {tz_tokyo,   maintenant};
    zoned_time new_york{tz_new_york,maintenant};

    // Formater avec std::format
    std::cout << std::format("Paris    : {:%H:%M:%S}\n", paris.get_local_time());
    std::cout << std::format("Tokyo    : {:%H:%M:%S}\n", tokyo.get_local_time());
    std::cout << std::format("New York : {:%H:%M:%S}\n", new_york.get_local_time());
}


// Je veux...
//
// La date d'aujourd'hui ?
// → year_month_day{floor<days>(system_clock::now())}
//
// L'heure actuelle ?
// → floor<seconds>(system_clock::now())
// → puis hh_mm_ss pour décomposer
//
// Mesurer une durée d'exécution ?
// → steady_clock::now() avant et après
// → duration_cast<milliseconds>(fin - debut)
//
// Calculer une date dans N mois ?
// → year_month_day + months{N}
// → attention : vérifier .ok() après !
//
// Le dernier jour du mois ?
// → year_month_last{annee/mois} → year_month_day
//
// Savoir quel jour de la semaine ?
// → weekday{sys_days{ma_date}}
//
// Convertir date → durée ?
// → sys_days{ma_date} → c'est déjà une durée !
//
// Convertir durée → date ?
// → year_month_day{sys_days{ma_duree}}
//
// Comparer deux dates ?
// → sys_days{date1} - sys_days{date2}
// → résultat en days



// DURÉES                    POINTS DANS LE TEMPS
// ──────────────────────────────────────────────
// nanoseconds               system_clock::time_point
// microseconds              steady_clock::time_point
// milliseconds                      ↓
// seconds                       floor<days>()
// minutes                           ↓
// hours              ←→         sys_days
// days                              ↓
// weeks              ←→     year_month_day
// months                    year_month_last
// years                     year_month_weekday
//                                   ↓
//                           weekday, year, month, day
//
// CONVERSIONS :
// duration_cast<T>() → change d'unité (troncature)
// floor<T>()         → arrondit vers le bas ← le plus utilisé
// ceil<T>()          → arrondit vers le haut
// round<T>()         → arrondit au plus proche
// hh_mm_ss{}         → décompose une durée en h/min/sec
//
// HORLOGES :
// system_clock  → date réelle ← pour les logs/timestamps
// steady_clock  → chronomètre ← pour mesurer les durées




#endif //TUTO_CPP_CPP_20_H
