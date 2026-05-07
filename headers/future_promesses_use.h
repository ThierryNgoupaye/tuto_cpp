//
// Created by thierry on 05/05/2026.
//

#ifndef TUTO_CPP_FUTURE_PROMESSES_USE_H
#define TUTO_CPP_FUTURE_PROMESSES_USE_H

#include <thread>
#include <future>
#include <chrono>
#include <iostream>


inline void process_value(std::promise<int> value) {
    constexpr int result = 74;
    value.set_value(result);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    std::cout << "Attribution de valeur terminée ! \n";
}

inline void apply() {
    std::promise<int> promise;
    std::future<int> futur = promise.get_future();

    std::thread worker(process_value, std::move(promise));

    std::cout << "Attente du thread promise \n";

    std::cout << futur.get() << "\n";  // bloque jusqu'a ce que la fonction process_value ait terminé de s'executer !
    worker.join();
}


inline int sum(const int a, const int b) {
    std::cout << "Entrée dans le async ..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return a + b;
}


inline void apply_async_function() {

    // passage de la fonction à executer en asynchrone ainsi que de ses parametres de facon explicite
    auto future = std::async(std::launch::async, sum, 10, 3);

    // utilisation d'une lamba
    constexpr int a = 19;
    constexpr int b = 20;

    auto future_lambda = std::async(std::launch::async, [=] {
        return sum(a,b);
    });

    std::cout << "Main faisant son travail ..." << std::endl;

    const int result = future_lambda.get();

    std::cout << result << std::endl;

}



#endif //TUTO_CPP_FUTURE_PROMESSES_USE_H
