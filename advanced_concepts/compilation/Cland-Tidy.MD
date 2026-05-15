# Guide VS Code — CMake + clang-tidy + C++ Moderne

> Ce guide règle **tous les problèmes courants** de VS Code avec un projet C++ basé sur CMake :
> bibliothèques non reconnues, IntelliSense cassé, clang-tidy non configuré, mauvais compilateur...

---

## Table des matières

1. [Pourquoi VS Code galère avec C++ ?](#1-pourquoi-vs-code-galère-avec-c-)
2. [Extensions indispensables](#2-extensions-indispensables)
3. [Configurer CMake Tools](#3-configurer-cmake-tools)
4. [Configurer le compilateur GCC 14](#4-configurer-le-compilateur-gcc-14)
5. [Résoudre les bibliothèques non reconnues](#5-résoudre-les-bibliothèques-non-reconnues)
6. [Configurer clang-tidy](#6-configurer-clang-tidy)
7. [Fichiers de configuration expliqués](#7-fichiers-de-configuration-expliqués)
8. [Workflow complet](#8-workflow-complet)
9. [Problèmes fréquents et solutions](#9-problèmes-fréquents-et-solutions)

---

## 1. Pourquoi VS Code galère avec C++ ?

### CLion vs VS Code — la différence fondamentale

```
CLion :
→ comprend CMake nativement dès l'installation
→ lit CMakeLists.txt automatiquement
→ configure IntelliSense depuis CMake
→ clang-tidy intégré et activé par défaut
→ zéro configuration nécessaire

VS Code :
→ éditeur de texte de base à l'origine
→ tout repose sur des EXTENSIONS
→ chaque extension doit être configurée
→ sans configuration → il ne comprend PAS ton projet CMake
→ IntelliSense devine les headers au lieu de lire CMake
→ résultat : bibliothèques "non trouvées" alors qu'elles existent !
```

### Ce qui se passe sans configuration

```
Tu ouvres Camera.cpp dans VS Code :
→ VS Code voit #include <opencv2/core.hpp>
→ il cherche opencv2/core.hpp dans des chemins par défaut
→ pas trouvé → soulignement rouge ❌
→ pourtant OpenCV est installé et CMake le trouve très bien !

Pourquoi ?
→ VS Code n'a pas lu ton CMakeLists.txt
→ il ne sait pas que find_package(OpenCV) a trouvé OpenCV
→ il ne connaît pas les chemins ajoutés par target_include_directories()

Solution :
→ dire à VS Code d'utiliser CMake pour configurer IntelliSense ✅
```

---

## 2. Extensions indispensables

Installe ces extensions dans VS Code (`Ctrl+Shift+X`) :

### Extension 1 — C/C++ (Microsoft) — OBLIGATOIRE

```
Nom       : C/C++
Éditeur   : Microsoft
ID        : ms-vscode.cpptools
Rôle      : IntelliSense de base, débogage, formatage
```

### Extension 2 — CMake Tools — OBLIGATOIRE

```
Nom       : CMake Tools
Éditeur   : Microsoft
ID        : ms-vscode.cmake-tools
Rôle      : intégration CMake complète
           → lit CMakeLists.txt
           → configure automatiquement IntelliSense
           → boutons Build/Run dans la barre du bas
           → gère les profils Debug/Release
```

### Extension 3 — clangd — RECOMMANDÉE (remplace C/C++ pour l'analyse)

```
Nom       : clangd
Éditeur   : LLVM
ID        : llvm-vs-code-extensions.vscode-clangd
Rôle      : IntelliSense avancé basé sur clang
           → clang-tidy intégré
           → meilleure complétion que C/C++
           → suggestions de correction automatiques

⚠️ Si tu utilises clangd → désactiver l'IntelliSense de C/C++
   pour éviter les conflits (voir section 6)
```

### Extension 4 — CMake (optionnelle mais utile)

```
Nom       : CMake
Éditeur   : twxs
ID        : twxs.cmake
Rôle      : coloration syntaxique des CMakeLists.txt
           → autocomplétion dans CMakeLists.txt
```

---

## 3. Configurer CMake Tools

C'est **l'étape la plus importante**. CMake Tools va lire ton `CMakeLists.txt` et configurer tout VS Code automatiquement.

### Étape 1 — Ouvrir le dossier racine du projet

```
File → Open Folder → sélectionner le dossier racine
                      (celui qui contient CMakeLists.txt)

⚠️ IMPORTANT : ouvrir le DOSSIER RACINE, pas un sous-dossier !
   VS Code cherche CMakeLists.txt dans le dossier ouvert.
```

### Étape 2 — Sélectionner le kit (compilateur)

Après l'installation de CMake Tools, une barre apparaît en bas :

```
[No Kit Selected]  →  cliquer dessus
→ VS Code scanne les compilateurs disponibles
→ sélectionner : GCC 14.x.x x86_64-linux-gnu
```

Si GCC 14 n'apparaît pas :

```bash
# Vérifier que GCC 14 est installé
which g++-14
# → /usr/bin/g++-14

# Si absent → installer
sudo apt install gcc-14 g++-14
```

### Étape 3 — Configurer CMake

```
Ctrl+Shift+P → "CMake: Configure"
→ CMake Tools lit ton CMakeLists.txt
→ génère les fichiers de build dans build/
→ génère compile_commands.json → utilisé par clang-tidy !
```

### Étape 4 — Vérifier dans la barre du bas

```
[GCC 14.x.x] [Debug] [monApp] [▶ Build] [▶ Run]
      ↑           ↑       ↑
   compilateur  mode   cible
```

---

## 4. Configurer le compilateur GCC 14

### Méthode 1 — Via CMake Tools (recommandée)

Créer le fichier `.vscode/settings.json` à la racine du projet :

```json
{
    "cmake.generator": "Ninja",
    "cmake.buildDirectory": "${workspaceFolder}/build",
    "cmake.configureSettings": {
        "CMAKE_CXX_COMPILER": "/usr/bin/g++-14",
        "CMAKE_C_COMPILER": "/usr/bin/gcc-14",
        "CMAKE_BUILD_TYPE": "Debug",
        "CMAKE_EXPORT_COMPILE_COMMANDS": "ON"
    },
    "cmake.copyCompileCommands": "${workspaceFolder}/compile_commands.json"
}
```

> **`CMAKE_EXPORT_COMPILE_COMMANDS: ON`** est crucial ! Il génère
> `compile_commands.json` dont clang-tidy a besoin pour comprendre
> comment ton projet compile.

### Méthode 2 — Via CMakePresets.json (moderne)

Créer `CMakePresets.json` à la racine du projet :

```json
{
    "version": 3,
    "configurePresets": [
        {
            "name": "debug-gcc14",
            "displayName": "Debug GCC 14",
            "generator": "Ninja",
            "binaryDir": "${sourceDir}/build",
            "cacheVariables": {
                "CMAKE_BUILD_TYPE": "Debug",
                "CMAKE_CXX_COMPILER": "/usr/bin/g++-14",
                "CMAKE_C_COMPILER": "/usr/bin/gcc-14",
                "CMAKE_EXPORT_COMPILE_COMMANDS": "ON"
            }
        },
        {
            "name": "release-gcc14",
            "displayName": "Release GCC 14",
            "generator": "Ninja",
            "binaryDir": "${sourceDir}/build-release",
            "cacheVariables": {
                "CMAKE_BUILD_TYPE": "Release",
                "CMAKE_CXX_COMPILER": "/usr/bin/g++-14",
                "CMAKE_C_COMPILER": "/usr/bin/gcc-14",
                "CMAKE_EXPORT_COMPILE_COMMANDS": "ON"
            }
        }
    ]
}
```

---

## 5. Résoudre les bibliothèques non reconnues

### Pourquoi ça arrive ?

```
Scénario typique :

CMakeLists.txt :
    find_package(OpenCV REQUIRED)
    target_include_directories(monApp PRIVATE ${OpenCV_INCLUDE_DIRS})

Camera.cpp :
    #include <opencv2/core.hpp>   ← soulignement rouge dans VS Code !

Pourquoi ?
→ IntelliSense de VS Code ne lit pas CMakeLists.txt par défaut
→ il ne sait pas que ${OpenCV_INCLUDE_DIRS} = /usr/include/opencv4/
→ solution : lui faire utiliser compile_commands.json généré par CMake
```

### Solution complète — configurer IntelliSense depuis CMake

#### Étape 1 — Générer compile_commands.json

Ce fichier dit à VS Code **exactement** comment compiler chaque fichier
(mêmes flags, mêmes includes que CMake).

```bash
# Dans le terminal
cd ton_projet
mkdir -p build && cd build
cmake .. \
    -DCMAKE_CXX_COMPILER=g++-14 \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
# → génère build/compile_commands.json ✅

# Copier à la racine (certaines extensions le cherchent là)
cp build/compile_commands.json ../compile_commands.json
```

**Ou automatiquement dans `.vscode/settings.json` :**

```json
{
    "cmake.copyCompileCommands": "${workspaceFolder}/compile_commands.json"
}
```

#### Étape 2 — Configurer C/C++ IntelliSense

Créer `.vscode/c_cpp_properties.json` :

```json
{
    "configurations": [
        {
            "name": "Linux-GCC14",
            "compileCommands": "${workspaceFolder}/compile_commands.json",
            "intelliSenseMode": "linux-gcc-x64",
            "compilerPath": "/usr/bin/g++-14",
            "cppStandard": "c++23",
            "includePath": []
        }
    ],
    "version": 4
}
```

> **`compileCommands`** pointe vers `compile_commands.json`.
> VS Code va lire ce fichier pour savoir où chercher les headers.
> Fini les soulignements rouges pour OpenCV, Qt, etc. ✅

#### Étape 3 — Recharger VS Code

```
Ctrl+Shift+P → "Developer: Reload Window"
→ VS Code relit toute la configuration
→ IntelliSense se reconfigure depuis compile_commands.json
→ les soulignements rouges disparaissent ✅
```

---

## 6. Configurer clang-tidy

### Option A — Via l'extension clangd (recommandée)

`clangd` est un serveur de langage qui intègre clang-tidy nativement.
C'est **la meilleure option** pour VS Code.

#### Installation

```bash
# Installer clangd
sudo apt install clangd-18
# ou
sudo apt install clangd
```

#### Configuration dans `.vscode/settings.json`

```json
{
    "clangd.path": "/usr/bin/clangd",
    "clangd.arguments": [
        "--background-index",
        "--clang-tidy",
        "--clang-tidy-checks=bugprone-*,modernize-*,performance-*,readability-*",
        "--compile-commands-dir=${workspaceFolder}/build",
        "--completion-style=detailed",
        "--header-insertion=iwyu",
        "--log=verbose"
    ],
    "clangd.onConfigChanged": "restart",

    "C_Cpp.intelliSenseEngine": "disabled"
    // ↑ IMPORTANT : désactiver IntelliSense C/C++ pour éviter les conflits
    //   clangd prend le relais et fait mieux !
}
```

> **`--clang-tidy`** : active clang-tidy dans clangd.
> Les suggestions apparaissent directement dans l'éditeur,
> exactement comme dans CLion ! 💡

#### Vérifier que ça marche

```
Ouvrir Camera.cpp → clangd analyse le fichier
→ soulignements jaunes = warnings clang-tidy
→ 💡 ampoule → suggestions de correction
→ Ctrl+. (ou clic sur l'ampoule) → appliquer la correction
```

### Option B — Via l'extension C/C++ (plus simple)

Si tu gardes l'extension C/C++ de Microsoft sans clangd :

Dans `.vscode/settings.json` :

```json
{
    "C_Cpp.codeAnalysis.clangTidy.enabled": true,
    "C_Cpp.codeAnalysis.clangTidy.path": "/usr/bin/clang-tidy",
    "C_Cpp.codeAnalysis.clangTidy.config": "",
    "C_Cpp.codeAnalysis.runAutomatically": true,
    "C_Cpp.default.compileCommands": "${workspaceFolder}/compile_commands.json"
}
```

### Fichier .clang-tidy — configurer les règles

Créer `.clang-tidy` à la racine du projet :

```yaml
---
Checks: >
  -*,
  bugprone-*,
  -bugprone-easily-swappable-parameters,
  cppcoreguidelines-*,
  -cppcoreguidelines-avoid-magic-numbers,
  -cppcoreguidelines-pro-bounds-pointer-arithmetic,
  modernize-*,
  -modernize-use-trailing-return-type,
  performance-*,
  readability-*,
  -readability-magic-numbers,
  clang-analyzer-*

WarningsAsErrors: ''

HeaderFilterRegex: 'src/.*'

CheckOptions:
  - key: readability-identifier-naming.VariableCase
    value: 'camelCase'
  - key: readability-identifier-naming.MemberCase
    value: 'camelCase'
  - key: readability-identifier-naming.MemberPrefix
    value: 'm_'
  - key: readability-identifier-naming.ClassCase
    value: 'CamelCase'
  - key: readability-identifier-naming.FunctionCase
    value: 'camelCase'
  - key: modernize-use-auto.MinTypeNameLength
    value: '8'
```

---

## 7. Fichiers de configuration expliqués

### Structure complète des fichiers VS Code

```
ton_projet/
├── .vscode/
│   ├── settings.json          ← configuration VS Code + CMake Tools
│   ├── c_cpp_properties.json  ← configuration IntelliSense
│   ├── launch.json            ← configuration débogage
│   └── tasks.json             ← tâches build custom
├── .clang-tidy                ← règles clang-tidy
├── CMakeLists.txt
├── CMakePresets.json          ← profils CMake (optionnel)
├── compile_commands.json      ← généré par CMake → ne pas versionner !
└── src/
    ├── Camera.cpp
    └── Camera.h
```

> **`.gitignore`** — ajouter ces fichiers générés :
> ```gitignore
> compile_commands.json
> .vscode/ipch/
> .vscode/*.db
> ```
>
> **Versionner** ces fichiers :
> ```
> .vscode/settings.json       ✅ versionner (config partagée équipe)
> .vscode/c_cpp_properties.json ✅ versionner
> .clang-tidy                 ✅ versionner
> CMakePresets.json           ✅ versionner
> ```

---

### `.vscode/settings.json` — complet et commenté

```json
{
    // ─────────────────────────────────────
    // CMake Tools
    // ─────────────────────────────────────
    "cmake.generator": "Ninja",
    "cmake.buildDirectory": "${workspaceFolder}/build",
    "cmake.installPrefix": "${workspaceFolder}/install",

    // Compilateur GCC 14
    "cmake.configureSettings": {
        "CMAKE_CXX_COMPILER": "/usr/bin/g++-14",
        "CMAKE_C_COMPILER": "/usr/bin/gcc-14",
        "CMAKE_BUILD_TYPE": "Debug",
        "CMAKE_EXPORT_COMPILE_COMMANDS": "ON"
    },

    // Copier compile_commands.json à la racine automatiquement
    "cmake.copyCompileCommands": "${workspaceFolder}/compile_commands.json",

    // Configurer IntelliSense depuis CMake automatiquement
    "cmake.configureOnOpen": true,
    "C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools",

    // ─────────────────────────────────────
    // clangd (si tu utilises clangd)
    // ─────────────────────────────────────
    "clangd.path": "/usr/bin/clangd",
    "clangd.arguments": [
        "--background-index",
        "--clang-tidy",
        "--compile-commands-dir=${workspaceFolder}/build",
        "--completion-style=detailed",
        "--log=error"
    ],

    // Désactiver IntelliSense C/C++ si tu utilises clangd
    // "C_Cpp.intelliSenseEngine": "disabled",

    // ─────────────────────────────────────
    // clang-tidy via C/C++ (si tu n'utilises PAS clangd)
    // ─────────────────────────────────────
    // "C_Cpp.codeAnalysis.clangTidy.enabled": true,
    // "C_Cpp.codeAnalysis.clangTidy.path": "/usr/bin/clang-tidy",
    // "C_Cpp.codeAnalysis.runAutomatically": true,

    // ─────────────────────────────────────
    // Éditeur
    // ─────────────────────────────────────
    "editor.formatOnSave": true,
    "editor.defaultFormatter": "llvm-vs-code-extensions.vscode-clangd",
    "C_Cpp.default.cppStandard": "c++23",
    "files.associations": {
        "*.h": "cpp",
        "*.hpp": "cpp"
    }
}
```

---

### `.vscode/c_cpp_properties.json` — complet et commenté

```json
{
    "configurations": [
        {
            "name": "Linux-GCC14",

            // Source de vérité → compile_commands.json généré par CMake
            // TOUTES les infos (includes, defines, flags) viennent d'ici
            "compileCommands": "${workspaceFolder}/compile_commands.json",

            // Compilateur utilisé
            "compilerPath": "/usr/bin/g++-14",
            "intelliSenseMode": "linux-gcc-x64",

            // Standard C++
            "cppStandard": "c++23",

            // Laisser vide → compileCommands gère tout !
            // Ne PAS ajouter manuellement des chemins ici
            // si tu utilises compileCommands
            "includePath": [],
            "defines": []
        }
    ],
    "version": 4
}
```

> **Règle importante** : si `compileCommands` est défini,
> ne pas ajouter de chemins dans `includePath`.
> CMake via `compile_commands.json` gère tout automatiquement.

---

### `.vscode/launch.json` — débogage

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Debug monApp",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/build/monApp",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "miDebuggerPath": "/usr/bin/gdb",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ],
            "preLaunchTask": "cmake-build"
        }
    ]
}
```

---

### `.vscode/tasks.json` — tâches build

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "cmake-configure",
            "type": "shell",
            "command": "cmake",
            "args": [
                "-S", "${workspaceFolder}",
                "-B", "${workspaceFolder}/build",
                "-DCMAKE_CXX_COMPILER=g++-14",
                "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON",
                "-DCMAKE_BUILD_TYPE=Debug"
            ],
            "group": "build"
        },
        {
            "label": "cmake-build",
            "type": "shell",
            "command": "cmake",
            "args": [
                "--build", "${workspaceFolder}/build",
                "--parallel"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "dependsOn": "cmake-configure"
        },
        {
            "label": "valgrind",
            "type": "shell",
            "command": "valgrind",
            "args": [
                "--leak-check=full",
                "--show-leak-kinds=all",
                "--track-origins=yes",
                "${workspaceFolder}/build/monApp"
            ],
            "group": "test",
            "dependsOn": "cmake-build"
        },
        {
            "label": "clang-tidy",
            "type": "shell",
            "command": "clang-tidy",
            "args": [
                "-p", "${workspaceFolder}/build",
                "${workspaceFolder}/src/Camera.cpp",
                "${workspaceFolder}/src/Calculatrice.cpp"
            ],
            "group": "test"
        }
    ]
}
```

---

## 8. Workflow complet

### Première ouverture du projet

```
1. Ouvrir VS Code → File → Open Folder → racine du projet

2. VS Code détecte CMakeLists.txt → propose de configurer
   → cliquer "Yes" OU Ctrl+Shift+P → "CMake: Configure"

3. Sélectionner le kit : GCC 14.x.x
   (barre du bas → [No Kit Selected])

4. CMake configure → génère build/ et compile_commands.json

5. Ctrl+Shift+P → "Developer: Reload Window"
   → IntelliSense se reconfigure depuis compile_commands.json

6. Ouvrir un .cpp → plus de soulignements rouges ✅
```

### Workflow quotidien

```
Modifier du code C++ :
→ clang-tidy souligne les problèmes en temps réel
→ 💡 Ctrl+. → corriger automatiquement

Compiler :
→ Ctrl+Shift+B (Build par défaut)
→ ou bouton ▶ Build dans la barre du bas

Déboguer :
→ F5 → lance le débogueur GDB
→ points d'arrêt avec clic dans la marge

Lancer les tests :
→ Ctrl+Shift+P → "CMake: Run Tests"
→ ou terminal : cd build && ctest

Analyser la mémoire :
→ Ctrl+Shift+P → "Tasks: Run Task" → valgrind
```

---

## 9. Problèmes fréquents et solutions

### Problème 1 — Bibliothèques toujours soulignées en rouge

```
Symptôme : #include <opencv2/core.hpp> → rouge
           même après configuration

Causes possibles et solutions :

Cause A → compile_commands.json non généré
Solution :
    cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    cp build/compile_commands.json .

Cause B → c_cpp_properties.json ne pointe pas vers compile_commands.json
Solution :
    Vérifier que "compileCommands" pointe vers le bon chemin
    Ctrl+Shift+P → "C/C++: Edit Configurations (JSON)"

Cause C → VS Code n'a pas rechargé
Solution :
    Ctrl+Shift+P → "Developer: Reload Window"

Cause D → conflit entre clangd et C/C++
Solution :
    Choisir l'UN ou l'AUTRE, pas les deux !
    Si clangd → "C_Cpp.intelliSenseEngine": "disabled"
```

### Problème 2 — GCC 13 utilisé au lieu de GCC 14

```
Symptôme : erreur <print> non trouvé
           GCC 13.x.x dans la barre du bas

Solution :
    Barre du bas → cliquer sur [GCC 13.x.x]
    → sélectionner GCC 14.x.x

    OU dans .vscode/settings.json :
    "cmake.configureSettings": {
        "CMAKE_CXX_COMPILER": "/usr/bin/g++-14"
    }

    Puis Ctrl+Shift+P → "CMake: Configure"
```

### Problème 3 — clang-tidy ne fonctionne pas

```
Symptôme : aucun soulignement clang-tidy
           aucune suggestion 💡

Solution avec clangd :
    1. Vérifier que clangd est installé : which clangd
    2. Vérifier que --clang-tidy est dans clangd.arguments
    3. Vérifier que compile_commands.json existe dans build/
    4. Ctrl+Shift+P → "clangd: Restart language server"

Solution avec C/C++ :
    1. "C_Cpp.codeAnalysis.clangTidy.enabled": true dans settings.json
    2. Vérifier clang-tidy installé : which clang-tidy
    3. Redémarrer VS Code
```

### Problème 4 — IntelliSense très lent ou qui plante

```
Symptôme : VS Code rame, CPU à 100%

Cause : conflit entre clangd et C/C++ IntelliSense

Solution :
    Choisir clangd OU C/C++, pas les deux !
    
    Si tu veux clangd (recommandé) :
    "C_Cpp.intelliSenseEngine": "disabled"

    Si tu veux C/C++ :
    Désinstaller l'extension clangd
```

### Problème 5 — cmake.copyCompileCommands ne fonctionne pas

```
Symptôme : compile_commands.json n'est pas copié à la racine

Solution manuelle :
    cd ton_projet/build
    cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    cp compile_commands.json ../

    Ou ajouter dans CMakeLists.txt :
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
```

### Problème 6 — Kit GCC 14 non détecté par VS Code

```
Symptôme : GCC 14 n'apparaît pas dans la liste des kits

Solution :
    Ctrl+Shift+P → "CMake: Edit User-Local CMake Kits"
    Ajouter manuellement :
    [
        {
            "name": "GCC 14.2.0",
            "compilers": {
                "C": "/usr/bin/gcc-14",
                "CXX": "/usr/bin/g++-14"
            },
            "isTrusted": true
        }
    ]
```

---

## Résumé — checklist de configuration

```
□ Extension "CMake Tools" installée
□ Extension "C/C++" OU "clangd" installée
□ .vscode/settings.json créé avec :
    □ CMAKE_CXX_COMPILER = g++-14
    □ CMAKE_EXPORT_COMPILE_COMMANDS = ON
    □ cmake.copyCompileCommands configuré
□ .vscode/c_cpp_properties.json créé avec :
    □ compileCommands pointant vers compile_commands.json
□ .clang-tidy créé à la racine
□ CMake configuré (Ctrl+Shift+P → CMake: Configure)
□ Kit GCC 14 sélectionné dans la barre du bas
□ VS Code rechargé (Developer: Reload Window)
□ compile_commands.json présent à la racine

Résultat attendu :
✅ Plus de soulignements rouges pour les bibliothèques
✅ IntelliSense fonctionne avec OpenCV, Qt...
✅ clang-tidy souligne les problèmes en temps réel
✅ GCC 14 utilisé pour compiler
✅ Débogage GDB fonctionnel avec F5
```