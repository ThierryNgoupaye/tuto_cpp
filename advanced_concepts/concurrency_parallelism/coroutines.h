//
// Created by thierry on 09/05/2026.
//

#ifndef TUTO_CPP_COROUTINES_H
#define TUTO_CPP_COROUTINES_H

#include <iostream>
#include <coroutine>
#include <generator>
#include <thread>
#include <string>
#include <chrono>

// Une coroutine est une fonction s'exécuté et peut se suspendre, puis continuer son éxécution où elle s'était arretée.
// Vocabulaire
// co_yield (produire) = voici un résultat partiel, je mets mon marque page et je te rends le controle
// co_await (attendre) j'ai besoin de télécharger une image, je mets mon marque page ici. Va faire autre achose, et reveille toi uniquement quand l'image sera arrivée
// co_return: j'ai fini tout mon libre, tu peux jeter le marque-page.


using string = std::string;
using thread = std::thread;

struct GenerateurCustom {
    struct promise_type {
        int valeur_actuelle;

        // Que retourner quand la coroutine est créée.
        GenerateurCustom get_return_object() {
            return GenerateurCustom{std::coroutine_handle<promise_type>::from_promise(*this)};
        }

        // Que faire au debut.
        std::suspend_always initial_suspend() { return  {};}
       // std::suspend_never initial_suspend();    démarre immédiatement.

        // Que faire à la fin
        std::suspend_always final_suspend() noexcept { return  {}; };
       // std::suspend_never final_suspend() noexcept;     détruit immédiatement.

        // Que faire avec un co_yield.
        std::suspend_always yield_value(const int value) {
            valeur_actuelle = value;
            return {};
        }

        // Que faire avec un co_return.
        void return_void() {}

        // Que faire s'il y a une exception.
        void unhandled_exception() {};
    };

    std::coroutine_handle<promise_type> handle;
};


inline GenerateurCustom ma_coroutine() {
    std::println(std::cout, "[Coroutine] je démarre et je calcule 10..,");
    co_yield 10;

    std::println(std::cout, "[Coroutine] On m'a réveillé ! Je calcule 20...");
    co_yield 20;

    std::println(std::cout, "[Coroutine] Je termine.");
    co_return;
}


inline void test_coroutine() {
    std::println(std::cout, "[Main] J'appelle la coroutine");
    auto [handle] = ma_coroutine();

    std::println(std::cout, "[Main] J'appuie sur PLAY (resume) une 1ere fois");
    handle.resume();
    std::println(std::cout, "[Main] Valeur recue {}", handle.promise().valeur_actuelle);

    std::println(std::cout, "[Main] J'appuie sur PLAY (resume) une 2eme fois.");
    handle.resume();
    std::println(std::cout, "[Main] Valeur recue {}", handle.promise().valeur_actuelle);

    std::println(std::cout, "[Main] J'appuie sur PLAY pour qu'elle en finisse.");
    handle.resume();

    // destruction de la télécommande.
    handle.destroy();
}



// Avec generator
inline std::generator<int> ma_coroutine_pro() {
    co_yield 10;
    co_yield 20;
}

inline void coroutine_pro_use() {
    for (int valeur : ma_coroutine_pro()) {
        std::println(std::cout, "Valeur {}", valeur);
    }
}



// coroutine avec co_await
struct BipeurRestaurant {
    string commande;

    bool await_ready() {
        std::println(std::cout, "[Bipeur] Vérification : Le {} est-il déja pret ? Non.",commande);
        return false;
    }

    void await_suspend(std::coroutine_handle<> handle) {
        std::println(std::cout, "[Bipeur] La coroutine se emt en PAUSE. Je donne la commande à la cuisine.");

        // simulation de la cuisine
        thread([this, handle] {
            std::println(std::cout, "(Cuisine) Cuisson de {} en cours (2 secondes) ...", commande);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::println(std::cout, "(Cuisine) C'est pret ! Je fais vibrer le bipeur (resume)");
            handle.resume();

        }).detach();
    }


    [[nodiscard]] string await_resume() const {
        std::println(std::cout, "[Bipeur] La coroutine s'est réveillée. On récupère la commande.");
        return commande + " bien chaud";
    }
};



// Obligatoire pour dire au C++ que notre fonction a le droit d'etre une coroutine
struct TacheAsynchrone {
    struct promise_type {
        TacheAsynchrone get_return_object() { return {}; };
        std::suspend_never initial_suspend() { return {}; };
        std::suspend_never final_suspend() noexcept { return {}; };
        void return_void() {};
        void unhandled_exception() {};

    };
};


inline TacheAsynchrone aller_au_fast_food() {
    std::println(std::cout, "[Client] J'arrive au comptoir et je passe commande.");

    string repas = co_await BipeurRestaurant{"Burger Maison"};

    std::println(std::cout, "[Client] Miam, je mange mon {} !", repas);
}


inline void await_coroutine_use() {
    std::println(std::cout, "[Main] Début du programme.");

    // lancement de la coroutine
    aller_au_fast_food();

    // Preuve que le thread principal n'est pas bloqué
    std::println(std::cout, "[Main] Le client attend son burger. Pendant ce temps, le Main gère autre chose !");
    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::println(std::cout, "[Main] Fin du programme");
}














#endif //TUTO_CPP_COROUTINES_H
