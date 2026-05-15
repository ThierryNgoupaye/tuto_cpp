# Guide Complet CMake — C++ Moderne

---

## Table des matières

1. [C'est quoi CMake ?](#1-cest-quoi-cmake-)
2. [Structure d'un projet](#2-structure-dun-projet)
3. [CMakeLists.txt minimal](#3-cmakeliststxt-minimal)
4. [Commandes fondamentales](#4-commandes-fondamentales)
5. [Variables importantes](#5-variables-importantes)
6. [Commandes terminal](#6-commandes-terminal)
7. [find_package](#7-find_package--trouver-des-bibliothèques)
8. [FetchContent](#8-fetchcontent--télécharger-des-dépendances)
9. [add_subdirectory](#9-add_subdirectory--sous-projets)
10. [Expressions génératrices](#10-expressions-génératrices)
11. [Options et cache](#11-options-et-cache)
12. [install()](#12-install--distribuer-sa-bibliothèque)
13. [Compilation conditionnelle cross-platform](#13-compilation-conditionnelle-cross-platform)
14. [enable_testing et ctest](#14-enable_testing-et-ctest)
15. [Bonnes pratiques équipe et Git](#15-bonnes-pratiques-équipe-et-git)
16. [Template Qt + OpenCV](#16-template-qt--opencv)

---

## 1. C'est quoi CMake ?

CMake est un **générateur de système de build**. Tu décris ton projet **une seule fois** dans un fichier `CMakeLists.txt`, et CMake génère les fichiers de compilation adaptés à chaque plateforme.

```
Sans CMake :
Linux   → g++ -std=c++23 fichier1.cpp fichier2.cpp -o app
Windows → cl.exe /std:c++23 fichier1.cpp fichier2.cpp /Fe:app.exe
macOS   → clang++ -std=c++23 fichier1.cpp fichier2.cpp -o app
→ 3 commandes différentes à maintenir ! 😱

Avec CMake :
→ un seul CMakeLists.txt → fonctionne partout ✅
```

**Le pipeline CMake :**

```
Tu écris CMakeLists.txt
        ↓
cmake ..          → CMake génère les fichiers de build
        ↓
cmake --build .   → le compilateur compile
        ↓
./monApp          → tu lances l'exécutable
```

---

## 2. Structure d'un projet

```
mon_projet/
├── CMakeLists.txt          ← versionné dans Git ✅
├── main.cpp
├── src/
│   ├── CMakeLists.txt      ← un par dossier (bonne pratique)
│   ├── Camera.cpp
│   ├── Camera.h
│   ├── Traitement.cpp
│   └── Traitement.h
├── tests/
│   ├── CMakeLists.txt
│   └── test_camera.cpp
└── build/                  ← dans .gitignore ❌ jamais versionné
    ├── Makefile
    └── monApp
```

**.gitignore typique :**

```gitignore
# Dossiers de build
build/
build-*/
cmake-build-*/
cmake-build-debug/
cmake-build-release/

# Fichiers générés par CMake
CMakeCache.txt
CMakeFiles/
cmake_install.cmake
Makefile
*.cmake
!CMakeLists.txt   # EXCEPTION : CMakeLists.txt est versionné !

# Binaires
*.o
*.a
*.so
*.dll
*.exe

# IDE
.idea/
.vscode/
```

---

## 3. CMakeLists.txt minimal

```cmake
# Version minimale de CMake requise
cmake_minimum_required(VERSION 3.16)

# Nom du projet et langage
project(MonProjet CXX)

# Standard C++ — les 3 lignes INDISPENSABLES
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)   # erreur si non supporté !
set(CMAKE_CXX_EXTENSIONS OFF)          # standard pur, pas extensions GNU

# Créer l'exécutable
add_executable(monApp main.cpp)
```

**Pourquoi ces 3 lignes sont indispensables :**

```
CMAKE_CXX_STANDARD 23
→ utiliser C++23

CMAKE_CXX_STANDARD_REQUIRED ON
→ SANS ça : si C++23 non supporté → CMake se rabat
  silencieusement sur C++17 sans te prévenir ! 😱
→ AVEC ça : erreur claire si non supporté ✅

CMAKE_CXX_EXTENSIONS OFF
→ SANS ça : GCC accepte des extensions non standard
  qui ne compileront pas sur Clang ou MSVC !
→ AVEC ça : code portable partout ✅
```

---

## 4. Commandes fondamentales

---

### `add_executable`

Crée un exécutable (programme qui se lance).

```cmake
# Un seul fichier
add_executable(monApp main.cpp)

# Plusieurs fichiers
add_executable(monApp
        main.cpp
        test_ex/src/Camera.cpp
        test_ex/src/Traitement.cpp
)

# Avec une variable
set(SOURCES main.cpp test_ex/src/Camera.cpp test_ex/src/Traitement.cpp)
add_executable(monApp ${SOURCES})
```

---

### `add_library`

Crée une bibliothèque réutilisable.

```cmake
# Bibliothèque STATIQUE → .a (Linux) / .lib (Windows)
# → code copié DANS l'exécutable final
# → exécutable autonome mais plus lourd
add_library(maLib STATIC
        test_ex/src/Camera.cpp
        test_ex/src/Traitement.cpp
)

# Bibliothèque DYNAMIQUE → .so (Linux) / .dll (Windows)
# → code séparé de l'exécutable
# → chargée au lancement du programme
# → partageable entre plusieurs programmes
add_library(maLib SHARED
        test_ex/src/Camera.cpp
        test_ex/src/Traitement.cpp
)
```

**STATIC vs SHARED :**

```
STATIC (.a / .lib) :
✅ exécutable autonome (pas de dépendance runtime)
✅ plus simple à distribuer
❌ exécutable plus lourd
❌ mise à jour = recompiler tout

SHARED (.so / .dll) :
✅ exécutable plus léger
✅ mise à jour sans recompiler l'exécutable
✅ partageable entre plusieurs programmes
❌ dépendance runtime (.so doit être présent)
❌ plus complexe à distribuer
```

---

### `target_include_directories`

Dit où chercher les fichiers headers `.h`.

```cmake
target_include_directories(monApp
        PRIVATE     # seulement pour monApp
        test_ex/src/
        include/
)
```

**Les trois mots clés :**

```
PRIVATE   → headers utilisés seulement par CETTE cible
            → 90% des cas ✅

PUBLIC    → headers utilisés par cette cible
            ET partagés avec ceux qui l'utilisent
            → si d'autres cibles incluent tes headers

INTERFACE → headers utilisés seulement par
            ceux qui utilisent cette cible
            → pour les bibliothèques header-only
```

**Exemple concret PUBLIC :**

```cmake
# maLib expose ses headers publiquement
target_include_directories(maLib PUBLIC test_ex/src/)

# monApp utilise maLib → hérite automatiquement de src/ !
target_link_libraries(monApp PRIVATE maLib)
# → monApp peut #include "Camera.h" sans configuration ✅
```

---

### `target_link_libraries`

Lie des bibliothèques à une cible.

```cmake
target_link_libraries(monApp PRIVATE
    pthread           # bibliothèque système (threads Linux)
    maLib             # notre bibliothèque custom
    ${OpenCV_LIBS}    # bibliothèque trouvée par find_package
)
```

**Ce que "lier" signifie :**

```
Compilation : .cpp → .o (fichiers objets)
Linkage     : .o + bibliothèques → exécutable final

Sans lier pthread :
→ std::thread utilise pthread en interne
→ "undefined reference to pthread_create" ❌

Avec target_link_libraries :
→ le linker sait où trouver pthread
→ l'intègre dans l'exécutable ✅
```

**Les trois mots clés PRIVATE/PUBLIC/INTERFACE :**

```
PRIVATE   → dépendance interne → pas partagée ← 90% des cas
PUBLIC    → dépendance partagée avec les utilisateurs
INTERFACE → seulement pour les utilisateurs (header-only)
```

```cmake
# Exemple avec héritage PUBLIC
add_library(maLib STATIC test_ex/src/Camera.cpp)
target_link_libraries(maLib PUBLIC ${OpenCV_LIBS})
# PUBLIC → monApp qui utilise maLib aura OpenCV automatiquement !

add_executable(monApp main.cpp)
target_link_libraries(monApp PRIVATE maLib)
# → monApp a maLib ✅ ET OpenCV (hérité) ✅
```

---

### `target_compile_options`

Ajoute des flags de compilation.

```cmake
target_compile_options(monApp PRIVATE
    -Wall              # tous les warnings
    -Wextra            # warnings supplémentaires
    -O2                # optimisation niveau 2
    -march=native      # optimise pour ce CPU spécifique
    -g                 # symboles de debug
    -fsanitize=address # détection erreurs mémoire (Debug)
)
```

---

### `target_compile_definitions`

Ajoute des `#define` sans toucher au code source.

```cmake
target_compile_definitions(monApp PRIVATE
    DEBUG                  # définit DEBUG (sans valeur)
    VERSION=42             # définit VERSION avec valeur
    APP_NAME="MonApp"      # définit APP_NAME avec string
)
```

**Dans ton code C++ :**

```c++
#ifdef DEBUG
    std::println("Mode debug activé");
#endif

std::println("Version : {}", VERSION);    // 42
std::println("Nom : {}", APP_NAME);       // MonApp
```

**Cas d'usage cross-platform :**

```cmake
if(WIN32)
    target_compile_definitions(monApp PRIVATE PLATFORM_WINDOWS)
elseif(APPLE)
    target_compile_definitions(monApp PRIVATE PLATFORM_MACOS)
elseif(UNIX)
    target_compile_definitions(monApp PRIVATE PLATFORM_LINUX)
endif()
```

```c++
// Dans le code
void ouvrirCamera() {
    #ifdef PLATFORM_WINDOWS
        ouvrirCameraDirectShow();
    #elif defined(PLATFORM_LINUX)
        ouvrirCameraV4L2();
    #elif defined(PLATFORM_MACOS)
        ouvrirCameraAVFoundation();
    #endif
}
```

---

### `include`

Inclut un autre fichier CMake. **PAS pour les bibliothèques !**

```cmake
# Activer des modules CMake intégrés
include(FetchContent)    # active FetchContent_Declare/MakeAvailable
include(GoogleTest)      # active gtest_discover_tests
include(CTest)           # active les tests CTest

# Inclure ton propre fichier CMake
include(sources.cmake)         # ta liste de sources
include(compiler_flags.cmake)  # tes flags custom
```

**Différence include vs add_subdirectory :**

```
include(fichier.cmake)
→ copie le contenu du fichier ici
→ même scope (mêmes variables)
→ pour réutiliser du code CMake

add_subdirectory(dossier/)
→ traite le sous-dossier comme un sous-projet
→ scope séparé
→ pour organiser un gros projet en modules
```

---

## 5. Variables importantes

```cmake
# Variables prédéfinies CMake
CMAKE_BUILD_TYPE          # Debug, Release, RelWithDebInfo
CMAKE_CXX_COMPILER        # chemin du compilateur
CMAKE_CXX_COMPILER_ID     # "GNU", "Clang", "MSVC"
CMAKE_CXX_COMPILER_VERSION# "14.2.0"
CMAKE_SOURCE_DIR          # dossier racine du projet
CMAKE_BINARY_DIR          # dossier de build
CMAKE_CURRENT_SOURCE_DIR  # dossier du CMakeLists.txt courant
CMAKE_INSTALL_PREFIX      # où installer (/usr/local par défaut)
PROJECT_NAME              # nom défini dans project()

# Définir ses propres variables
set(MA_VARIABLE "valeur")
set(MES_SOURCES main.cpp test_ex/src/Camera.cpp)

# Utiliser une variable
add_executable(monApp ${MES_SOURCES})
#                     ↑ ${} pour accéder

# Variable booléenne
set(ACTIVER_DEBUG ON)
if (ACTIVER_DEBUG)
    target_compile_definitions(monApp PRIVATE DEBUG)
endif ()

# Afficher la valeur d'une variable
message(STATUS "Compilateur : ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Sources : ${MES_SOURCES}")
```

---

## 6. Commandes terminal

```bash
# ─────────────────────────────────────
# Première compilation
# ─────────────────────────────────────
mkdir build && cd build
cmake ..                         # générer les fichiers de build
cmake --build .                  # compiler

# ─────────────────────────────────────
# Choisir le mode de build
# ─────────────────────────────────────
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo  # optimisé + debuggable

# ─────────────────────────────────────
# Choisir le compilateur
# ─────────────────────────────────────
cmake .. -DCMAKE_CXX_COMPILER=g++-14
cmake .. -DCMAKE_CXX_COMPILER=clang++

# ─────────────────────────────────────
# Recompiler après modification
# ─────────────────────────────────────
cmake --build .                  # recompile seulement ce qui a changé
cmake --build . --clean-first    # recompilation complète

# ─────────────────────────────────────
# Voir les commandes de compilation
# ─────────────────────────────────────
cmake --build . --verbose        # affiche g++/clang++ complet

# ─────────────────────────────────────
# Installer sur le système
# ─────────────────────────────────────
cmake --install .
cmake --install . --prefix /usr/local
sudo cmake --install .           # si /usr/local nécessite sudo

# ─────────────────────────────────────
# Voir les options disponibles
# ─────────────────────────────────────
cmake .. -L                      # liste les variables de cache
cmake-gui ..                     # interface graphique
```

---

## 7. `find_package` — trouver des bibliothèques

`find_package` cherche une bibliothèque **déjà installée** sur le système et configure automatiquement les chemins et flags.

```cmake
# Syntaxe de base
find_package(OpenCV REQUIRED)
#                   ↑
#            REQUIRED = erreur si non trouvé
#            QUIET    = aucun message si non trouvé

# Avec composants spécifiques
find_package(Qt6 REQUIRED COMPONENTS
    Core Widgets Gui Concurrent
)

# Optionnel
find_package(TBB QUIET)
if(TBB_FOUND)
    message(STATUS "TBB trouvé ✅")
    target_link_libraries(monApp PRIVATE TBB::tbb)
else()
    message(WARNING "TBB absent → parallel STL limité")
endif()
```

**Variables générées par find_package :**

```cmake
# Après find_package(OpenCV REQUIRED) :
${OpenCV_FOUND}           # TRUE si trouvé
${OpenCV_VERSION}         # "4.8.0"
${OpenCV_INCLUDE_DIRS}    # chemins vers les headers
${OpenCV_LIBS}            # bibliothèques à lier

# Utilisation
target_include_directories(monApp PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(monApp PRIVATE ${OpenCV_LIBS})
```

**Comment CMake trouve la bibliothèque ?**

```
find_package(OpenCV) cherche dans cet ordre :
1. OpenCVConfig.cmake dans les chemins standard
   (/usr/lib/cmake/opencv4/OpenCVConfig.cmake)
2. FindOpenCV.cmake dans les modules CMake
3. CMAKE_PREFIX_PATH si défini

→ c'est pourquoi installer une lib avec
  cmake --install génère un fichier *Config.cmake ✅
```

---

## 8. `FetchContent` — télécharger des dépendances

`FetchContent` télécharge et intègre une bibliothèque **directement depuis internet** au moment du `cmake ..`.

```cmake
include(FetchContent)  # activer le module

# Déclarer la dépendance
FetchContent_Declare(
    googletest                                              # nom local
    GIT_REPOSITORY https://github.com/google/googletest.git # source
    GIT_TAG        v1.14.0                                  # version fixée !
)

# Rendre disponible (télécharge si nécessaire)
FetchContent_MakeAvailable(googletest)

# Utiliser normalement
target_link_libraries(tests PRIVATE GTest::gtest_main)
```

**Autres sources supportées :**

```cmake
# Depuis une URL directe
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp
)

# Depuis un dossier local
FetchContent_Declare(
    malib
    SOURCE_DIR /home/thierry/projets/malib
)
```

**Avantages :**

```
✅ version fixée → reproductible sur toutes les machines
✅ pas d'installation manuelle
✅ tout le monde a la même version
✅ idéal pour Google Test, nlohmann/json, spdlog...
```

---

## 9. `add_subdirectory` — sous-projets

Organiser un gros projet en modules indépendants.

```cmake
# CMakeLists.txt racine
cmake_minimum_required(VERSION 3.16)
project(MonProjet CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(test_ex/src)    # compile src/
add_subdirectory(tests)  # compile tests/

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE traitement_lib)
```

```cmake
# src/CMakeLists.txt
add_library(traitement_lib STATIC
    Camera.cpp
    Traitement.cpp
    Logger.cpp
)

target_include_directories(traitement_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}  # expose les headers de ce dossier
)

target_link_libraries(traitement_lib PUBLIC
    ${OpenCV_LIBS}
)
```

```cmake
# tests/CMakeLists.txt
add_executable(tests_unitaires
    test_camera.cpp
    test_traitement.cpp
)

target_link_libraries(tests_unitaires PRIVATE
    traitement_lib
    GTest::gtest_main
)
```

**Bénéfice en équipe :**

```
Développeur A → modifie src/CMakeLists.txt
Développeur B → modifie tests/CMakeLists.txt
→ fichiers différents → zéro conflit Git ! ✅
```

---

## 10. Expressions génératrices

Conditions évaluées **au moment de la génération** du système de build.

**Syntaxe :** `$<condition:valeur>`

```cmake
# Flags selon le mode de build
target_compile_options(monApp PRIVATE
    $<$<CONFIG:Debug>:-g -O0 -fsanitize=address,undefined>
    $<$<CONFIG:Release>:-O2 -march=native -DNDEBUG>
    -Wall -Wextra  # toujours
)

# Flags selon le compilateur
target_compile_options(monApp PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-Wno-unused-parameter>
    $<$<CXX_COMPILER_ID:Clang>:-Wno-unused-variable>
)

# Lier seulement si une lib est trouvée
target_link_libraries(monApp PRIVATE
    $<$<BOOL:${TBB_FOUND}>:TBB::tbb>
    # ↑ lie TBB seulement si TBB_FOUND = true
)
```

**Lire une expression génératrice :**

```
$<$<CONFIG:Debug>:-g -O0>
 ↑  ↑              ↑
 |  condition       valeur si vraie
 expression génératrice

Lecture : "si le mode de build est Debug
           → ajoute les flags -g -O0"
```

---

## 11. Options et cache

Permettent à l'utilisateur de **configurer le build** sans modifier CMakeLists.txt.

```cmake
# Déclarer des options
option(ACTIVER_TESTS     "Compiler les tests"     ON)
option(ACTIVER_OPENCL    "Utiliser OpenCL"        OFF)
option(ACTIVER_PROFILING "Activer le profiling"   OFF)

# Utiliser les options
if(ACTIVER_TESTS)
    enable_testing()
    add_subdirectory(tests)
    message(STATUS "Tests activés ✅")
endif()

if(ACTIVER_PROFILING)
    target_compile_options(monApp PRIVATE -pg)
    target_link_options(monApp PRIVATE -pg)
    message(STATUS "Profiling activé (gprof)")
endif()
```

**Dans le terminal :**

```bash
cmake .. -DACTIVER_TESTS=OFF
cmake .. -DACTIVER_OPENCL=ON
cmake .. -DACTIVER_PROFILING=ON -DCMAKE_BUILD_TYPE=Release
```

---

## 12. `install()` — distribuer sa bibliothèque

`install()` dit à CMake **quoi copier et où** quand on fait `cmake --install`.

---

### Solution 1 — installation simple

```cmake
# Installer la bibliothèque
install(TARGETS traitement_lib
        ARCHIVE DESTINATION lib   # .a → {prefix}/lib/
        LIBRARY DESTINATION lib   # .so → {prefix}/lib/
        RUNTIME DESTINATION bin   # .exe/.dll → {prefix}/bin/
)

# Installer les headers
install(FILES
        test_ex/headers/Camera.h
        test_ex/src/Traitement.h
        DESTINATION include/traitement  # → {prefix}/include/traitement/
)
```

**Décryptage des arguments :**

```
TARGETS traitement_lib
→ la cible à installer (créée par add_library)

ARCHIVE DESTINATION lib
→ ARCHIVE = bibliothèque statique (.a / .lib)
→ DESTINATION lib = copier dans {prefix}/lib/

LIBRARY DESTINATION lib
→ LIBRARY = bibliothèque dynamique (.so / .dylib)
→ DESTINATION lib = copier dans {prefix}/lib/

RUNTIME DESTINATION bin
→ RUNTIME = exécutables + .dll Windows
→ DESTINATION bin = copier dans {prefix}/bin/

FILES src/Camera.h src/Traitement.h
→ ces fichiers spécifiquement

DESTINATION include/traitement
→ copiés dans {prefix}/include/traitement/
```

**Pourquoi installer les headers séparément ?**

```
La bibliothèque .a/.so contient le code compilé
mais PAS les headers !

Les headers sont nécessaires pour que l'autre projet
sache COMMENT utiliser la lib :
→ types, méthodes, signatures...

Sans headers → #include "Camera.h" → introuvable ❌
Avec headers installés → tout fonctionne ✅
```

**Dans le terminal :**

```bash
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local
cmake --build .
sudo cmake --install .

# Résultat :
# /usr/local/lib/libtraitement_lib.a
# /usr/local/include/traitement/Camera.h
# /usr/local/include/traitement/Traitement.h
```

**Dans l'autre projet :**

```cmake
find_library(TRAITEMENT_LIB
    NAMES traitement_lib
    HINTS /usr/local/lib
)

target_include_directories(autreApp PRIVATE
    /usr/local/include/traitement
)

target_link_libraries(autreApp PRIVATE ${TRAITEMENT_LIB})
```

---

### Solution 2 — Package CMake (façon moderne ✅)

Permet d'utiliser `find_package(Traitement REQUIRED)` exactement comme OpenCV ou Qt !

```cmake
include(CMakePackageConfigHelpers)

# Étape 1 — installer la lib et l'associer à un export
install(TARGETS traitement_lib
        EXPORT TraitementTargets      # ← enregistrer dans ce groupe
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
)

# Étape 2 — installer les headers
install(FILES
        test_ex/headers/Camera.h
        test_ex/src/Traitement.h
        DESTINATION include/traitement
)

# Étape 3 — générer et installer TraitementConfig.cmake
install(EXPORT TraitementTargets
        FILE TraitementConfig.cmake   # nom cherché par find_package
        NAMESPACE Traitement::             # préfixe des cibles
        DESTINATION lib/cmake/Traitement   # où CMake cherche automatiquement
)
```

**Décryptage install(EXPORT ...) :**

```
EXPORT TraitementTargets
→ utilise le groupe défini dans install(TARGETS ... EXPORT ...)
→ contient toutes les infos sur notre lib

FILE TraitementConfig.cmake
→ nom du fichier de config généré
→ find_package(Traitement) cherche TraitementConfig.cmake ✅

NAMESPACE Traitement::
→ préfixe ajouté aux cibles
→ traitement_lib devient Traitement::traitement_lib
→ comme Qt6::Core, OpenCV::opencv_core...
→ évite les conflits de noms ✅

DESTINATION lib/cmake/Traitement
→ CMake cherche automatiquement dans lib/cmake/
→ {prefix}/lib/cmake/Traitement/TraitementConfig.cmake ✅
```

**Ce qui est installé sur le système :**

```
/usr/local/
├── lib/
│   ├── libtraitement_lib.a
│   └── cmake/
│       └── Traitement/
│           └── TraitementConfig.cmake  ← fichier magique !
└── include/
    └── traitement/
        ├── Camera.h
        └── Traitement.h
```

**Dans l'autre projet — ultra simple :**

```cmake
# Exactement comme find_package(OpenCV REQUIRED) !
find_package(Traitement REQUIRED)

add_executable(autreApp main.cpp)

target_link_libraries(autreApp PRIVATE
    Traitement::traitement_lib
)
# → headers inclus automatiquement ✅
# → lib liée automatiquement ✅
```

---

### Solution 3 — FetchContent (la plus moderne)

```cmake
include(FetchContent)

FetchContent_Declare(
    traitement
    GIT_REPOSITORY https://github.com/toi/traitement_lib.git
    GIT_TAG        v1.0.0
)

FetchContent_MakeAvailable(traitement)

target_link_libraries(autreApp PRIVATE traitement_lib)
```

---

### Quand utiliser quelle solution ?

```
Projet perso rapide
→ chemin direct (pas d'install)

Projet partagé en équipe
→ FetchContent ✅ (même version pour tous)

Bibliothèque à distribuer publiquement
→ Package CMake (Solution 2) ✅
→ find_package() comme OpenCV, Qt...

Bibliothèque système locale
→ Solution 1 (cmake --install simple) ✅
```

---

## 13. Compilation conditionnelle cross-platform

```cmake
# Détecter la plateforme
if(WIN32)
    message(STATUS "Windows détecté")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        PLATFORM_WINDOWS
        WIN32_LEAN_AND_MEAN   # réduit les includes Windows
        NOMINMAX              # évite les macros min/max Windows
    )
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)

elseif(APPLE)
    message(STATUS "macOS détecté")
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_MACOS)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        "-framework CoreFoundation"
        "-framework AVFoundation"
    )

elseif(UNIX)
    message(STATUS "Linux détecté")
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_LINUX)
    target_link_libraries(${PROJECT_NAME} PRIVATE pthread)
endif()
```

**Dans le code C++ :**

```cpp
void ouvrirCamera() {
#ifdef PLATFORM_WINDOWS
    ouvrirCameraDirectShow();
#elif defined(PLATFORM_MACOS)
    ouvrirCameraAVFoundation();
#elif defined(PLATFORM_LINUX)
    ouvrirCameraV4L2();
#else
    #error "Plateforme non supportée !"
#endif
}
```

---

## 14. `enable_testing` et ctest

```cmake
# CMakeLists.txt racine
enable_testing()           # activer le système de tests CMake
add_subdirectory(tests)
```

```cmake
# tests/CMakeLists.txt
add_executable(mes_tests
    test_camera.cpp
    test_traitement.cpp
)

target_link_libraries(mes_tests PRIVATE
    traitement_lib
    GTest::gtest_main
)

# Enregistrer comme test ctest
add_test(
    NAME    tests_unitaires
    COMMAND mes_tests
)
```

**Dans le terminal :**

```bash
cmake --build .

ctest                    # lancer tous les tests
ctest --verbose          # avec détails
ctest -R tests_unitaires # un test spécifique
ctest -j4                # en parallèle sur 4 cœurs

# Output :
# Test project /build
#     Start 1: tests_unitaires
# 1/1 Test #1: tests_unitaires .... Passed  0.05 sec
# 100% tests passed, 0 tests failed out of 1
```

---

## 15. Bonnes pratiques équipe et Git

### Gestion des fichiers sources en équipe

```
Problème :
Développeur A et B modifient CMakeLists.txt
→ conflit Git !

Solutions :

1. Un CMakeLists.txt par dossier
   → chaque développeur touche son dossier
   → zéro conflit ✅

2. Convention de commit
   → fichier source + CMakeLists.txt = même commit
   → jamais l'un sans l'autre

3. Si conflit quand même → toujours simple à résoudre !
   → garder les DEUX ajouts
   → CMakeLists.txt = liste de fichiers
   → fusionner = additionner les listes
```

**Résolution de conflit type :**

```
<<<<<<< HEAD
add_executable(monApp
    main.cpp
    src/Camera.cpp    ← ton ajout
=======
add_executable(monApp
    main.cpp
    src/Traitement.cpp ← ajout collègue
>>>>>>> feature/traitement

→ Résolution : garder les deux !
add_executable(monApp
    main.cpp
    src/Camera.cpp      ← ton ajout ✅
    src/Traitement.cpp  ← ajout collègue ✅
)
```

### Messages de diagnostic

```cmake
message(STATUS "Information normale")
message(WARNING "Attention !")
message(SEND_ERROR "Erreur → continue quand même")
message(FATAL_ERROR "Erreur → arrête cmake !")

# Bonnes pratiques
message(STATUS "Compilateur : ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Mode        : ${CMAKE_BUILD_TYPE}")
message(STATUS "OpenCV      : ${OpenCV_VERSION}")
```

---

## 16. Template Qt + OpenCV

CMakeLists.txt complet prêt à l'emploi pour un projet Qt + OpenCV cross-platform.

```cmake
cmake_minimum_required(VERSION 3.16)
project(TraitementImage CXX)

# ─────────────────────────────────────
# Configuration C++
# ─────────────────────────────────────
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Qt → génération automatique MOC/UIC/RCC
set(CMAKE_AUTOMOC ON)   # Meta-Object Compiler (signaux/slots)
set(CMAKE_AUTOUIC ON)   # UI Compiler (.ui → .h)
set(CMAKE_AUTORCC ON)   # Resource Compiler (.qrc)

# ─────────────────────────────────────
# Options configurables
# ─────────────────────────────────────
option(ACTIVER_TESTS "Compiler les tests" ON)
option(ACTIVER_DEBUG_LOG "Activer les logs debug" OFF)

# ─────────────────────────────────────
# Dépendances
# ─────────────────────────────────────
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui Concurrent)
find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)

find_package(TBB QUIET)
if (TBB_FOUND)
    message(STATUS "TBB trouvé → parallel STL complet ✅")
endif ()

# ─────────────────────────────────────
# Sources
# ─────────────────────────────────────
set(SOURCES
        main.cpp
        test_ex/src/MainWindow.cpp
        test_ex/src/Camera.cpp
        test_ex/src/PipelineTraitement.cpp
        test_ex/src/Logger.cpp
)

set(HEADERS
        test_ex/src/MainWindow.h
        test_ex/headers/Camera.h
        test_ex/src/PipelineTraitement.h
        test_ex/src/Logger.h
)

# ─────────────────────────────────────
# Exécutable
# ─────────────────────────────────────
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# ─────────────────────────────────────
# Includes
# ─────────────────────────────────────
target_include_directories(${PROJECT_NAME} PRIVATE
        test_ex/src/
        ${OpenCV_INCLUDE_DIRS}
)

# ─────────────────────────────────────
# Bibliothèques
# ─────────────────────────────────────
target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Gui
        Qt6::Concurrent
        ${OpenCV_LIBS}
        Threads::Threads
        $<$<BOOL:${TBB_FOUND}>:TBB::tbb>
)

# ─────────────────────────────────────
# Flags de compilation
# ─────────────────────────────────────
target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall -Wextra
        $<$<CONFIG:Debug>:-g -O0 -fsanitize=address,undefined>
        $<$<CONFIG:Release>:-O2 -march=native>
)

target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:-fsanitize=address,undefined>
)

# ─────────────────────────────────────
# Cross-platform
# ─────────────────────────────────────
if (WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
            PLATFORM_WINDOWS WIN32_LEAN_AND_MEAN NOMINMAX)
elseif (APPLE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_MACOS)
elseif (UNIX)
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_LINUX)
endif ()

# ─────────────────────────────────────
# Options
# ─────────────────────────────────────
if (ACTIVER_DEBUG_LOG)
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG_LOG)
endif ()

# ─────────────────────────────────────
# Tests
# ─────────────────────────────────────
if (ACTIVER_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif ()

# ─────────────────────────────────────
# Informations de build
# ─────────────────────────────────────
message(STATUS "─────────────────────────────────────")
message(STATUS "Projet      : ${PROJECT_NAME}")
message(STATUS "Compilateur : ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Mode        : ${CMAKE_BUILD_TYPE}")
message(STATUS "C++         : ${CMAKE_CXX_STANDARD}")
message(STATUS "OpenCV      : ${OpenCV_VERSION}")
message(STATUS "Qt          : ${Qt6_VERSION}")
message(STATUS "─────────────────────────────────────")
```

---

## Résumé des commandes essentielles

| Commande | Rôle |
|----------|------|
| `cmake_minimum_required(VERSION x)` | Version minimale de CMake |
| `project(nom CXX)` | Nom du projet |
| `set(CMAKE_CXX_STANDARD 23)` | Version C++ |
| `set(CMAKE_CXX_STANDARD_REQUIRED ON)` | Obligatoire ! |
| `set(CMAKE_CXX_EXTENSIONS OFF)` | Standard pur |
| `add_executable(nom fichiers)` | Créer un exécutable |
| `add_library(nom STATIC/SHARED fichiers)` | Créer une bibliothèque |
| `target_include_directories(cible PRIVATE dirs)` | Headers |
| `target_link_libraries(cible PRIVATE libs)` | Lier des bibliothèques |
| `target_compile_options(cible PRIVATE flags)` | Flags de compilation |
| `target_compile_definitions(cible PRIVATE DEFS)` | `#define` |
| `find_package(lib REQUIRED)` | Trouver une lib système |
| `FetchContent_Declare/MakeAvailable` | Télécharger une lib |
| `add_subdirectory(dossier)` | Inclure un sous-projet |
| `include(module)` | Inclure un fichier CMake |
| `install(TARGETS ...)` | Installer la lib |
| `install(FILES ...)` | Installer les headers |
| `install(EXPORT ...)` | Générer la config find_package |
| `enable_testing()` | Activer ctest |
| `add_test(NAME ... COMMAND ...)` | Enregistrer un test |
| `option(NOM "description" ON/OFF)` | Option configurable |
| `message(STATUS "...")` | Afficher un message |