//
// Created by thierry on 09/05/2026.
//

#ifndef TUTO_CPP_CONDITION_VARIABLES_H
#define TUTO_CPP_CONDITION_VARIABLES_H

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <print>
#include <thread>
#include <queue>
#include <vector>


using thread = std::thread;
using mutex = std::mutex;
using condition_variable = std::condition_variable;
using string = std::string;
using std::vector;
using std::cout;
using std::queue;


inline mutex mtx_;
inline condition_variable cv;
inline bool pret = false;


inline void producteur_cv() {
    std::println(cout, "[Producteur] Je prépare quelque chose... ");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    {
        std::lock_guard lock(mtx_);
        pret = true;
    }

    std::println(cout, "[Producteur] C'est prêt ! Je réveille le consommateur.");
    cv.notify_one();   // reveille le consommateur
}


inline void consommateur_cv() {
    std::println(cout, "[Consommateur] J'attends que ce soit prêt...");
    std::unique_lock lock(mtx_);
    cv.wait(lock, []{return pret;});
    // on dort jusqu'à ce que pret soit true

    std::println(cout, "[Consommateur] Je suis réveillé ! Je traite.");
}


inline void simple_condition_variable_use() {
    thread t1(producteur_cv);
    thread t2(consommateur_cv);
    t1.join();
    t2.join();
}



// Exemple avec une file de tache partagée
inline bool arret = false;
inline queue<string> fileTaches;


inline void produce_task() {
    const vector<string> taches = {
        "Traiter image 1",
        "Traiter image 2",
        "Traiter image 3"
    };


    for (auto& tache : taches) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        {
            std::lock_guard lock(mtx_);
            fileTaches.push(tache);
            std::println(cout, "[Producteur] Ajouté : {}", tache);
        }
        cv.notify_one();   // reveille un worker
    }


    {
        std::lock_guard lock(mtx_);
        arret = true;
    }
    cv.notify_all();
}


inline void consume_task(const int id) {
    while (true) {
        std::unique_lock lock(mtx_);

        cv.wait(lock, [] {
           return !fileTaches.empty() || arret;
        });

        if (fileTaches.empty() && arret) {
            std::println(cout, "[Worker {}] Je m'arrête", id);
            return;
        }

        string tache = fileTaches.front();
        fileTaches.pop();
        lock.unlock();  // libere le mutex avant de traiter


        std::println(cout, "[Worker {}] Traite la tache \"{}\" ", id, tache);

        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
}



inline void traiter_file_taches() {
    thread prod(produce_task);
    thread t1(consume_task, 1);
    thread t2(consume_task, 2);
    prod.join();
    t1.join();
    t2.join();
}

#endif //TUTO_CPP_CONDITION_VARIABLES_H
