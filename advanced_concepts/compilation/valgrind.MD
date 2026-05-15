# Guide Valgrind — Détection d'erreurs mémoire en C++

---

## Table des matières

1. [C'est quoi Valgrind ?](#1-cest-quoi-valgrind-)
2. [Installation](#2-installation)
3. [Pourquoi analyser la mémoire ?](#3-pourquoi-analyser-la-mémoire-)
4. [Utilisation de base](#4-utilisation-de-base)
5. [Les erreurs détectées](#5-les-erreurs-détectées)
6. [Lire un rapport Valgrind](#6-lire-un-rapport-valgrind)
7. [Valgrind dans CMakeLists.txt](#7-valgrind-dans-cmakeliststxt)
8. [Valgrind avec Google Test](#8-valgrind-avec-google-test)
9. [Alternatives à Valgrind](#9-alternatives-à-valgrind)
10. [Bonnes pratiques](#10-bonnes-pratiques)
11. [Résumé des commandes](#11-résumé-des-commandes)

---

## 1. C'est quoi Valgrind ?

Valgrind est un outil qui **surveille ton programme pendant son exécution** et détecte les problèmes liés à la gestion de la mémoire.

### Analogie du comptable

```
Valgrind = un comptable très strict
           qui surveille ton "compte bancaire mémoire"

new int(42)  → dépôt de 4 octets dans le compte
delete ptr   → retrait de 4 octets du compte

À la fin du programme :
→ solde = 0       → parfait, tout est équilibré ✅
→ solde > 0       → fuite mémoire ! tu as oublié de rembourser ❌
→ retrait sans dépôt → accès invalide ! tu touches de l'argent
                        qui n'est pas à toi ❌
```

### Ce que Valgrind surveille

Valgrind intercepte et analyse :

- Chaque **allocation** mémoire (`new`, `malloc`)
- Chaque **libération** mémoire (`delete`, `free`)
- Chaque **accès** en lecture et en écriture

Et il signale ensuite :

| Problème                | Description                                          |
|-------------------------|------------------------------------------------------|
| Fuite mémoire           | Mémoire allouée mais jamais libérée                  |
| Accès invalide          | Lire/écrire en dehors des limites allouées           |
| Mémoire non initialisée | Utiliser une variable avant de lui donner une valeur |
| Double free             | Libérer deux fois le même pointeur                   |

---

## 2. Installation

```bash
# Installer Valgrind sur Ubuntu/Debian
sudo apt install valgrind

# Vérifier que l'installation a réussi
valgrind --version
# → valgrind-3.22.0  (ou version similaire)
```

---

## 3. Pourquoi analyser la mémoire ?

En C++, contrairement à Java ou Python, **il n'y a pas de garbage collector**. C'est le développeur qui est responsable de libérer la mémoire qu'il alloue.

```
Java / Python :
→ tu crées un objet → le runtime le libère automatiquement
→ pas de fuite possible (ou très rare)

C++ :
→ tu crées un objet avec new → tu DOIS faire delete
→ si tu oublies → la mémoire reste occupée pour toujours
→ sur un long programme → la RAM se remplit progressivement
→ le programme ralentit puis plante (Out of Memory)
```

### Exemple concret de fuite progressive

```c++
// Programme qui fuit 1 Mo par seconde
while (true) {
    char* buffer = new char[1024 * 1024];  // alloue 1 Mo
    utiliserBuffer(buffer);
    // oubli de delete[] buffer
    std::this_thread::sleep_for(std::chrono::seconds(1));
}
// Après 1h → 3.6 Go de RAM consommés → crash !
```

### La bonne pratique : les smart pointers

```c++
// Avec unique_ptr → libération automatique, zéro fuite
while (true) {
    auto buffer = std::make_unique<char[]>(1024 * 1024);
    utiliserBuffer(buffer.get());
}   // buffer libéré automatiquement ici ✅
```

> **Règle d'or** : préférer `std::unique_ptr` et `std::shared_ptr` aux pointeurs bruts. Mais Valgrind reste utile pour détecter les problèmes dans le code existant ou les bibliothèques tierces.

---

## 4. Utilisation de base

### Prérequis : compiler avec les symboles de debug

Pour que Valgrind affiche les **noms de fichiers et numéros de lignes** dans son rapport, il faut absolument compiler avec le flag `-g` et sans optimisation (`-O0`).

```bash
# Compilation manuelle
g++-14 -std=c++23 -g -O0 main.cpp -o monApp
#                  ↑   ↑
#            symboles  pas d'optimisation
#            de debug  (pour garder la structure du code)
```

```cmake
# Dans CMakeLists.txt — automatique en mode Debug
target_compile_options(monApp PRIVATE
    $<$<CONFIG:Debug>:-g -O0>
)
```

```bash
# Configurer CMake en mode Debug
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=g++-14
cmake --build .
```

### Lancer Valgrind

```bash
# Version simple — juste lancer sous Valgrind
valgrind ./monApp

# Version complète avec rapport détaillé des fuites
valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         ./monApp
```

### Explication des options

| Option                  | Rôle                                                                |
|-------------------------|---------------------------------------------------------------------|
| `--leak-check=full`     | Analyse complète des fuites mémoire                                 |
| `--show-leak-kinds=all` | Affiche tous les types de fuites (definitely, possibly, indirectly) |
| `--track-origins=yes`   | Indique d'où vient la mémoire non initialisée                       |
| `--verbose`             | Affichage détaillé                                                  |
| `--error-exitcode=1`    | Retourne un code d'erreur si problème trouvé (utile en CI/CD)       |

---

## 5. Les erreurs détectées

### Erreur 1 — Fuite mémoire (Memory Leak)

C'est l'erreur la plus courante. De la mémoire est allouée mais n'est jamais libérée.

```c++
// ❌ Code avec fuite mémoire
#include <iostream>

void fonctionAvecFuite() {
    int* ptr = new int(42);          // allocation ligne 4
    std::cout << *ptr << std::endl;
    //  oubli de : delete ptr;
}   // ptr est perdu ici → 4 octets perdus pour toujours !

int main() {
    fonctionAvecFuite();
    return 0;
}
```

```
Rapport Valgrind :

==12345== LEAK SUMMARY:
==12345==    definitely lost: 4 bytes in 1 blocks
                              ↑ fuite certaine !

==12345== 4 bytes in 1 block are definitely lost
==12345==    at 0x...: operator new(unsigned long)
==12345==    by 0x...: fonctionAvecFuite() (main.cpp:4)
                                            ↑ ligne exacte !
==12345==    by 0x...: main (main.cpp:9)
```

```c++
//  Code corrigé
void fonctionSansFuite() {
    int* ptr = new int(42);
    std::cout << *ptr << std::endl;
    delete ptr;   // libération 
}

// Encore mieux — avec unique_ptr
void fonctionOptimale() {
    auto ptr = std::make_unique<int>(42);
    std::cout << *ptr << std::endl;
}   // libéré automatiquement 
```

---

### Erreur 2 — Accès invalide (Invalid Read/Write)

Lire ou écrire en dehors de la zone mémoire allouée.

```c++
//  Code avec accès invalide
int main() {
    int* tab = new int[5];  // indices valides : 0, 1, 2, 3, 4

    tab[5]  = 42;   //  hors limites ! index 5 n'existe pas
    tab[-1] = 10;   //  index négatif !

    int valeur = tab[5];  //  lecture hors limites

    delete[] tab;
    return 0;
}
```

```
Rapport Valgrind :

==12345== Invalid write of size 4
==12345==    at 0x...: main (main.cpp:4)
==12345==  Address 0x... is 0 bytes after a block of size 20
           ↑ écriture juste APRÈS le tableau (5 × 4 = 20 octets)

==12345== Invalid read of size 4
==12345==    at 0x...: main (main.cpp:7)
```

> **Danger** : ces erreurs peuvent provoquer des comportements imprévisibles. Le programme peut "fonctionner" par chance ou crasher aléatoirement selon les données en mémoire.

---

### Erreur 3 — Mémoire non initialisée

Utiliser une variable sans lui avoir donné de valeur.

```c++
//  Code avec variable non initialisée
int main() {
    int x;              // déclarée mais pas initialisée !
    if (x > 0) {        //  x contient une valeur aléatoire
        std::cout << "positif" << std::endl;
    }

    int* tab = new int[10];  //  contenu indéfini !
    std::cout << tab[0];     // lecture de valeur aléatoire
    delete[] tab;

    return 0;
}
```

```
Rapport Valgrind :

==12345== Conditional jump or move depends on uninitialised value(s)
==12345==    at 0x...: main (main.cpp:4)
           ↑ la condition if() utilise une valeur non initialisée !

==12345== Use of uninitialised value of size 8
==12345==    at 0x...: main (main.cpp:9)
```

```c++
//  Toujours initialiser ses variables
int x = 0;
int* tab = new int[10]();   // () → initialise à 0
// ou mieux :
auto tab = std::make_unique<int[]>(10);  // initialisé à 0 
```

---

### Erreur 4 — Double Free

Libérer deux fois le même pointeur.

```c++
//  Code avec double free
int main() {
    int* ptr = new int(42);
    delete ptr;   // première libération 
    delete ptr;   //  deuxième libération → comportement indéfini !
    return 0;
}
```

```
Rapport Valgrind :

==12345== Invalid free() / delete / delete[] / realloc()
==12345==    at 0x...: operator delete(void*)
==12345==    by 0x...: main (main.cpp:4)
==12345==  Address 0x... is 0 bytes inside a block of size 4
           free'd at main.cpp:3
           ↑ déjà libéré à la ligne 3 !
```

```c++
//  Après un delete, mettre le pointeur à nullptr
int* ptr = new int(42);
delete ptr;
ptr = nullptr;   // sécurité → un delete nullptr est inoffensif 
delete ptr;      // ne fait rien 
```

---

### Erreur 5 — Utilisation après libération (Use After Free)

Accéder à de la mémoire qui a déjà été libérée.

```c++
//  Code avec use-after-free
int main() {
    int* ptr = new int(42);
    delete ptr;            // libération
    std::cout << *ptr;     //  accès après libération !
                           // ptr pointe vers de la mémoire
                           // qui peut maintenant appartenir
                           // à quelqu'un d'autre !
    return 0;
}
```

```
Rapport Valgrind :

==12345== Invalid read of size 4
==12345==    at 0x...: main (main.cpp:4)
==12345==  Address 0x... is 0 bytes inside a block of size 4
           free'd at main.cpp:3
```

---

## 6. Lire un rapport Valgrind

### Structure d'un rapport complet

```
==12345== Memcheck, a memory error detector
          ↑ PID du processus — ignore ce nombre, il change à chaque exécution

[... sortie normale de ton programme ...]

==12345== HEAP SUMMARY:
==12345==     in use at exit: 6,220,808 bytes in 2 blocks
              ↑ mémoire encore allouée à la fin du programme
==12345==   total heap usage: 4 allocs, 2 frees, 6,221,320 bytes allocated
              ↑ 4 allocations, 2 libérations → 2 fuites !

==12345== 8 bytes in 1 blocks are definitely lost
==12345==    at 0x...: operator new(unsigned long)
==12345==    by 0x...: testerCamera() (main.cpp:10)
              ↑ fuite à la ligne 10 → new Camera()
==12345==    by 0x...: main (main.cpp:30)

==12345== 6,220,800 bytes in 1 blocks are definitely lost
==12345==    at 0x...: operator new(unsigned long)
==12345==    by 0x...: testerCamera() (main.cpp:14)
              ↑ fuite à la ligne 14 → new Frame() avec 6 Mo de pixels !

==12345== LEAK SUMMARY:
==12345==    definitely lost: 6,220,808 bytes in 2 blocks
              ↑ fuites certaines à corriger en priorité !
==12345==    indirectly lost: 0 bytes in 0 blocks
==12345==      possibly lost: 0 bytes in 0 blocks
==12345==    still reachable: 0 bytes in 0 blocks
==12345==         suppressed: 0 bytes in 0 blocks

==12345== ERROR SUMMARY: 2 errors from 2 contexts
          ↑ résumé final : 2 problèmes à corriger
          → objectif : ERROR SUMMARY: 0 errors ✅
```

### Types de fuites — du plus grave au moins grave

| Type              | Signification                                 | Priorité                             |
|-------------------|-----------------------------------------------|--------------------------------------|
| `definitely lost` | Mémoire allouée, jamais libérée, inaccessible | 🔴 Corriger immédiatement            |
| `indirectly lost` | Perdu à cause d'un autre objet perdu          | 🟠 Corriger avec la fuite principale |
| `possibly lost`   | Pointeur déplacé, fuite possible              | 🟡 Examiner                          |
| `still reachable` | Accessible à la fin (variables statiques)     | 🟢 Souvent acceptable                |

### Objectif à atteindre

```
==12345== HEAP SUMMARY:
==12345==     in use at exit: 0 bytes in 0 blocks  ← parfait !
==12345==   total heap usage: X allocs, X frees    ← allocs = frees ✅

==12345== All heap blocks were freed -- no leaks are possible ✅

==12345== ERROR SUMMARY: 0 errors from 0 contexts ✅
```

---

## 7. Valgrind dans CMakeLists.txt

Ajouter une cible `valgrind` pour l'intégrer dans le workflow de build.

```cmake
# Dans CMakeLists.txt

# Chercher Valgrind sur le système
find_program(VALGRIND valgrind)

if(VALGRIND)
    message(STATUS "Valgrind trouvé : ${VALGRIND} ")

    # Cible pour analyser l'application principale
    add_custom_target(valgrind
        COMMAND ${VALGRIND}
            --leak-check=full
            --show-leak-kinds=all
            --track-origins=yes
            --error-exitcode=1
            $<TARGET_FILE:monApp>
        DEPENDS monApp
        COMMENT "Analyse Valgrind de monApp en cours..."
    )

    # Cible pour analyser les tests
    add_custom_target(valgrind_tests
        COMMAND ${VALGRIND}
            --leak-check=full
            --show-leak-kinds=all
            --track-origins=yes
            --error-exitcode=1
            $<TARGET_FILE:mes_tests>
        DEPENDS mes_tests
        COMMENT "Analyse Valgrind des tests en cours..."
    )

else()
    message(WARNING "Valgrind non trouvé → sudo apt install valgrind")
endif()
```

**Utilisation :**

```bash
# Compiler en debug (indispensable !)
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=g++-14
cmake --build .

# Lancer Valgrind via CMake
cmake --build . --target valgrind         # analyse l'application
cmake --build . --target valgrind_tests   # analyse les tests
```

---

## 8. Valgrind avec Google Test

Valgrind fonctionne parfaitement avec Google Test. Il analyse non seulement le code des tests, mais aussi le **code qu'ils testent**.

```bash
# Lancer Valgrind sur l'exécutable de tests
valgrind --leak-check=full \
         --error-exitcode=1 \
         ./tests/mes_tests

# Filtrer les tests ET analyser la mémoire
valgrind --leak-check=full \
         ./tests/mes_tests \
         --gtest_filter="Camera*"
```

### Exemple — test avec et sans fuite

```c++
// tests/test_camera.cpp
#include <gtest/gtest.h>
#include "Camera.h"

//  Ce test va être détecté par Valgrind
TEST(CameraTest, AvecFuite) {
    Camera* cam = new Camera();   // fuite !
    cam->initialiser();
    EXPECT_TRUE(cam->estOuverte());
    // oubli de : delete cam;
}

//  Ce test est propre
TEST(CameraTest, SansFuite) {
    auto cam = std::make_unique<Camera>();
    cam->initialiser();
    EXPECT_TRUE(cam->estOuverte());
}   // cam libéré automatiquement 
```

```
Valgrind détectera la fuite de AvecFuite
et signalera que SansFuite est propre.

→ utile pour vérifier que ton code de test
  lui-même ne fuit pas !
```

---

## 9. Alternatives à Valgrind

### AddressSanitizer (ASan) — pour le développement quotidien

```
Valgrind :
✅ très complet, pas besoin de recompiler
❌ programme 10x à 50x plus lent !
❌ pas disponible sur Windows

AddressSanitizer :
✅ beaucoup plus rapide (seulement 2x plus lent)
✅ intégré directement au compilateur GCC/Clang
✅ fonctionne sur Linux, macOS, Windows partiel
❌ nécessite de recompiler avec des flags spéciaux
```

**Activer ASan dans CMakeLists.txt :**

```cmake
# Déjà configuré si tu as suivi le guide CMake !
target_compile_options(monApp PRIVATE
    $<$<CONFIG:Debug>:-fsanitize=address,undefined>
)

target_link_options(monApp PRIVATE
    $<$<CONFIG:Debug>:-fsanitize=address,undefined>
)
```

```bash
# Compiler et lancer — ASan s'active automatiquement
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .
./monApp   # ASan intercepte les erreurs à l'exécution
```

### Tableau comparatif

| Outil      | Vitesse          | Installation           | Précision   | Plateforme          |
|------------|------------------|------------------------|-------------|---------------------|
| Valgrind   | 10-50x plus lent | `apt install valgrind` | Très élevée | Linux/macOS         |
| ASan       | 2x plus lent     | Intégré GCC/Clang      | Élevée      | Linux/macOS/Windows |
| Dr. Memory | 10x plus lent    | Séparé                 | Élevée      | Windows/Linux       |

### Quelle stratégie adopter ?

```
Développement quotidien :
→ ASan activé en mode Debug (-fsanitize=address)
→ rapide, feedback immédiat

Analyse approfondie (avant release) :
→ Valgrind avec --leak-check=full
→ rapport complet, aucun problème ignoré

CI/CD (intégration continue) :
→ les deux ! ASan pour la rapidité, Valgrind pour la complétude
→ --error-exitcode=1 → le build échoue si erreur mémoire
```

---

## 10. Bonnes pratiques

### Règle 1 — Toujours utiliser les smart pointers

```c++
//  Pointeur brut → risque de fuite
void mauvaise() {
    Camera* cam = new Camera();
    cam->initialiser();
    // si exception ici → fuite !
    delete cam;
}

// ✅ unique_ptr → libération automatique
void bonne() {
    auto cam = std::make_unique<Camera>();
    cam->initialiser();
}   // libéré même si exception 
```

### Règle 2 — Initialiser toutes les variables

```c++
//  Valeurs indéfinies
int compteur;
float ratio;
char buffer[100];

// ✅ Toujours initialiser
int compteur = 0;
float ratio = 0.0f;
char buffer[100] = {};   // {} → remplit de zéros
```

### Règle 3 — Compiler en debug pour Valgrind

```bash
#  Sans symboles de debug → rapport inutile
g++-14 -O2 main.cpp -o monApp
valgrind ./monApp
# → "???" à la place des noms de fichiers

#  Avec symboles de debug → rapport précis
g++-14 -g -O0 main.cpp -o monApp
valgrind ./monApp
# → "main.cpp:42" → tu sais exactement où corriger
```

### Règle 4 — Intégrer Valgrind dans ta routine

```
Développement normal :
→ code + compile + teste avec Google Test

Avant un commit important :
→ cmake --build . --target valgrind_tests
→ zéro erreur → tu commites 

Avant une release :
→ valgrind --leak-check=full sur l'application complète
→ objectif : ERROR SUMMARY: 0 errors 
```

### Règle 5 — Ne pas paniquer avec "still reachable"

```
still reachable ≠ fuite mémoire !

C'est de la mémoire encore accessible mais pas libérée
→ souvent des variables statiques ou globales
→ libérées par l'OS à la fin du programme
→ acceptable dans la plupart des cas

Exemple courant :
→ bibliothèques système (pthread, OpenSSL...)
→ variables statiques de cout, cin...
→ tu n'as pas à t'en inquiéter !
```

---

## 11. Résumé des commandes

```bash
# Installation
sudo apt install valgrind

# Compilation indispensable pour Valgrind
g++-14 -std=c++23 -g -O0 main.cpp -o monApp

# Lancement basique
valgrind ./monApp

# Lancement complet avec rapport détaillé
valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         ./monApp

# Pour la CI/CD (retourne 1 si erreur)
valgrind --leak-check=full \
         --error-exitcode=1 \
         ./monApp

# Sur les tests Google Test
valgrind --leak-check=full \
         ./mes_tests

# Via CMake (si cible valgrind configurée)
cmake --build . --target valgrind
cmake --build . --target valgrind_tests

# Filtrer les résultats (ignorer les fuites système)
valgrind --leak-check=full \
         --suppressions=/usr/lib/valgrind/default.supp \
         ./monApp
```

---

## Exemple complet — code à tester

```c++
// main.cpp — exemples de fuites pour pratiquer avec Valgrind
#include <iostream>
#include <memory>
#include "Camera.h"

//  Fonction avec plusieurs fuites
void avecFuites() {
    // Fuite 1 — Camera non libérée
    Camera* cam = new Camera();
    cam->initialiser();
    cam->setResolution(1920, 1080);

    // Fuite 2 — Frame non libérée (6 Mo !)
    Frame* frame = new Frame();
    frame->largeur = 1920;
    frame->hauteur = 1080;
    frame->pixels.resize(1920 * 1080 * 3, 0);

    std::cout << "Frame : "
              << frame->largeur << "x"
              << frame->hauteur << std::endl;

    //  oubli de delete cam et delete frame !
}

// ✅ Fonction propre — zéro fuite
void sansFuite() {
    auto cam = std::make_unique<Camera>();
    cam->initialiser();
    cam->setResolution(1280, 720);

    auto frame = cam->capturerFrame();
    std::cout << "Frame : "
              << frame.largeur << "x"
              << frame.hauteur << std::endl;
}   // cam libéré automatiquement 

int main() {
    std::cout << "=== Test avec fuites ===" << std::endl;
    avecFuites();   // Valgrind détecte 2 fuites ici

    std::cout << "=== Test sans fuite ===" << std::endl;
    sansFuite();    // Valgrind : propre 

    return 0;
}
```

```bash
# Compiler et analyser
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=g++-14
cmake --build .

valgrind --leak-check=full --show-leak-kinds=all ./monApp

# Résultat attendu :
# definitely lost: ~6,220,808 bytes in 2 blocks (fuites de avecFuites)
# ERROR SUMMARY: 2 errors
#
# Après correction (utiliser unique_ptr) :
# All heap blocks were freed -- no leaks are possible 
# ERROR SUMMARY: 0 errors ✅
```

---

> **En résumé** : Valgrind est un outil indispensable pour tout développeur C++ sérieux. Il permet de détecter des bugs qui peuvent être très difficiles à trouver autrement. L'objectif est toujours d'arriver à `ERROR SUMMARY: 0 errors`. La meilleure façon d'y arriver est de systématiquement utiliser les smart pointers (`unique_ptr`, `shared_ptr`) et d'initialiser toutes ses variables. 😊