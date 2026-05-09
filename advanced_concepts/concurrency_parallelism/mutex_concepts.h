//
// Created by thierry on 08/05/2026.
//

#ifndef TUTO_CPP_MUTEX_CONCEPTS_H
#define TUTO_CPP_MUTEX_CONCEPTS_H

#include <thread>
#include <mutex>
#include <iostream>

using std::thread;
using std::mutex;
using std::jthread;

inline int compteur = 0;
inline mutex mtx;

inline void incrementer_avec_mutex(const int n) {
    for (int i = 0; i <=n; i++) {
        mtx.lock();   // verrouillage
        compteur++;    // section critique
        mtx.unlock();   // déverrouillage
    }
}

inline void simple_mutex_use() {

    thread t1(incrementer_avec_mutex, 10);
    thread t2(incrementer_avec_mutex, 10);

    t1.join();
    t2.join();

    std::cout << compteur << std::endl;
}



// RAII sur les mutex
inline void mutex_with_lock_guard() {
    for (int i = 0; i < 10; i++) {
        std::lock_guard lock(mtx);
        compteur++;
    }     // lock_guard verrouille et déverrouille automatiquement à la fin du scope meme si exception
}

inline void mutex_with_unlock_guard() {
    std::unique_lock lock(mtx);
    compteur ++;

    lock.unlock();  // peut déverrouiller manuellement

    // autre code, sans le mutex

    lock.lock();
    compteur ++;
    // déverrouille automatiquement à la fin du scope
}


// Eviter les deadlock en utilisant scoped_lock qui verrouille plusieurs mutex de maniere atomique et garantit, qu'il n'y aura d'inter blockage
inline mutex mtx1, mtx2;
inline void mutex_with_scoped_lock() {
    std::scoped_lock lock(mtx1, mtx2);  // verrouille plusieurs mutex sans deadlock
    compteur ++;
    //déverrouille les deux automatiquement à la fin du scope
}


// Eviter les deadlock - Solution 2 — std::lock + adopt_lock
inline void thread1_safe2() {
    std::lock(mtx1, mtx2);  // verrouille les deux atomiquement
    std::lock_guard lockA(mtx1, std::adopt_lock);
    std::lock_guard lockB(mtx2, std::adopt_lock);
}



inline void mutex_with_raii() {
    {
        jthread t1(mutex_with_lock_guard);
        jthread t2(mutex_with_lock_guard);
    }

    std::cout << compteur << std::endl;
}














#endif //TUTO_CPP_MUTEX_CONCEPTS_H
