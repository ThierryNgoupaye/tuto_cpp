# Guide Profiling C++ — Perf & Hotspot

> Trouver et éliminer les goulots d'étranglement de performance
> dans un programme C++ sur Linux.

---

## Table des matières

1. [C'est quoi le profiling ?](#1-cest-quoi-le-profiling-)
2. [Installation](#2-installation)
3. [Préparer le code](#3-préparer-le-code)
4. [Perf — utilisation complète](#4-perf--utilisation-complète)
5. [Hotspot — interface graphique](#5-hotspot--interface-graphique)
6. [Perf stat — métriques globales](#6-perf-stat--métriques-globales)
7. [Lire et comprendre un Flamegraph](#7-lire-et-comprendre-un-flamegraph)
8. [Chrono manuel — alternative simple](#8-chrono-manuel--alternative-simple)
9. [Workflow profiling complet](#9-workflow-profiling-complet)
10. [Problèmes fréquents](#10-problèmes-fréquents)
11. [Résumé des commandes](#11-résumé-des-commandes)

---

## 1. C'est quoi le profiling ?

### Définition

Le profiling consiste à **mesurer où ton programme passe son temps** pendant son exécution, afin d'identifier les parties du code qui consomment le plus de ressources (CPU, mémoire, etc.).

### La règle des 80/20

```
80% du temps d'exécution est passé dans 20% du code.

→ Trouver ces 20% = le but du profiling !
→ Optimiser les 80% restants = perte de temps !
```

### Analogie

```
Sans profiling = chercher une fuite d'eau en changeant
                 TOUTES les canalisations de la maison

Avec profiling = utiliser un détecteur de fuite
                 → trouver EXACTEMENT où ça fuit
                 → réparer seulement là ✅
                 → économie de temps et d'énergie !
```

### Pourquoi ne pas optimiser à l'aveugle ?

```cpp
// Tu penses que ça :
void calculerMoyenne(std::vector<int>& v) {
    double somme = 0;
    for (auto x : v) somme += x;          // ← tu optimises ici
    return somme / v.size();
}

// Est le problème. Mais Perf te montre que ça :
void chargerFichier(const std::string& path) {
    // lecture disque → 10 000x plus lent que le calcul !
    std::ifstream f(path);
    // ...
}

// → tu aurais optimisé le mauvais endroit ! 😱
```

---

## 2. Installation

```bash
# Perf — profiler Linux
sudo apt install linux-tools-common \
                 linux-tools-generic \
                 linux-tools-$(uname -r)

# Hotspot — interface graphique pour Perf
sudo apt install hotspot

# Vérifier les installations
perf --version
# → perf version 6.x.x ✅

which hotspot
# → /usr/bin/hotspot ✅
```

### Autoriser Perf sans sudo (recommandé)

Par défaut, `perf` nécessite `sudo`. C'est gênant car les fichiers
générés appartiennent à root. La solution :

```bash
# Autoriser perf pour les utilisateurs normaux
echo 1 | sudo tee /proc/sys/kernel/perf_event_paranoid

# Rendre permanent (survit aux redémarrages)
echo "kernel.perf_event_paranoid=1" | sudo tee -a /etc/sysctl.conf
sudo sysctl -p

# Maintenant perf fonctionne sans sudo !
perf record -g ./monApp    # ✅ pas besoin de sudo
hotspot perf.data          # ✅ même utilisateur → pas de problème de droits
```

---

## 3. Préparer le code

### Choisir le bon mode de compilation

```
Debug         (-O0 -g)  → pas représentatif !
                           le vrai programme tourne optimisé
                           → profiling inutile

Release       (-O2)     → optimisé mais sans symboles
                           → Perf ne peut pas nommer les fonctions
                           → rapport illisible

RelWithDebInfo(-O2 -g)  → OPTIMAL pour le profiling ✅
                           → optimisé comme en production
                           → Perf voit les noms des fonctions
```

### Dans CMakeLists.txt

```cmake
target_compile_options(monApp PRIVATE
    # Mode spécial profiling
    $<$<CONFIG:RelWithDebInfo>:
        -O2                      # optimisation comme en production
        -g                       # symboles debug pour Perf
        -fno-omit-frame-pointer  # garde les frame pointers
    >                            # → pile d'appels complète dans Perf !
)
```

### Compiler en RelWithDebInfo

```bash
# Supprimer l'ancien build
rm -rf build/

# Configurer en RelWithDebInfo
cmake .. \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo \
    -DCMAKE_CXX_COMPILER=g++-14

# Compiler
cmake --build .
```

---

## 4. Perf — utilisation complète

### 4.1 Enregistrer les performances

```bash
# Enregistrement de base
perf record ./monApp

# Enregistrement avec pile d'appels complète (recommandé)
perf record -g ./monApp
#            ↑
#      -g = enregistre la pile d'appels à chaque échantillon
#           indispensable pour les flamegraphs
#           et pour comprendre QUI appelle la fonction lente

# → génère perf.data dans le dossier courant
# → fichier binaire contenant tous les échantillons
```

### 4.2 Lire le rapport

```bash
# Rapport interactif dans le terminal
perf report

# Rapport non interactif (pour scripts CI/CD)
perf report --stdio

# Rapport avec code source annoté
perf report --stdio --show-nr-samples
```

### 4.3 Navigation dans perf report

```
Touches clavier dans perf report :

↑ ↓          → naviguer entre les fonctions
Entrée       → voir le détail d'une fonction
              → voir le code source annoté si disponible
+            → déplier la pile d'appels
-            → replier
Tab          → passer à la prochaine ligne chaude
q            → quitter
h            → aide
```

### 4.4 Exemple de rapport commenté

```
Samples: 12K of event 'cycles', Event count: 8,234,567,890

Overhead  Command  Shared Object   Symbol
─────────────────────────────────────────────────────────────
  68.23%  monApp   monApp          trierLentement(std::vector...)
  ↑
  68% du temps passé ici → GOULOT PRINCIPAL à optimiser !

  24.51%  monApp   monApp          calculerSomme(std::vector...)
  ↑
  24% du temps → second goulot, à optimiser après

   5.12%  monApp   libm.so         __sin_avx
  ↑
  appelé par calculerSomme → std::sin() interne

   1.89%  monApp   monApp          main
   0.25%  monApp   libc.so         __memcpy_avx_unaligned

Lecture :
→ Overhead % = pourcentage du temps CPU
→ Shared Object = bibliothèque ou exécutable
→ Symbol = nom de la fonction
→ Plus le % est grand → plus c'est prioritaire à optimiser
```

---

## 5. Hotspot — interface graphique

Hotspot est une **interface graphique** qui rend la lecture de `perf.data`
beaucoup plus agréable et intuitive.

### Lancer Hotspot

```bash
# Après perf record -g ./monApp
hotspot perf.data

# Si perf.data appartient à root (lancé avec sudo)
sudo chown $USER:$USER perf.data
hotspot perf.data
```

### Les onglets de Hotspot

#### Onglet "Summary"

```
→ vue d'ensemble rapide
→ temps total, CPUs utilisés
→ liste des fonctions les plus consommatrices
→ graphique en barres coloré
→ premier onglet à regarder !
```

#### Onglet "Bottom Up"

```
→ liste des fonctions feuilles (sans appels)
→ montre QUI consomme réellement
→ utile pour trouver la vraie cause
   (pas juste la fonction intermédiaire)
```

#### Onglet "Top Down"

```
→ liste depuis le point d'entrée (main)
→ montre l'arbre d'appels complet
→ utile pour comprendre LE CHEMIN vers le goulot
```

#### Onglet "Flame Graph" ← le plus puissant

```
→ visualisation en flammes
→ axe X = temps total (largeur = temps consommé)
→ axe Y = pile d'appels (profondeur = qui appelle qui)
→ cliquer sur une barre → zoomer
→ voir section 7 pour la lecture détaillée
```

#### Onglet "Caller/Callee"

```
→ sélectionner une fonction
→ voir QUI l'appelle (callers)
→ voir QU'ELLE appelle (callees)
→ utile pour comprendre le contexte d'une fonction lente
```

---

## 6. Perf stat — métriques globales

`perf stat` donne des **statistiques globales** sur l'exécution sans
créer de fichier perf.data. C'est la première commande à lancer.

```bash
perf stat ./monApp
```

### Exemple de sortie complète commentée

```
Performance counter stats for './monApp':

      2,345.67 msec task-clock         #  0.998 CPUs utilized
      ↑ temps CPU total utilisé

             3      context-switches   #  1.279 /sec
             ↑ changements de contexte (peu = bon)

             0      cpu-migrations     #  0.000 /sec
             ↑ migrations entre cœurs (0 = parfait)

           127      page-faults        #  54.12 /sec
           ↑ défauts de page mémoire

 6,234,567,890      cycles             #  2.658 GHz
 ↑ cycles CPU totaux

 8,901,234,567      instructions       #  1.43  insn per cycle
 ↑ instructions exécutées             ↑ IPC

 1,234,567,890      branches           #  526.3 M/sec
 ↑ branchements (if/else, boucles)

     2,345,678      branch-misses      #  0.19% of all branches ✅
     ↑ prédictions de branche ratées

   456,789,012      cache-references   #  194.7 M/sec
    12,345,678      cache-misses       #  2.70% of cache refs ✅
    ↑ accès mémoire qui ont raté le cache

       2.350431819 seconds time elapsed
       ↑ temps réel écoulé (wall clock)
```

### Métriques clés à surveiller

#### IPC (Instructions Per Cycle)

```
IPC = instructions / cycles

> 3.0  → excellent, CPU très bien utilisé
> 2.0  → bon ✅
1.0-2.0→ correct
< 1.0  → mauvais → CPU attend beaucoup
           causes : cache miss, branches ratées,
                   dépendances de données
```

#### Branch misses (prédictions ratées)

```
< 1%   → excellent ✅
1-5%   → acceptable
> 5%   → problème de branch prediction
          solution : trier les données avant de filtrer
                    utiliser [[likely]] / [[unlikely]]
                    branchless programming
```

#### Cache misses

```
< 3%   → excellent ✅
3-10%  → acceptable
> 10%  → problème de cache locality !
          solution : passer de AoS à SoA
                    améliorer l'ordre de parcours
                    (ligne par ligne au lieu de colonne)
```

---

## 7. Lire et comprendre un Flamegraph

Le flamegraph est la visualisation la plus puissante du profiling.

### Structure d'un flamegraph

```
                    ┌────────────────────────────────────┐
                    │         trierLentement() 68%       │
          ┌─────────┴──────────┬─────────────────────────┐
          │   std::swap() 45%  │   comparaison  23%      │
 ┌────────┴──────────┬─────────┴─────────────────────────┤
 │   main()  100%                                        │
 ├───────────────────────────────────────────────────────┤
 │                    _start  100%                       │
 └───────────────────────────────────────────────────────┘

Axe X (horizontal) = temps total
  → plus une barre est LARGE → plus elle consomme de temps
  → ← → la largeur = le pourcentage du temps CPU

Axe Y (vertical) = pile d'appels
  → bas = fonctions racines (main, _start)
  → haut = fonctions feuilles (sans appels)
  → chaque niveau = un appel de fonction supplémentaire
```

### Comment lire le flamegraph

```
Étape 1 → regarder les barres les plus LARGES au sommet
           → ce sont les fonctions qui consomment le plus
           → et qui n'appellent rien d'autre (elles calculent vraiment)

Étape 2 → remonter vers le bas pour comprendre QUI les appelle
           → trouver la chaîne d'appels complète

Étape 3 → cliquer sur une barre dans Hotspot
           → zoom sur cette fonction
           → voir ses appelants et ses appelés

Étape 4 → chercher les "plateaux" larges
           → une fonction large avec peu de fils
           → c'est elle qui fait le vrai travail
           → c'est là qu'optimiser !
```

### Exemple concret

```
Flamegraph d'un traitement d'image :

┌──────────────────────────────────────────────────────────┐
│  memcpy 15%  │     calcul gris 68%          │ autres 17% │
├──────────────┴──────────────────────────────┴────────────┤
│              traiterFrame() 100%                         │
├──────────────────────────────────────────────────────────┤
│                  boucle principale 100%                  │
├──────────────────────────────────────────────────────────┤
│                        main() 100%                       │
└──────────────────────────────────────────────────────────┘

Lecture :
→ "calcul gris 68%" → fonction la plus large au sommet
→ c'est ici qu'optimiser en priorité !
→ peut-être passer à par_unseq ou SIMD ?

→ "memcpy 15%" → second goulot
→ peut-être des copies inutiles ?
→ passer à move semantics ou span ?
```

---

## 8. Chrono manuel — alternative simple

Pour des mesures rapides **sans installer Perf**, utiliser `std::chrono`
directement dans le code.

```c++
#include <chrono>
#include <print>
#include <string_view>

// Classe chronomètre réutilisable
class Chronometre {
public:
    void demarrer() {
        m_debut = std::chrono::high_resolution_clock::now();
    }

    long long arreter(std::string_view nom = "") {
        auto fin = std::chrono::high_resolution_clock::now();
        auto duree = std::chrono::duration_cast<
            std::chrono::microseconds>(fin - m_debut).count();

        if (!nom.empty()) {
            std::println(" {} : {} µs ({:.3f} ms)",
                         nom,
                         duree,
                         duree / 1000.0);
        }
        return duree;
    }

private:
    std::chrono::high_resolution_clock::time_point m_debut;
};

// Macro pratique pour mesurer un bloc
#define MESURER(nom, code) \
    { \
        Chronometre __c; \
        __c.demarrer(); \
        code; \
        __c.arreter(nom); \
    }

// Exemple d'utilisation
int main() {
    const int W = 1920, H = 1080;
    std::vector<uint8_t> frame(W * H * 3, 128);

    Chronometre c;

    // Mesure version lente
    c.demarrer();
    for (int i = 0; i < 10; i++) {
        traiterFrameLent(frame, W, H);
    }
    auto t1 = c.arreter("Traitement lent  (x10)");

    // Mesure version rapide
    c.demarrer();
    for (int i = 0; i < 10; i++) {
        traiterFrameRapide(frame);
    }
    auto t2 = c.arreter("Traitement rapide (x10)");

    // Comparer
    std::println("─────────────────────────────");
    std::println("Gain : {:.1f}x plus rapide !", (double)t1 / t2);
    std::println("Économie : {} µs par appel",
                 (t1 - t2) / 10);

    // Avec la macro
    MESURER("std::sort", std::sort(frame.begin(), frame.end()));

    return 0;
}
```

### Chrono vs Perf — quand utiliser lequel ?

```
Chrono manuel :
✅ rapide à mettre en place
✅ pas d'installation nécessaire
✅ précis pour comparer deux implémentations
✅ fonctionne partout (Linux, Windows, macOS)
❌ intrusif → modifier le code source
❌ mesure seulement ce que tu as pensé à mesurer
❌ peut fausser les mesures (le chrono lui-même prend du temps)

Perf :
✅ non intrusif → pas de modification du code
✅ mesure TOUT automatiquement
✅ trouve les goulots INATTENDUS
✅ métriques hardware (cache, branches...)
❌ Linux seulement
❌ installation nécessaire
❌ courbe d'apprentissage

Règle pratique :
→ Perf → pour trouver où est le problème
→ Chrono → pour valider que l'optimisation a fonctionné
```

---

## 9. Workflow profiling complet

Voici le workflow professionnel à suivre systématiquement.

### Étape 1 — Mesurer la baseline

```bash
# Mesurer le temps de référence AVANT toute optimisation
perf stat ./monApp 2>&1 | tee baseline.txt

# Noter :
# - temps total (seconds time elapsed)
# - IPC
# - cache-misses %
# - branch-misses %
```

### Étape 2 — Profiler pour trouver les goulots

```bash
# Enregistrer avec pile d'appels
perf record -g ./monApp

# Analyser
perf report --stdio | head -30  # top 30 fonctions

# Ou visuellement
hotspot perf.data
```

### Étape 3 — Identifier la cause

```
Questions à se poser :

"Quelle fonction est la plus large dans le flamegraph ?"
→ c'est là qu'optimiser

"Pourquoi est-elle lente ?"
→ cache-misses élevés → problème AoS/SoA, ordre de parcours
→ branch-misses élevés → données non triées, if fréquents
→ IPC bas → dépendances de données, attente mémoire
→ algorithme O(n²) → changer d'algorithme

"Est-ce vraiment nécessaire ?"
→ peut-on éviter ce calcul ?
→ peut-on le mettre en cache ?
→ peut-on le paralléliser ?
```

### Étape 4 — Optimiser UNE chose à la fois

```c++
// Avant optimisation → traitement colonne par colonne
// cache-unfriendly → détecté par Perf (cache-misses > 10%)
void traiterLent(std::vector<uint8_t>& pixels,
                  int largeur, int hauteur) {
    for (int x = 0; x < largeur; x++) {       // ← colonne d'abord
        for (int y = 0; y < hauteur; y++) {   // ← ligne ensuite
            int idx = (y * largeur + x) * 3;  // ← sauts en mémoire !
            // traitement...
        }
    }
}

// Après optimisation → traitement linéaire
// cache-friendly → détecté par Perf (cache-misses < 2%)
void traiterRapide(std::vector<uint8_t>& pixels) {
    for (size_t i = 0; i < pixels.size(); i += 3) {  // ← linéaire ✅
        // traitement...
    }
}
```

### Étape 5 — Mesurer APRÈS optimisation

```bash
# Même mesure qu'à l'étape 1
perf stat ./monApp 2>&1 | tee apres_optim.txt

# Comparer
diff baseline.txt apres_optim.txt

# Ou afficher côte à côte
echo "=== AVANT ===" && cat baseline.txt
echo "=== APRÈS ===" && cat apres_optim.txt
```

### Étape 6 — Répéter si nécessaire

```
Après chaque optimisation :
→ profiler à nouveau
→ nouveau goulot identifié ?
→ optimiser à nouveau
→ jusqu'à ce que les performances soient satisfaisantes

⚠️ Règle importante :
→ ARRÊTER quand le code devient illisible
→ la performance n'est pas le seul critère !
→ code maintenable > code ultra-optimisé illisible
```

---

## 10. Problèmes fréquents

### Problème 1 — "perf.data n'est pas lisible" dans Hotspot

```
Cause : perf.data créé par sudo appartient à root
        hotspot lancé en utilisateur normal → accès refusé

Solutions :

Option A — Lancer hotspot avec sudo
    sudo hotspot perf.data

Option B — Changer le propriétaire
    sudo chown $USER:$USER perf.data
    hotspot perf.data

Option C — Ne plus utiliser sudo pour perf (recommandé ✅)
    echo 1 | sudo tee /proc/sys/kernel/perf_event_paranoid
    # Puis lancer perf sans sudo :
    perf record -g ./monApp
    hotspot perf.data
```

### Problème 2 — Fonctions nommées "???" dans le rapport

```
Cause : pas de symboles de debug
        → compilé en Release sans -g

Solution : recompiler en RelWithDebInfo
    cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo
    cmake --build .
    perf record -g ./monApp
```

### Problème 3 — Pas assez d'échantillons

```
Cause : programme trop rapide
        → perf n'a pas le temps d'échantillonner

Solution : augmenter la charge de travail
    for (int i = 0; i < 1000; i++) {
        maFonction();  // répéter pour donner du temps à perf
    }

Ou augmenter la fréquence d'échantillonnage :
    perf record -F 10000 -g ./monApp
    #            ↑ 10 000 échantillons par seconde
```

### Problème 4 — "perf: command not found"

```bash
# Installer la version correspondant au kernel actuel
sudo apt install linux-tools-$(uname -r)
sudo apt install linux-tools-generic
sudo apt install linux-tools-common
```

### Problème 5 — Résultats non reproductibles

```
Cause : ASLR (Address Space Layout Randomization)
        ou fréquence CPU variable (turbo boost)

Solution : désactiver temporairement pour les benchmarks
    # Désactiver ASLR
    echo 0 | sudo tee /proc/sys/kernel/randomize_va_space

    # Fixer la fréquence CPU
    sudo cpupower frequency-set -g performance

    # Faire les mesures
    perf stat -r 5 ./monApp  # répéter 5 fois → moyenne

    # Remettre en ordre après
    echo 2 | sudo tee /proc/sys/kernel/randomize_va_space
    sudo cpupower frequency-set -g powersave
```

---

## 11. Résumé des commandes

```bash
# ─────────────────────────────────────
# Installation
# ─────────────────────────────────────
sudo apt install linux-tools-common linux-tools-generic \
                 linux-tools-$(uname -r) hotspot

# Autoriser perf sans sudo (une fois)
echo 1 | sudo tee /proc/sys/kernel/perf_event_paranoid
echo "kernel.perf_event_paranoid=1" | sudo tee -a /etc/sysctl.conf

# ─────────────────────────────────────
# Compilation
# ─────────────────────────────────────
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo \
         -DCMAKE_CXX_COMPILER=g++-14
cmake --build .

# ─────────────────────────────────────
# Perf — statistiques globales
# ─────────────────────────────────────
perf stat ./monApp                    # statistiques de base
perf stat -r 5 ./monApp              # répéter 5 fois → moyenne
perf stat -e cache-misses,cache-references,\
             branch-misses,branches ./monApp  # métriques spécifiques

# ─────────────────────────────────────
# Perf — enregistrement
# ─────────────────────────────────────
perf record ./monApp                  # enregistrement simple
perf record -g ./monApp              # avec pile d'appels ← toujours utiliser !
perf record -F 10000 -g ./monApp     # fréquence élevée (10 000 Hz)

# ─────────────────────────────────────
# Perf — lecture du rapport
# ─────────────────────────────────────
perf report                           # rapport interactif
perf report --stdio                   # rapport texte (non interactif)
perf report --stdio | head -20        # top 20 fonctions

# ─────────────────────────────────────
# Hotspot — interface graphique
# ─────────────────────────────────────
hotspot perf.data                     # ouvrir le rapport
sudo chown $USER:$USER perf.data      # si droits insuffisants

# ─────────────────────────────────────
# Workflow complet en une commande
# ─────────────────────────────────────
perf record -g ./monApp && hotspot perf.data
```

---

## Tableau récapitulatif des outils

| Outil         | Commande                  | Rôle                                      | Quand l'utiliser               |
|---------------|---------------------------|-------------------------------------------|--------------------------------|
| `perf stat`   | `perf stat ./monApp`      | Métriques globales (IPC, cache, branches) | Première étape, vue d'ensemble |
| `perf record` | `perf record -g ./monApp` | Enregistrer les échantillons              | Trouver les fonctions lentes   |
| `perf report` | `perf report`             | Lire le rapport en terminal               | Analyse rapide sans GUI        |
| `hotspot`     | `hotspot perf.data`       | Interface graphique + flamegraph          | Analyse détaillée et visuelle  |
| `Chrono`      | `std::chrono`             | Mesurer dans le code                      | Valider une optimisation       |

---

## Métriques de référence

| Métrique          | Excellent | Bon   | Problème |
|-------------------|-----------|-------|----------|
| IPC               | > 3.0     | > 2.0 | < 1.0    |
| Cache misses      | < 1%      | < 3%  | > 10%    |
| Branch misses     | < 0.5%    | < 1%  | > 5%     |
| Gain optimisation | > 10x     | > 2x  | < 1.5x   |

---

> **Règle d'or du profiling** :
> Toujours **mesurer avant** d'optimiser, et **mesurer après** pour valider.
> Ne jamais optimiser à l'aveugle. Le compilateur avec `-O2` fait déjà
> beaucoup de travail — concentre-toi sur les algorithmes et la
> structure des données avant les micro-optimisations. 