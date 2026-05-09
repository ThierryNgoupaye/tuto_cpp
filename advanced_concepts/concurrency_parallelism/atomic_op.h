//
// Created by thierry on 08/05/2026.
//

#ifndef TUTO_CPP_ATOMIC_OP_H
#define TUTO_CPP_ATOMIC_OP_H

#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>


using std::atomic;
using std::jthread;
using std::mutex;




inline atomic compteur_{0};

inline void incrementer_atomic(const int n) {
    for (int i = 0; i < n; ++i) {
        ++compteur_;
    }
}


inline void  atomic_op_use() {
    {
        jthread t1(incrementer_atomic, 100);
        jthread t2(incrementer_atomic, 100);
    }

    std::cout << compteur_ << std::endl;

    atomic a{10};

    a.store(20);          // écriture atomique
    int val = a.load();      // lecture atomique
    a.fetch_add(5);        // += atomique
    a.fetch_sub(2);         // -= atomique
    a.fetch_and(0xFF);       // &atomique
    a.fetch_or(0x01);     // | atomique

    // compare and swap
    int expected = 45;
    bool ok = a.compare_exchange_strong(expected, 100);
    // si a == expected alors a = 100, retourne true
    // sinon expected = a, retourne false


    // atomic_flag,
    std::atomic_flag  flag = ATOMIC_FLAG_INIT;
    flag.test_and_set();    // met à true, retourne ancienne valeur
    flag.clear();
}




// Memory Ordering
inline atomic data{0};
inline atomic ready{false};


inline void producteur_() {
    data.store(42, std::memory_order_relaxed);

    ready.store(true, std::memory_order_release);
}


inline void consommateur_() {
    while (!ready.load(std::memory_order_acquire)) {

    }
    std::cout << data.load(std::memory_order_relaxed) << std::endl;
}

inline void memory_ordering_use() {
    {
        jthread t1(producteur_);
        jthread t2(consommateur_);
    }
}


// memory_order_relaxed → aucune synchronisation
//                         juste atomicité de l'opération
//                         le plus rapide !
//
// memory_order_acquire → lecture → voit tout ce qui
//                         précède le release correspondant
//
// memory_order_release → écriture → tout ce qui précède
//                         est visible après l'acquire
//
// memory_order_acq_rel → acquire + release combinés
//                         pour read-modify-write
//
// memory_order_seq_cst → ordre total garanti entre tous
//                         les threads → le plus sûr
//                         mais le plus lent !
//                         → valeur par défaut de atomic


#endif //TUTO_CPP_ATOMIC_OP_H
