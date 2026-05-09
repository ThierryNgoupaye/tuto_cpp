//
// Created by thierry on 08/05/2026.
//

#ifndef TUTO_CPP_THREADS_CONCEPTS_H
#define TUTO_CPP_THREADS_CONCEPTS_H

#include <iostream>
#include <thread>


using std::thread;
using std::jthread;


// Thread
inline void tache(const int id, const int n) {
    for (int t = 0; t < n; ++t) {
        std::cout << "Thread " << id << " : " <<  t << std::endl;
    }
}


inline void basic_thread_use() {

    // creer un thread
    thread t1(tache, 1,2);
    thread t2(tache, 1,3);


    // join = permet d'attendre la fin du thread
    t1.join();
    t2.join();

    // detach = thread indépendant
    thread t3 ([] {
        std::cout << "Thread détaché " << std::endl;
    });
    t3.detach();  // t3 continue de s'exécuter meme si main se termine



    int valeur = 42;
    thread t4([&valeur]() {
        std::cout << valeur << std::endl;
    });

    t4.join();
}


// Thread local
inline thread_local int compteur = 0;  // chaque thread a sa propre copie !

inline void incrementer_tl(const int n) {
    for (int i = 0; i < n; ++i) {
        compteur++;
    }
    std::cout << "Thread compteur : " << compteur << std::endl;
}

inline void thread_local_use() {
    thread t1(incrementer_tl, 5);
    thread t2(incrementer_tl, 3);


    t1.join();
    t2.join();
    // aucun conflit, car chaque thread a son propre compteur.
}



// jthread  (version améliorée de thread)
inline void j_thread_use() {

    {
        jthread t([]() {
            std::cout << "jthread" << std::endl;
        });

        // meme en cas d'exception, join() est appelé sur t
    }

    jthread t2([](const std::stop_token& st) {
        while (!st.stop_requested()) {
            std::cout << "travail ..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        std::cout << "arret demandé !" << std::endl;
    });


    std::this_thread::sleep_for(std::chrono::milliseconds(350));
    t2.request_stop();
}




#endif //TUTO_CPP_THREADS_CONCEPTS_H
