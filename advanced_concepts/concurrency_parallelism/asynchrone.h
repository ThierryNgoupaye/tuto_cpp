//
// Created by thierry on 08/05/2026.
//

#ifndef TUTO_CPP_ASYNCHRONE_H
#define TUTO_CPP_ASYNCHRONE_H

#include <functional>
#include <iostream>
#include <future>
#include <thread>


using std::thread;
using std::future;
using std::async;
using std::promise;
using std::shared_future;
using std::packaged_task;


inline int calculer_(const int a, const int b) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    return a + b;
}


// async permet de lancer une tache asynchrone et faire une autre en attendant
inline void async_use() {

    auto ftr = async(std::launch::async, calculer_, 3,5);

    std::cout << "Calcul en cours ..." << std::endl;

    const int result = ftr.get();    // attend et récupère le résultat
    std::cout << "Résultat : " << result << std::endl;


    // Promesse = canal de communication entre threads
    promise<int> promesse;   // la valeur entre par la promesse.
    future<int> future2 = promesse.get_future();    // la valeur est recupere par future

    thread t([&promesse] {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        promesse.set_value(42);
    });

    std::cout << "Valeur : " << future2.get() << std::endl;
    t.join();


    // Gestions des exceptions avec future
    auto futurExc = async(std::launch::async, [] {
        throw std::runtime_error("Erreur dans le thread !");
    });

    try {
        futurExc.get();
    } catch (std::exception& e) {
        std::cout << "Exception : " << e.what() << std::endl;
    }
}



inline void package_task_shared_future_use() {

    // la tache à executer.
    // packaged_task fabrique la promesse (entrée), et le future(sortie).
    packaged_task<int(int, int)> task([](const int a, const int b) {
        return a + b;
    });

    future<int> ftr = task.get_future();


    // Executer dans un thread
    thread t(std::move(task), 3, 5);
    std::cout << ftr.get() << std::endl;
    t.join();



    // share future = plusieurs thread attendent le meme résultat.
    promise<int> promesse;
    shared_future<int> sf = promesse.get_future().share();

    std::vector<thread> threads;
    for (int i = 0; i<3; ++i) {
        threads.emplace_back([sf, i] {
            std::cout << "Thread "  << i << " recoit : " << sf.get() << std::endl;
        });
    }

    promesse.set_value(100);
    for (auto& th : threads) th.join();
}




#endif //TUTO_CPP_ASYNCHRONE_H
