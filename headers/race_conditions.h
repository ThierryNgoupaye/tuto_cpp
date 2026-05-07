//
// Created by thierry on 01/05/2026.
//

#ifndef TUTO_CPP_RACE_CONDITIONS_H
#define TUTO_CPP_RACE_CONDITIONS_H

#include <condition_variable>
#include <format>
#include <iostream>
#include <mutex>
#include <semaphore>
#include <chrono>
#include <thread>
#include <vector>


using std::vector;


inline std::mutex mtx;
inline std::condition_variable cv;
inline bool isOdd = true;
constexpr int MAX_NUMBER = 51;


inline void print_pair() {
    for (int i = 0; i <= MAX_NUMBER; i+=2) {
        std::unique_lock lock(mtx);
        cv.wait(lock, []{return isOdd;});
        std::cout << std::format("Paire : {}", i) << std::endl;
        isOdd = !isOdd;
        cv.notify_all();
    }
}

inline void print_impair() {
    for (int i = 1; i < MAX_NUMBER; i+=2) {
        std::unique_lock lock(mtx);
        cv.wait(lock, []{return !isOdd;});
        std::cout << std::format("Impair : {}", i) << std::endl;
        isOdd = !isOdd;
        cv.notify_all();
    }
}



inline  void displayLoop(const int& x) {
    std::lock_guard lock(mtx);
    for (int i = 0; i < 100; ++i) {
        std::cout << std::this_thread::get_id()  << " : " <<  x+i  << std::endl;
    }
}


inline  void incrementValue(const int n, int& x) {
    for (int i = 0; i < n; ++i) {
        std::lock_guard lock(mtx);
        x++;
        std::cout << std::this_thread::get_id() << " : " << x << std::endl;
    }
}




// Semaphore
// une variable bloquée peut etre débloquée par un autre thread contrairement aux mutex
// binary semaphore et counting semaphore

inline std::binary_semaphore sema(1);  // le verouillage n'est pas actif
inline std::counting_semaphore<4> c_sema(4);    // 4 threads peuvent de maniere asychrone gérer les fonctions


inline void worker(const int ID) {
    sema.acquire();
    std::cout << "Thread " << ID << " entre dans la section critique " << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    sema.release();
}


inline void handle_request(const int client_id) {
    std::cout << "Client " << client_id << " en attente... " << std::endl;
    c_sema.acquire();  // accéder à la section critique s'il y a encore des places libres !
    std::cout << "Client " << client_id << " est connecté " << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(4));
    std::cout << "Client " << client_id << " a terminé sa requete ! " << std::endl;
    c_sema.release();  // libere son vérou
}

inline void apply_semaphore() {

    // std::thread t1(worker, 1);
    // std::thread t2(worker, 2);
    //
    // t1.join();
    // t2.join();

    constexpr int total_clients = 10;

    vector<std::thread> clients;

    for (int i = 0; i < total_clients; ++i) {
        clients.emplace_back(handle_request, i+1);
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    for (auto& client : clients) {
        client.join();
    }

    std::cout << "Toutes les connexion sont terminées " << std::endl;
}


#endif //TUTO_CPP_RACE_CONDITIONS_H
